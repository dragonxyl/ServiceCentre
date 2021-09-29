#pragma once
#include"AlgorithmService.hh"
#include <functional>

namespace UserService {
    /*
    bool AlgorithmService::Initialize()
    {
        SetFunctors();

        return true;
    }
*/
    bool AlgorithmService::Start()
    {
        return true;
    }

    void AlgorithmService::SetFunctors()
    {
        ADDFUNC(FUNCtest);
    }


    void AlgorithmService::FUNCtest(WFHttpTask* task)
    {
        return;
    }
}
