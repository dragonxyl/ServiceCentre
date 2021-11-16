#pragma once
#include "TaskService.hh"

#include <mutex>
#include <condition_variable>

#include "../3rdParty/njson/optional_json.hh"
#include "workflow/HttpUtil.h"

#include "../misc/wf_http_utils.hh"

namespace user_service {
using nlohmann::json;

class TaskServiceImpl
{
public:

    struct StartTaskData {
        WFHttpTask* start_task;
    };


    TaskServiceImpl(TaskService* p_owner):p_owner_(p_owner){};
    ~TaskServiceImpl() = default;
    void GetInitTaskInfo();
    void SaveTaskInfo();

    void CheckAlgorithmStatus(WFHttpTask* task, std::shared_ptr<json> pJson);
    void ReplaceAlgId(std::shared_ptr<json>& p_ori_json, std::shared_ptr<json>& p_current_json);
    void QueryAlgorithmSuccCallback(WFHttpTask* task);
    void StartAlgorithmSuccCallback(WFHttpTask* start_algo_req);
    void StartTaskAnaysis(WFHttpTask* task, std::shared_ptr<json> p_ori_json);

    void StopTaskAnaysis(WFHttpTask* task, std::shared_ptr<json> pJson);


private:
    std::vector<VATask> container_;
    TaskService* p_owner_;
};


TaskService::TaskService(const char* serviceName) : HttpTaskProcService(serviceName)
{
    p_impl_ = std::make_unique<TaskServiceImpl>(this);
};

TaskService::~TaskService() = default;

/*
bool TaskService::Initialize()
{
    SetFunctors();

    return true;
}
    
bool TaskService::Start()
{
    return true;
}
*/
void TaskService::SetFunctors()
{
    ADDFUNC(PREFIXED_FUNC_NAME(test));

    ADDKEYFUNC(KEY_PREFIX(""), PREFIXED_FUNC_NAME(start),HttpMethodPost);
    ADDKEYFUNC(KEY_PREFIX(""), PREFIXED_FUNC_NAME(stop), HttpMethodDelete);

}

/*!
* \fn start
* \brief ��ʼ����
* \param[in] task
* \return
* \remarks  �����������£�
*           1����㷨�����Ƿ�����
*           2�㷨�����ѯ�ص�������1���ؽ����
*             2.1�����У���ת��4
*             2.2�����ڣ��������̣������ص�������ʧ��ԭ���㷨�����ڣ�
*             2.3δ���У�����������Ӧ�㷨���񣬲���ת��3���н������
*           3�����㷨����ص�������2.3���ؽ����
*             3.1�����ɹ�����ת��4
*             3.2����ʧ�ܣ��������̣������ص�������ʧ��ԭ�������㷨ʧ�ܣ�
*           4�·����񵽷�������
*           5�������Ļص�
*             5.1�ɹ������ص������ĳɹ�
*             5.2ʧ�ܣ����ص�������ʧ��ԭ�򣨷������ķ��ش���
*/
DEFINE_PREFIXED_FUNC_TASKSERV(start)        
{
    auto* req = task->get_req();
    auto* resp = task->get_resp();
    const void* body;
    size_t len;

    req->get_parsed_body(&body, &len);


    std::shared_ptr<json> p_json_start_task;

    try
    {
        p_json_start_task = std::make_shared<json>(json::parse((char*)body));
        std::string algorithm_id;
        p_json_start_task->at("algorithm_id").get_to(algorithm_id);
    }
    //JSON�����쳣
    catch (const std::exception& e)
    {
        p_json_start_task.reset();
        return wf_http_utils::ProcessJsonException(resp,e);
    }

    p_impl_->CheckAlgorithmStatus(task, p_json_start_task);


    return;
}


DEFINE_PREFIXED_FUNC_TASKSERV(stop) 
{
    auto* req = task->get_req();
    auto* resp = task->get_resp();
    const void* body;
    size_t len;

    req->get_parsed_body(&body, &len);

    /*
    protocol::HttpHeaderCursor cursor(req);

    std::string hostIp;
    if (cursor.find("Host", hostIp))
    {
        DBG("host ip is :%s", hostIp.c_str());
    }
    */

    std::shared_ptr<json> p_json_stop_task;
    std::string task_id;

    try
    {
        p_json_stop_task = std::make_shared<json>(json::parse((char*)body));
        p_json_stop_task->at("task_id").get_to(task_id);
    }
    //JSON�����쳣
    catch (const std::exception& e)
    {
        return wf_http_utils::ProcessJsonException(resp, e);
    }

    p_impl_->StopTaskAnaysis(task, p_json_stop_task);



    return;
}

/*!
* \fn getInitTaskInfo
* \brief ��ȡ�����б�
* \param[in]
* \return
* \remarks ��ʼ��ʱ����ã��ӱ��ػ��������ݿ���ȫ��������Ϣ
*/
void TaskServiceImpl::GetInitTaskInfo()
{
    return;
}

/*!
* \fn saveTaskInfo
* \brief ���������б�
* \param[in]
* \return
* \remarks ��������Ϣ�б��ʱ����
*/
void TaskServiceImpl::SaveTaskInfo()
{
    return;
}


/*!
* \fn CheckAlgorithmStatus
* \brief 1����㷨�����Ƿ����ߣ�����ڻص�����2�д���
* \param[in] ������������task
* \param[in] ������������Json�������p_json_stop_task
*/
void TaskServiceImpl::CheckAlgorithmStatus(WFHttpTask* task, std::shared_ptr<json> p_json_start_task)
{
    std::string url = p_owner_->pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/status";

    wf_http_utils::HttpTaskUserData* context = new wf_http_utils::HttpTaskUserData;
    context->url = std::move(url);
    context->ori_task = task;
    context->p_ori_json = p_json_start_task;
    context->successFunc = BINDMEMFUNC1(QueryAlgorithmSuccCallback);

    auto* query_algo_task = WFTaskFactory::create_http_task(context->url, 0, 0, wf_http_utils::HttpRestCallback);
    
    query_algo_task->user_data = (void*)context;
    auto* req = query_algo_task->get_req();
    req->set_method(HttpMethodPost);

    //body
    std::string algorithm_id;
    p_json_start_task->at("algorithm_id").get_to(algorithm_id);
    wf_http_utils::formdata::GenerateFormdataMsg(req,"service_id", std::move(algorithm_id));

    //run
    SeriesWork* series = series_of(task);
    *series << query_algo_task;
}

/*!
* \fn ReplaceAlgId
* \brief �滻�㷨IDΪ�㷨url
* \param[in] ԭʼ����json������ p_ori_json
* \param[in] �滻url rep_url
*/
void TaskServiceImpl::ReplaceAlgId(std::shared_ptr<json>& p_ori_json, std::shared_ptr<json>& p_current_json)
{
    try
    {
        p_ori_json->at("algorithm_id") = std::string("http://") + p_current_json->at("service_url").get<std::string>() + "/image";
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "%s\n", e.what());
    }
}

