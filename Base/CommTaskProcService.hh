#pragma once
#include <unordered_map>
#include <iostream>

#include "BaseService.hh"
#include "TaskProcessor.hh"

#include "workflow/WFTaskFactory.h"


template <typename Task>
class CommTaskProcService:public BaseService, public TaskProcessor<Task>
{
public:
    CommTaskProcService(const char* serviceName) :BaseService(serviceName) {};
    virtual ~CommTaskProcService() = default;

    virtual bool Initialize() { SetFunctors(); return true; }

}; 

using HttpTaskProcService = CommTaskProcService<WFHttpTask>;
