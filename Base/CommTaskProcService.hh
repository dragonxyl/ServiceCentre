#pragma once
#include <unordered_map>
#include <iostream>

#include "BaseService.hh"
#include "TaskProcessor.hh"

#include "workflow/WFTaskFactory.h"


template <typename TASK>
class CommTaskProcService:public BaseService, public TaskProcessor<TASK>
{
public:
    CommTaskProcService(const char* serviceName) :BaseService(serviceName) {};
	~CommTaskProcService() = default;

	virtual bool Initialize() { SetFunctors(); return true; }

}; 

using HttpTaskProcService = CommTaskProcService<WFHttpTask>;