/*!
* \fn QueryAlgorithmSuccCallback
* \brief 2�㷨�����ѯ�ص�������1���ؽ����
*             2.1�����У���ת��4
*             2.2�����ڣ��������̣������ص�������ʧ��ԭ���㷨�����ڣ�
*             2.3δ���У�����������Ӧ�㷨���񣬲���ת��3���н������
* \param[in] �㷨�����ѯ���� query_algo_task
*/
void TaskServiceImpl::QueryAlgorithmSuccCallback(WFHttpTask* query_algo_task)
{
    wf_http_utils::HttpTaskUserData* query_algo_context = (wf_http_utils::HttpTaskUserData*)query_algo_task->user_data;

    //��ȡ�ظ����
    const unsigned code = (*(query_algo_context->p_current_json.get()))["code"];
    //�㷨������
    if (code == 0)
    {
        //TODO �޸�ԭʼ�����е��㷨url
        ReplaceAlgId(query_algo_context->p_ori_json, query_algo_context->p_current_json);
        StartTaskAnaysis(query_algo_context->ori_task, query_algo_context->p_ori_json);
    }
    //������
    else if (code == 2)
    {
        wf_http_utils::ProcessRestFailure(query_algo_context->ori_task->get_resp(), kRestStatusExCondUnsatisfied);
    }
    //û������
    else if (code == 1)
    {
        //���������㷨����
        std::string url = p_owner_->pSender_->GetHostAddr(AlgorithmManager) + "/v2/service/start";

        wf_http_utils::HttpTaskUserData* start_algo_context = new wf_http_utils::HttpTaskUserData;
        start_algo_context->url = std::move(url);
        start_algo_context->ori_task = query_algo_context->ori_task;
        start_algo_context->p_ori_json = query_algo_context->p_ori_json;
        start_algo_context->successFunc = BINDMEMFUNC1(StartAlgorithmSuccCallback);

        auto* start_algo_task = WFTaskFactory::create_http_task(start_algo_context->url, 0, 0, wf_http_utils::HttpRestCallback);

        start_algo_task->user_data = (void*)start_algo_context;

        //��������д
        auto* start_algo_req = start_algo_task->get_req();
        start_algo_req->set_method(HttpMethodPost);

        //��ȡԭʼ�㷨id
        std::string algorithm_id = (*(query_algo_context->p_ori_json.get()))["algorithm_id"];

        wf_http_utils::formdata::GenerateFormdataMsg(start_algo_req, "service_id", std::move(algorithm_id),"instances_nums","1");

        SeriesWork* series = series_of(start_algo_context->ori_task);
        *series << start_algo_task;
    }
}

