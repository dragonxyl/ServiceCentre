#pragma once
#include"TaskService.hh"
#include <functional>

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
    }

    void TaskService::FUNCtest(WFHttpTask* task)
    {
        std::cout << "This is a test Function: "<< __func__ <<" in TaskServices with pid= " << GetCurrentThreadId() << std::endl;

        return;
    }

}