#pragma once

#include "workflow/WFTaskFactory.h"
#include "../3rdParty/njson/optional_json.hh"

namespace wf_http_utils{

//rest返回成功
void ProcessRestSuccess(protocol::HttpResponse* resp);
//rest返回失败
void ProcessRestFailure(protocol::HttpResponse* resp, unsigned no);
//rest返回失败,并附加错误字符串
void ProcessRestFailureWithError(protocol::HttpResponse* resp, unsigned no, const char* error);
//处理json解析异常
void ProcessJsonException(protocol::HttpResponse* resp, const std::exception& e);


struct HttpTaskUserData
{
    //代理任务url
    std::string url;
    //原始任务指针
    WFHttpTask* ori_task;
    //原始任务json解析结果
    std::shared_ptr<nlohmann::json> p_ori_json;
    //成功处理函数(非业务)
    http_callback_t successFunc;
    //失败处理函数（非业务）
    http_callback_t failedFunc;
    //当前任务json解析结果，在回调函数中解析后赋值，可以用于后续成功处理函数处理
    std::shared_ptr<nlohmann::json> p_current_json;
    //自定义数据集合
    void* custom_data;
    HttpTaskUserData() :url(""), ori_task(nullptr), p_ori_json(nullptr), successFunc(nullptr), failedFunc(nullptr), p_current_json(nullptr), custom_data(nullptr){};
};

/*!
 * \fn HttpRestCallback
 * \brief rest风格的默认回调
 * \remarks 如果远程服务器可达，则检查restful的回复是否合法，如果合法进入用户函数处理
 * \param[in] task
**/
void HttpRestCallback(WFHttpTask* task);

/*!
 * \fn HttpProxyCallback
 * \brief 代理方法的默认回调
 * \remarks 如果远程服务器可达，就把结果原封不动返回请求源
 * \param[in] task
**/
void HttpProxyCallback(WFHttpTask* task);

/*!
 * \fn HttpProxy
 * \brief 代理方法的接口（用户自己创建代理任务）
 * \remarks 用户自己创建代理任务后调用此接口
 * \param[in] ori_task
 * \param[in] proxy_task
**/
void HttpProxy(WFHttpTask* ori_task, WFHttpTask* proxy_task);

/*!
 * \fn HttpProxy
 * \brief 代理方法的接口（用户提供url，若不提供则按照原始任务url解析）
 * \remarks 用户自己创建代理任务后调用此接口
 * \param[in] ori_task
 * \param[in] url           = ""
 * \param[in] method   = nullptr
 * \param[in] successFunc   = nullptr
 * \param[in] failedFunc    = nullptr
**/
void HttpProxy(WFHttpTask* task, const std::string& url = "", const char* method = nullptr, http_callback_t successFunc = nullptr, http_callback_t failedFunc = nullptr);

namespace formdata
{
    static const char* kBoundry = "---------gc0p4Jq0M2Yt08jU534c0p";

    static void GenerateFormdataBody(std::string& target)
    {
        target += "--";
        target += kBoundry;
        target += "--\r\n";
    }

    template<typename T1, typename T2, typename ...Args>
    static void GenerateFormdataBody(std::string& target, const T1& key, const T2& value, const Args&... args)
    {
        target += "--";
        target += kBoundry;
        target += "\r\n";
        target += "Content-Disposition: form-data; name=\"";
        target += key;
        target += "\"\r\n\r\n";
        target += value;
        target += "\r\n";

        GenerateFormdataBody(target, args...);
    }

    template<typename ...Args>
    void GenerateFormdataMsg(protocol::HttpMessage* msg, const Args&... args)
    {
        std::string buf;
        buf.reserve(300);

        buf += "multipart/form-data; boundary=";
        buf += kBoundry;

        msg->set_header_pair("Content-Type", buf.c_str());

        buf.clear();
        GenerateFormdataBody(buf, args...);
        
        msg->append_output_body(buf.c_str());
    }
}



}