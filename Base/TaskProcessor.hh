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

#define ADDKEYFUNC_HELPER(str,f) (m_functor_map.emplace(#str, BINDCLASSFUNC1(DECL_HELPER(this),f)))
#define ADDKEYFUNC(str,f)  ADDKEYFUNC_HELPER(FUNC##str,f)

template <typename TASK>
class TaskProcessor:public ITaskProcessor
{
    using func = std::function< void(TASK*)>;
public:
    TaskProcessor() = default;
	~TaskProcessor() = default;

	//ITaskProcessor接口
	virtual void ProcessTask(void* vTask);
    virtual void SetSender(ISender* pSender){m_pSender = pSender;};

    virtual std::string GetRequestFunc(TASK* task) { return ""; };
	
    virtual ITaskProcessor* GetTaskHandler()
    {
        return dynamic_cast<ITaskProcessor*>(this);
    }

protected:
    //子类中通过重写该函数添加对应的消息处理函数
	virtual void SetFunctors()
    {
        //通过ADDFUNC宏快速添加处理函数
        //ADDFUNC(FUNC);
        //自定义键值LLL示例
        //ADDKEYFUNC(LLL,FUNC);
        //SetFunctor("LLL",BINDMEMFUNC1(FUNC));
    };
    
    virtual void SetFunctor(const std::string& funcKey, func funcToMap) final
    {
        std::string Key = "FUNC" + funcKey;
        if (m_functor_map.find(Key) == m_functor_map.end())
        {
            m_functor_map.emplace(Key, funcToMap);
        }
    }

    virtual void FUNC(TASK* task) { DBG("This is the common processor's root."); };
    
protected:
	std::unordered_map<std::string, func> m_functor_map;
    ISender* m_pSender;
}; 

template <typename TASK>
void TaskProcessor<TASK>::ProcessTask(void* vTask)
{
    TASK *task = static_cast<TASK*>(vTask);
    std::string reqFunc = GetRequestFunc(task);

    if (reqFunc == "FUNC")
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