/*!
* \fn StartAlgorithmSuccCallback
* \brief 3�����㷨����ص�������2.3���ؽ����
*             3.1�����ɹ�����ת��4
*             3.2����ʧ�ܣ��������̣������ص�������ʧ��ԭ�������㷨ʧ�ܣ�
* \param[in] �����㷨����start_algo_task
*/
void TaskServiceImpl::StartAlgorithmSuccCallback(WFHttpTask* start_algo_task)
{
    wf_http_utils::HttpTaskUserData* context = (wf_http_utils::HttpTaskUserData*)start_algo_task->user_data;

    const unsigned code = (*(context->p_current_json.get()))["status"];

    //�㷨�����ɹ�
    if (code == 200)
    {
        //TODO �޸�ԭʼ�����е��㷨url
        ReplaceAlgId(context->p_ori_json, context->p_current_json);
        StartTaskAnaysis(context->ori_task,context->p_ori_json);
    }
    else
    {
        wf_http_utils::ProcessRestFailure(context->ori_task->get_resp(), kRestStatusExCondUnsatisfied);
    }
}

/*!
* \fn StartTaskAnaysis
* \brief 4�·����񵽷�������
* \param[in] ԭʼ������������task
* \param[in] ԭʼ����json�����ṹ��
*/
void TaskServiceImpl::StartTaskAnaysis(WFHttpTask* task, std::shared_ptr<json> p_ori_json)
{
    std::string url = p_owner_->pSender_->GetHostAddr(VideoAnalyser) + "/startAnalysis";

    wf_http_utils::HttpTaskUserData* context = new wf_http_utils::HttpTaskUserData;
    context->url = std::move(url);
    context->ori_task = task;
    //ҵ��ɹ���ִ�������߼�
    context->successFunc = [](WFHttpTask* start_analysis_task) {

        wf_http_utils::HttpTaskUserData* context = (wf_http_utils::HttpTaskUserData*)start_analysis_task->user_data;
        auto* proxy_resp = context->ori_task->get_resp();

        //���ݻ�ȡ�ɹ�--ҵ��ɹ�
        if (context->p_current_json->at("code") == 0)
        {
            wf_http_utils::ProcessRestSuccess(proxy_resp);
        }
        //ҵ��ʧ��
        else
        {
            wf_http_utils::ProcessRestFailureWithError(proxy_resp, kRestStatusRemoteHostReturnFailure, context->p_current_json->dump().c_str());
        }
    };

    auto* start_analysis_task = WFTaskFactory::create_http_task(context->url, 0, 0, wf_http_utils::HttpRestCallback);

    start_analysis_task->user_data = (void*)context;
    auto* req = start_analysis_task->get_req();

    //Head
    req->set_method(HttpMethodPost);
    req->set_header_pair("Content-Type", "application/json");

    //Body
    req->append_output_body(p_ori_json->dump());

    //run
    SeriesWork* series = series_of(task);
    *series << start_analysis_task;
}


void TaskServiceImpl::StopTaskAnaysis(WFHttpTask* task, std::shared_ptr<json> p_json_stop_task)
{
    std::string url = p_owner_->pSender_->GetHostAddr(VideoAnalyser) + "/stopAnalysis";
    url += "?task_id=";
    std::string task_id = p_json_stop_task->at("task_id");
    url += task_id;
    wf_http_utils::HttpProxy(task, url, HttpMethodPost,[](WFHttpTask* httpApplyTask) {

        auto* resp = httpApplyTask->get_resp();
        wf_http_utils::HttpTaskUserData* context = (wf_http_utils::HttpTaskUserData*)httpApplyTask->user_data;
        auto* proxy_resp = context->ori_task->get_resp();

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
            //JSON�ظ������쳣
            catch (const std::exception& e)
            {
                fprintf(stderr, "%s\n", e.what());
            }
        }

        //��ȡ����ʧ��
        if (!respFlag)
        {
            return wf_http_utils::ProcessRestFailure(proxy_resp, kRestStatusRemoteHostReturnInvalid);
        }

        //���ݻ�ȡ�ɹ�--ҵ��ɹ�
        if (jBody["code"] == 0)
        {

            TODO//delete the task info
            wf_http_utils::ProcessRestSuccess(proxy_resp);

        }
        //ҵ��ʧ��
        else
        {
            return wf_http_utils::ProcessRestFailureWithError(proxy_resp, kRestStatusRemoteHostReturnFailure, jBody.dump().c_str());

            /* Copy the remote webserver's response, to proxy response. 
            if (resp->get_parsed_body(&body, &len))
                resp->append_output_body_nocopy(body, len);
            *proxy_resp = std::move(*resp);*/
        }
        });

}




}
