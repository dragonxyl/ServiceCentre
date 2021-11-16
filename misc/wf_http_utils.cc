#pragma once

#include "wf_http_utils.hh"

#include <stdio.h>
#include <string.h>

#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"

#include "define.hh"
#include "rest_utils.hh"

namespace wf_http_utils {

void ProcessRestSuccess(protocol::HttpResponse* resp)
{
    size_t len;
    const char* msg = RestUtils::GetJsonMsg(kRestStatusSuccess, len);
    resp->append_output_body(msg, len);
}

void ProcessRestFailure(protocol::HttpResponse* resp, unsigned no)
{
    protocol::HttpUtil::set_response_status(resp, HttpStatusBadRequest);
    size_t len;
    const char* msg = RestUtils::GetJsonMsg(no, len);
    resp->append_output_body(msg, len);
}

void ProcessRestFailureWithError(protocol::HttpResponse* resp, unsigned no,const char* error)
{
    static constexpr size_t buf_len = 500;
    char buf[buf_len];
    size_t len = ::RestUtils::GenerateJsonMsg(buf, buf_len, no, error);

    protocol::HttpUtil::set_response_status(resp, HttpStatusBadRequest);
    resp->append_output_body(buf, len);
}


void ProcessJsonException(protocol::HttpResponse* resp, const std::exception& e)
{
    DBG("Json Format excep:%s", e.what());

    ProcessRestFailureWithError(resp, kRestStatusInvalidArgument, e.what());
}


void HttpRestCallback(WFHttpTask* task)
{
    using nlohmann::json;

    int state = task->get_state();
    int error = task->get_error();
    auto* resp = task->get_resp();

    //针对单一client任务，可能没有创建用户数据，在此处创建
    HttpTaskUserData* context = (HttpTaskUserData*)task->user_data;
    if (context == nullptr)
    {
        context = new HttpTaskUserData;
        task->user_data = (void*)context;
    }

    //单一client任务，也不需要做回复
    protocol::HttpResponse* ori_resp = nullptr;
    if (context->ori_task != nullptr)
    {
        ori_resp = context->ori_task->get_resp();
    }

    /* Some servers may close the socket as the end of http response. */
    if (state == WFT_STATE_SYS_ERROR && error == ECONNRESET)
        state = WFT_STATE_SUCCESS;

    //获得远程回复，根据rest风格判断
    if (state == WFT_STATE_SUCCESS)
    {
        const void* body;
        size_t len;
        bool respFlag = false;
        if (resp->get_parsed_body(&body, &len))
        {
            try
            {
                context->p_current_json = std::make_shared<json>(json::parse((char*)body));
                respFlag = true;
            }
            //JSON回复解析异常
            catch (const std::exception& e)
            {
                context->p_current_json.reset();
                fprintf(stderr, "%s\n", e.what());
            }
        }

        //获取数据失败
        if (!respFlag)
        {
            if(ori_resp)
            ProcessRestFailure(ori_resp, kRestStatusRemoteHostReturnInvalid);
        }
        else 
        {
            if (context->successFunc != nullptr)
            {
                context->successFunc(task);
            }
            //默认回复200 ok
            else if (ori_resp)
            {
                protocol::HttpUtil::set_response_status(ori_resp, HttpStatusOK);
                ProcessRestSuccess(ori_resp);
            }
        }
    }
    //代理任务没有获得远程主机回复
    else
    {
        if (context->failedFunc != nullptr)
        {
            context->failedFunc(task);
        }
        else
        {
            const char* err_string;
            int error = task->get_error();

            if (state == WFT_STATE_SYS_ERROR)
                err_string = strerror(error);
            else if (state == WFT_STATE_DNS_ERROR)
                err_string = gai_strerror(error);
            else if (state == WFT_STATE_SSL_ERROR)
                err_string = "SSL error";
            else /* if (state == WFT_STATE_TASK_ERROR) */
                err_string = "URL error (Cannot be a HTTPS proxy)";

            fprintf(stderr, "%s: Fetch failed. state = %d, error = %d: %s\n",
                context->url.c_str(), state, task->get_error(),
                err_string);

            //增加json状态回复
            if (ori_resp)
            { 
                protocol::HttpUtil::set_response_status(ori_resp, HttpStatusBadRequest);
                ProcessRestFailure(ori_resp, kRestStatusRemoteHostUnavailable);
            }
        }
    }

    if (task->user_data != NULL)
    {
        task->user_data = NULL;
        delete context;
    }
}

void HttpProxyCallback(WFHttpTask *task)
{
    int state = task->get_state();
    int error = task->get_error();
    auto *resp = task->get_resp();
    HttpTaskUserData *const context = (HttpTaskUserData*)task->user_data;

    auto *proxy_resp = context->ori_task->get_resp();

    /* Some servers may close the socket as the end of http response. */
    if (state == WFT_STATE_SYS_ERROR && error == ECONNRESET)
        state = WFT_STATE_SUCCESS;

    //获得远程回复，但具体内容不详，默认处理直接转发回复
    if (state == WFT_STATE_SUCCESS)
    {
        if (context->successFunc != nullptr)
        {
            context->successFunc(task);
        }
        else
        {
            const void *body;
            size_t len;

            /* Copy the remote webserver's response, to proxy response. */
            if (resp->get_parsed_body(&body, &len))
                resp->append_output_body_nocopy(body, len);
            *proxy_resp = std::move(*resp);
        }
    }
    //代理任务没有获得远程主机回复
    else
    {
        if (context->failedFunc != nullptr)
        {
            context->failedFunc(task);
        }
        else
        {        
            const char *err_string;
            int error = task->get_error();

            if (state == WFT_STATE_SYS_ERROR)
                err_string = strerror(error);
            else if (state == WFT_STATE_DNS_ERROR)
                err_string = gai_strerror(error);
            else if (state == WFT_STATE_SSL_ERROR)
                err_string = "SSL error";
            else /* if (state == WFT_STATE_TASK_ERROR) */
                err_string = "URL error (Cannot be a HTTPS proxy)";

            fprintf(stderr, "%s: Fetch failed. state = %d, error = %d: %s (timeout?:%d)\n",
                    context->url.c_str(), state, task->get_error(),
                    err_string, task->get_timeout_reason());
            
            protocol::HttpUtil::set_response_status(proxy_resp, HttpStatusBadRequest);
            ProcessRestFailure(proxy_resp, kRestStatusRemoteHostUnavailable);
         }
    }

    if(task->user_data != NULL)
        delete context;
}
void HttpProxy(WFHttpTask* ori_task, WFHttpTask* proxy_task)
{
    auto* req = ori_task->get_req();

    const void* body;
    size_t len;

    /* Copy user's request to the new task's reuqest using std::move() */
    //修改请求uri和方法为新任务设置的uri和方法，主要是为了后面通过move req把整个req移动到proxy_task的req中
    //所以使用代理模式处理的任务不能再进行后续的链式处理
    req->set_request_uri(proxy_task->get_req()->get_request_uri());
    req->set_method(proxy_task->get_req()->get_method());
    //我们的HttpMessage实现里,解析得到的body和发送请求的body是两个域
    req->get_parsed_body(&body, &len);
    req->append_output_body_nocopy(body, len);
    *proxy_task->get_req() = std::move(*req);

    /* also, limit the remote webserver response size. */
   // proxy_task->get_resp()->set_size_limit(200 * 1024 * 1024);

    SeriesWork* series = series_of(ori_task);
    *series << proxy_task;
}

void HttpProxy(WFHttpTask* ori_task,const std::string& url, const char* method, http_callback_t successFunc, http_callback_t failedFunc)
{
    DBG("Proxy url is: %s\n",url.c_str());

    auto *req = ori_task->get_req();

    HttpTaskUserData *context = new HttpTaskUserData;
    if(url.empty())
    {
        context->url = req->get_request_uri()+1;
    }
    else
    {
        context->url = url;
    }
    context->ori_task = ori_task;
    context->successFunc = successFunc;
    context->failedFunc = failedFunc;

    WFHttpTask* http_task = WFTaskFactory::create_http_task(context->url, 0, 0, HttpProxyCallback);
    if(method) http_task->get_req()->set_method(method);
    http_task->user_data = (HttpTaskUserData*)context;

    HttpProxy(ori_task, http_task);
}

/*
void HttpRestProxy(WFHttpTask* task, const std::string& url = "", http_callback_t successFunc = nullptr, http_callback_t failedFunc = nullptr)
{

    HttpProxy(task, std::move(url), [](WFHttpTask* httpApplyTask) {
        auto* resp = httpApplyTask->get_resp();
        HttpTaskUserData* context = (HttpTaskUserData*)httpApplyTask->user_data;
        auto* proxy_resp = context->proxy_task->get_resp();

        const void* body;
        size_t len;
        json jBody;
        bool respFlag = false;
        if (resp->get_parsed_body(&body, &len))
        {
            try
            {
                jBody = json::parse((char*)body);
                respFlag = true;
            }
            //JSON回复解析异常
            catch (const std::exception& e)
            {
                fprintf(stderr, "%s\n", e.what());
            }
        }

        //获取数据失败
        if (!respFlag)
        {
            protocol::HttpUtil::set_response_status(proxy_resp, HttpStatusBadRequest);
            proxy_resp->append_output_body(JSONFAILED2, constStrLength(JSONFAILED2));
            return;
        }

        //数据获取成功--业务成功
        if (jBody["code"] == "0")
        {

            TODO//save the task info
                proxy_resp->append_output_body(JSONSUCCESS, constStrLength(JSONSUCCESS));

        }
        //业务失败
        else
        {
            // Copy the remote webserver's response, to proxy response.
            if (resp->get_parsed_body(&body, &len))
                resp->append_output_body_nocopy(body, len);
            *proxy_resp = std::move(*resp);
        }
        });
}

*/

}