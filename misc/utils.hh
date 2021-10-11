#pragma once

#include <stdio.h>
#include <utility>
#include "workflow/Workflow.h"
#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"
#include "workflow/WFHttpServer.h"

#ifndef _WIN32
#include <unistd.h>
#endif

struct http_task_user_data
{
	std::string url;
	WFHttpTask *proxy_task;
	bool is_keep_alive;
    http_callback_t userFunc;
};

class Proxy
{
public:
	static void http_proxy(WFHttpTask* task, const std::string& url = "", http_callback_t userFunc = nullptr);

private:
    static void http_callback(WFHttpTask *task);
    
    static void http_callback_default(WFHttpTask *task);
};

void Proxy::http_callback_default(WFHttpTask *task)
{
	int state = task->get_state();
	int error = task->get_error();
	auto *resp = task->get_resp();
//	SeriesWork *series = series_of(task);
	http_task_user_data *context = (http_task_user_data*)task->user_data;

	auto *proxy_resp = context->proxy_task->get_resp();

	/* Some servers may close the socket as the end of http response. */
	if (state == WFT_STATE_SYS_ERROR && error == ECONNRESET)
		state = WFT_STATE_SUCCESS;

	if (state == WFT_STATE_SUCCESS)
	{
		const void *body;
		size_t len;

		/* Copy the remote webserver's response, to proxy response. */
		if (resp->get_parsed_body(&body, &len))
			resp->append_output_body_nocopy(body, len);
		*proxy_resp = std::move(*resp);
		if (!context->is_keep_alive)
			proxy_resp->set_header_pair("Connection", "close");
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

		fprintf(stderr, "%s: Fetch failed. state = %d, error = %d: %s\n",
				context->url.c_str(), state, task->get_error(),
				err_string);

        //增加json状态回复
		/* As a tutorial, make it simple. And ignore reply status. */
		proxy_resp->set_status_code("404");
		proxy_resp->append_output_body_nocopy(
							"<html>404 Not Found.</html>", 27);
	}
}

void Proxy::http_callback(WFHttpTask *task)
{
	http_task_user_data* context = (http_task_user_data*)task->user_data;

    if(context->userFunc != nullptr)
    {
        context->userFunc(task);
    }    
    else
    {
        http_callback_default(task);
    }

    delete context;
}


void Proxy::http_proxy(WFHttpTask* proxy_task,const std::string& url, http_callback_t userFunc)
{
    DBG("Proxy url is: %s\n",url.c_str());


	auto *req = proxy_task->get_req();
	SeriesWork *series = series_of(proxy_task);
	WFHttpTask *http_task; /* for requesting remote webserver. */

	http_task_user_data *context = new http_task_user_data;
    
    if(url.empty())
    {
	    context->url = req->get_request_uri()+1;
    }
    else
    {
        context->url = url;
    }
	context->proxy_task = proxy_task;
	context->is_keep_alive = req->is_keep_alive();
    context->userFunc = userFunc;
    
	http_task = WFTaskFactory::create_http_task(context->url, 0, 0, Proxy::http_callback);
    
    http_task->user_data = (http_task_user_data*)context;

	const void *body;
	size_t len;

	/* Copy user's request to the new task's reuqest using std::move() */
	req->set_request_uri(http_task->get_req()->get_request_uri());
    //我们的HttpMessage实现里,解析得到的body和发送请求的body是两个域
	req->get_parsed_body(&body, &len);
	req->append_output_body_nocopy(body, len);
	*http_task->get_req() = std::move(*req);

	/* also, limit the remote webserver response size. */
	http_task->get_resp()->set_size_limit(200 * 1024 * 1024);

	*series << http_task;
}

