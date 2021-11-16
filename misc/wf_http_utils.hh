#pragma once

#include "workflow/WFTaskFactory.h"
#include "../3rdParty/njson/optional_json.hh"

namespace wf_http_utils{

//rest���سɹ�
void ProcessRestSuccess(protocol::HttpResponse* resp);
//rest����ʧ��
void ProcessRestFailure(protocol::HttpResponse* resp, unsigned no);
//rest����ʧ��,�����Ӵ����ַ���
void ProcessRestFailureWithError(protocol::HttpResponse* resp, unsigned no, const char* error);
//����json�����쳣
void ProcessJsonException(protocol::HttpResponse* resp, const std::exception& e);


struct HttpTaskUserData
{
    //��������url
    std::string url;
    //ԭʼ����ָ��
    WFHttpTask* ori_task;
    //ԭʼ����json�������
    std::shared_ptr<nlohmann::json> p_ori_json;
    //�ɹ�������(��ҵ��)
    http_callback_t successFunc;
    //ʧ�ܴ���������ҵ��
    http_callback_t failedFunc;
    //��ǰ����json����������ڻص������н�����ֵ���������ں����ɹ�����������
    std::shared_ptr<nlohmann::json> p_current_json;
    //�Զ������ݼ���
    void* custom_data;
    HttpTaskUserData() :url(""), ori_task(nullptr), p_ori_json(nullptr), successFunc(nullptr), failedFunc(nullptr), p_current_json(nullptr), custom_data(nullptr){};
};

/*!
 * \fn HttpRestCallback
 * \brief rest����Ĭ�ϻص�
 * \remarks ���Զ�̷������ɴ����restful�Ļظ��Ƿ�Ϸ�������Ϸ������û���������
 * \param[in] task
**/
void HttpRestCallback(WFHttpTask* task);

/*!
 * \fn HttpProxyCallback
 * \brief ��������Ĭ�ϻص�
 * \remarks ���Զ�̷������ɴ�Ͱѽ��ԭ�ⲻ����������Դ
 * \param[in] task
**/
void HttpProxyCallback(WFHttpTask* task);

/*!
 * \fn HttpProxy
 * \brief �������Ľӿڣ��û��Լ�������������
 * \remarks �û��Լ����������������ô˽ӿ�
 * \param[in] ori_task
 * \param[in] proxy_task
**/
void HttpProxy(WFHttpTask* ori_task, WFHttpTask* proxy_task);

/*!
 * \fn HttpProxy
 * \brief �������Ľӿڣ��û��ṩurl�������ṩ����ԭʼ����url������
 * \remarks �û��Լ����������������ô˽ӿ�
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