#pragma once
#include"AlgorithmService.hh"
#include "../misc/wf_http_utils.hh"

namespace user_service {
/*
bool AlgorithmService::Initialize()
{
    SetFunctors();

    return true;
}

bool AlgorithmService::Start()
{
    return true;
}
*/
void AlgorithmService::SetFunctors()
{
    ADDFUNC(PREFIXED_FUNC_NAME(model), HttpMethodPost);
    ADDFUNC(PREFIXED_FUNC_NAME(server), HttpMethodGet);

    ADDKEYFUNC(KEY_PREFIX("service"), PREFIXED_FUNC_NAME(add), HttpMethodPost);
    ADDKEYFUNC(KEY_PREFIX("service"), PREFIXED_FUNC_NAME(delete), HttpMethodDelete);
    ADDKEYFUNC(KEY_PREFIX("service"), PREFIXED_FUNC_NAME(query), HttpMethodGet);

    ADDKEYFUNC(KEY_PREFIX("service_run"), PREFIXED_FUNC_NAME(start), HttpMethodPost);
    ADDKEYFUNC(KEY_PREFIX("service_run"), PREFIXED_FUNC_NAME(stop), HttpMethodDelete);

}

DEFINE_PREFIXED_FUNC_ALGOSERV(server)
{
    std::string url = pSender_->GetHostAddr(AlgorithmManager) + "/v2/worker/status";
    HttpProxyHelper(task, url, HttpMethodPost);

    return;
}
DEFINE_PREFIXED_FUNC_ALGOSERV(model)
{
    std::string url = pSender_->GetHostAddr(AlgorithmManager) + "/v2/model/import";
    HttpProxyHelper(task, url);

    return;
}
DEFINE_PREFIXED_FUNC_ALGOSERV(add)
{
    std::string url  = pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/create";
    HttpProxyHelper(task,url);

    return;
}

DEFINE_PREFIXED_FUNC_ALGOSERV(delete)
{
    std::string url  = pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/remove";
    HttpProxyHelper(task,url, HttpMethodPost);

    return;
}

DEFINE_PREFIXED_FUNC_ALGOSERV(start)
{
    std::string url  = pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/start";
    HttpProxyHelper(task,url);

    return;
}

DEFINE_PREFIXED_FUNC_ALGOSERV(stop)
{
    std::string url  = pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/stop";
    HttpProxyHelper(task,url, HttpMethodPost);

    return;
}

DEFINE_PREFIXED_FUNC_ALGOSERV(query)
{
    std::string url  = pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/status";
    HttpProxyHelper(task,url, HttpMethodPost);
        
    return;
}


void AlgorithmService::HttpProxyHelper(WFHttpTask* ori_task, const std::string& url,const char* method)
{
    wf_http_utils::HttpTaskUserData* context = new wf_http_utils::HttpTaskUserData;

    context->url = url;
    context->ori_task = ori_task;
  
    WFHttpTask* http_task = WFTaskFactory::create_http_task(context->url, 0, 0, wf_http_utils::HttpProxyCallback);
    http_task->user_data = (wf_http_utils::HttpTaskUserData*)context;

    if (method != nullptr) {
        http_task->get_req()->set_method(method);
    }
    else {
        http_task->get_req()->set_method(ori_task->get_req()->get_method());
    }
    //http_task->set_receive_timeout(20 * 1000);
    //http_task->set_keep_alive(-1);
    //http_task->set_wait_timeout(-1);

    wf_http_utils::HttpProxy(ori_task, http_task);
}




}
