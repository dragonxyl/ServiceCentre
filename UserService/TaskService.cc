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
        
        //自定义键值示例
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
    * \brief 获取任务列表
    * \param[in]
    * \return
    * \remarks 初始化时候调用，从本地或网络数据库获得全部任务信息
    */
    void TaskService::getInitTaskInfo()
    {
        return;
    }

    /*!
    * \fn saveTaskInfo
    * \brief 保存任务列表
    * \param[in]
    * \return
    * \remarks 当任务信息有变更时调用
    */
    void TaskService::saveTaskInfo()
    {
        return;
    }






}
