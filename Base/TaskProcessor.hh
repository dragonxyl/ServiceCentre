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

template <typename TASK>
class TaskProcessor:public ITaskProcessor
{
    using func = std::function< void(TASK*)>;
public:
    TaskProcessor() = default;
	~TaskProcessor() = default;

	//ITaskProcessor½Ó¿Ú
	virtual void ProcessTask(void* vTask);

    virtual std::string GetRequestFunc(TASK* task) { return ""; };
	
    virtual ITaskProcessor* GetTaskHandler()
    {
        return dynamic_cast<ITaskProcessor*>(this);
    }

protected:
	virtual void SetFunctors(){};
    virtual void FUNC(TASK* task) { DBG("This is the common processor's root."); };
    
protected:
	std::unordered_map<std::string, func> m_functor_map;
}; 

template <typename TASK>
void TaskProcessor<TASK>::ProcessTask(void* vTask)
{
    TASK *task = static_cast<TASK*>(vTask);
    std::string reqFunc = GetRequestFunc(task);
    if("FUNC" == reqFunc)
    {
        FUNC(task);
        return;
    }
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

using HttpTaskProcessor = TaskProcessor<WFHttpTask>;
using HttpFunc = std::function< void(WFHttpTask* )>;

template <>
std::string HttpTaskProcessor::GetRequestFunc(WFHttpTask* task)
{
    std::string reqFunc = task->get_req()->get_request_uri();

    return "FUNC" +reqFunc.substr(reqFunc.find_last_of("/")+1);
}

