#pragma once
#include"AlgorithmService.hh"
#include "../misc/utils.hh"

namespace UserService {
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
        ADDFUNC(FUNCtest);
        ADDFUNC(FUNCadd);
        ADDFUNC(FUNCdelete);
        ADDFUNC(FUNCstart);
        ADDFUNC(FUNCstop);
        ADDFUNC(FUNCrefresh);
    }

    void AlgorithmService::FUNCtest(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;
        return;
    }

    void AlgorithmService::FUNCadd(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        
        std::string url  = m_pSender->GetHostAddr(AlgorithmManager) + "/importAlgorithmService";
        Proxy::http_proxy(task,std::move(url));

        return;
    }
    
    void AlgorithmService::FUNCdelete(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        std::string url  = m_pSender->GetHostAddr(AlgorithmManager) + "/deleteAlgorithmService";
        Proxy::http_proxy(task,std::move(url));

        return;
    }

    void AlgorithmService::FUNCstart(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        std::string url  = m_pSender->GetHostAddr(AlgorithmManager) + "/startAlgorithmService";
        Proxy::http_proxy(task,std::move(url));

        return;
    }
    
    void AlgorithmService::FUNCstop(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        std::string url  = m_pSender->GetHostAddr(AlgorithmManager) + "/stopAlgorithmService";
        Proxy::http_proxy(task,std::move(url));

        return;
    }

    void AlgorithmService::FUNCrefresh(WFHttpTask* task)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        std::string url  = m_pSender->GetHostAddr(AlgorithmManager) + "/refreshAlgorithmService";
        Proxy::http_proxy(task,std::move(url));
        
        return;
    }
    

   
}
