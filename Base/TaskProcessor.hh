#pragma once
#include <unordered_map>
#include <iostream>

#include "../interface/iTaskProcessor.hh"

#include "workflow/WFTaskFactory.h"

#ifndef DECL_HELPER
#define DECL_HELPER(X) std::decay<decltype(*X)>::type
#endif 
#ifndef BINDCLASSFUNC1
#define BINDCLASSFUNC1(cls,f) std::bind(&cls::f, this, std::placeholders::_1)
#endif

#define ADDMEMFUNC(cls,f) (m_functor_map.emplace(#f, BINDCLASSFUNC1(cls,f)))

#define ADDFUNC(f) ADDMEMFUNC(DECL_HELPER(this),f)
#define ADDSTATICFUNC(cls,f) (m_functor_map.emplace(#f, std::bind(&cls::f, std::placeholders::_1)))

template <typename TaskType>
class TaskProcessor:public ITaskProcessor<TaskType>
{
    using func = std::function< void(TaskType*)>;
public:
    TaskProcessor() = default;
	~TaskProcessor() = default;

	//ITaskProcessor½Ó¿Ú
	virtual void ProcessTask(TaskType* task)
	{
	    std::cout<<"This processor belong to: "<<GetName()<<std::endl;
	};

    virtual std::string GetRequestFunc(TaskType* task) { return ""; };
	
    virtual ITaskProcessor<TaskType>* GetTaskHandler()
    {
        return dynamic_cast<ITaskProcessor<TaskType>*>(this);
    }

protected:
	virtual void SetFunctors(){};
    
protected:
	std::unordered_map<std::string, func> m_functor_map;
}; 


using HttpTaskProcessor = TaskProcessor<WFHttpTask>;
using HttpFunc = std::function< void(WFHttpTask* )>;

template <>
std::string HttpTaskProcessor::GetRequestFunc(WFHttpTask* task)
{
    std::string reqFunc = task->get_req()->get_request_uri();

    return "FUNC" +reqFunc.substr(reqFunc.find_last_of("/")+1);
}

template <>
void HttpTaskProcessor::ProcessTask(WFHttpTask* task)
{
    std::string reqFunc = GetRequestFunc(task);
    auto it = m_functor_map.find(reqFunc);
    if(it!= m_functor_map.end())
    {
        it->second(task);
    }
    else
    {
        std::cout<<"cannot find function:"<<reqFunc<< " in Service:"<<dynamic_cast<IService*>(this)->GetName()<<std::endl;
    }
}

