#pragma once
#include "TaskService.hh"
//#include "../misc/utils.hh"

namespace UserService {
    /*
    bool TaskService::Initialize()
    {
        SetFunctors();
        return true;
    }
*/
    bool TaskService::Start()
    {
        return true;
    }

    void TaskService::SetFunctors()
    {
        ADDFUNC(FUNCtest);
        
        //�Զ����ֵʾ��
        //ADDKEYFUNC(LLL,FUNCtest);
        //SetFunctor("LLL",BINDMEMFUNC1(FUNCtest));
    }

    void TaskService::FUNCtest(WFHttpTask* task)
    {
        std::cout << "This is a test Function: "<< __func__ <<" in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        return;
    }
    
    /*

    void TaskService::FUNCadd(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;
        std::string url  = "";
        Proxy::http_proxy(task,url);

        return;
    }


    void TaskService::FUNCdelete(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        return;
    }

    void TaskService::FUNCstart(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        return;
    }

    void TaskService::FUNCrefresh(WFHttpTask* task)
    {
        FUNCstart(task);
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        return;
    }

*/
    /*!
    * \fn getInitTaskInfo
    * \brief ��ȡ�����б�
    * \param[in]
    * \return
    * \remarks ��ʼ��ʱ����ã��ӱ��ػ��������ݿ���ȫ��������Ϣ
    */
    void TaskService::getInitTaskInfo()
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
    void TaskService::saveTaskInfo()
    {
        return;
    }






}
