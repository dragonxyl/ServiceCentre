#pragma once
#include <unordered_map>
#include <iostream>

#include "../interface/iTaskProcessor.hh"
#include "../misc/rest_utils.hh"
#include "../misc/wf_http_utils.hh"

#include "workflow/WFTaskFactory.h"
#include "workflow/HttpUtil.h"

#ifndef DECL_HELPER
#define DECL_HELPER(X) std::decay<decltype(*X)>::type
#endif 
#ifndef BINDCLASSFUNC1
#define BINDCLASSFUNC1(cls,f) std::bind(&cls::f, this, std::placeholders::_1)
#endif

#define ADDKEYMEMFUNC(str,cls,f,...) SetFunctor(str, BINDCLASSFUNC1(cls,f),__VA_ARGS__)

#define ADDFUNC1(f,...) ADDKEYMEMFUNC(#f,DECL_HELPER(this),f,__VA_ARGS__)
#define ADDFUNC(f,...)  ADDFUNC1(f,__VA_ARGS__)

#define ADDKEYFUNC(str,f,...)  ADDKEYMEMFUNC(str,DECL_HELPER(this),f,__VA_ARGS__) 

#define ADDSTATICFUNC(cls,f,...) (SetFunctor(#f, cls, std::bind(&cls::f, std::placeholders::_1),__VA_ARGS__))

#define _PREFIX_ _INTERFACE_
#define FUNC_PREFIX STR(_PREFIX_)
#define KEY_PREFIX(str) FUNC_PREFIX##str

#define PREFIXED_FUNC0(PREFIX,NAME,...) PREFIX##NAME(__VA_ARGS__)
#define PREFIXED_FUNC1(PREFIX,NAME,... ) PREFIXED_FUNC0(PREFIX,NAME,__VA_ARGS__ )
#define PREFIXED_FUNC(NAME,... ) PREFIXED_FUNC1(_PREFIX_,NAME,__VA_ARGS__ )

#define PREFIXED_FUNC_NAME0(PREFIX,NAME) PREFIX##NAME
#define PREFIXED_FUNC_NAME1(PREFIX,NAME) PREFIXED_FUNC_NAME0(PREFIX,NAME)
#define PREFIXED_FUNC_NAME(NAME) PREFIXED_FUNC_NAME1(_PREFIX_,NAME)

#define DECLARE_PREFIXED_FUNC0(PREFIX,NAME,PARA) void PREFIX##NAME(PARA* task) 
#define DECLARE_PREFIXED_FUNC1(PREFIX,NAME,PARA) DECLARE_PREFIXED_FUNC0(PREFIX,NAME,PARA)
#define DECLARE_PREFIXED_FUNC(NAME,PARA) DECLARE_PREFIXED_FUNC1(_PREFIX_,NAME,PARA)
#define DECLARE_PREFIXED_FUNC_HTTP(NAME) DECLARE_PREFIXED_FUNC1(_PREFIX_,NAME,WFHttpTask)

#define DEFINE_PREFIXED_FUNC0(CLS,PREFIX,NAME,PARA) void CLS::PREFIX##NAME(PARA* task) 
#define DEFINE_PREFIXED_FUNC1(CLS,PREFIX,NAME,PARA) DEFINE_PREFIXED_FUNC0(CLS,PREFIX,NAME,PARA)
#define DEFINE_PREFIXED_FUNC(CLS,NAME,PARA) DEFINE_PREFIXED_FUNC1(CLS,_PREFIX_,NAME,PARA)
#define DEFINE_PREFIXED_FUNC_HTTP(CLS,NAME) DEFINE_PREFIXED_FUNC1(CLS,_PREFIX_,NAME,WFHttpTask)

template <typename Task>
class TaskProcessor:public ITaskProcessor
{
    using Func = std::function< void(Task*)>;
public:
    TaskProcessor() = default;
    virtual ~TaskProcessor() = default;

    //ITaskProcessor接口
    virtual void ProcessTask(void* vTask);
    virtual void SetSender(ISender* pSender){pSender_ = pSender;};

    
    virtual ITaskProcessor* GetTaskHandler()
    {
        return dynamic_cast<ITaskProcessor*>(this);
    }

protected:
    //函数式的绑定处理函数
    template <typename Str, typename... Methods>
    void SetFunctor(Str&& func_key, Func func, Methods&&... args)
    {
        if (rest_functor_map_.find(func_key) == rest_functor_map_.end())
        {
            rest_functor_map_.emplace(std::forward<Str>(func_key), std::unordered_map<std::string, Func>{ {std::forward<Methods>(args), func}... });
        }
        else
        {
            auto& it = rest_functor_map_[func_key];
            auto il = { (it[args] = func, 0)... };
        } 
    }

    template <typename Str>
    void SetFunctor(Str&& func_key, Func func)
    {
        rest_functor_map_.emplace(std::forward<Str>(func_key), std::unordered_map<std::string, Func>{ {"", func} });
    }

    //子类中通过重写该函数添加对应的消息处理函数
    virtual void SetFunctors()
    {
        //通过ADDFUNC宏快速添加处理函数
        //ADDFUNC(PREFIXED_FUNC_NAME());
        //自定义键值LLL示例
        //ADDKEYFUNC(LLL,PREFIXED_FUNC_NAME());
        //SetFunctor("LLL",BINDMEMFUNC1(PREFIXED_FUNC_NAME()));
    };

    //获取请求的函数索引
    virtual std::string GetRequestFunc(Task* task) { return ""; };
    //获取请求的函数索引
    virtual std::string GetRequestMethod(Task* task) final { return ""; };

    virtual void FuncRoot(Task* task) { DBG("This is the common processor's funcRoot."); };

    virtual void FuncMethodUndefined(Task* task) { DBG("This is the common processor's FuncMethodUndefined."); };

    virtual void FuncUndefined(Task* task) { DBG("This is the common processor's funcUndefined."); };

    //在函数处理前，设置共有属性
    virtual void SetCommonAttr(Task* task) { };

protected:
    //key-method-func
    std::unordered_map<std::string, std::unordered_map<std::string, Func>> rest_functor_map_;

    ISender* pSender_;
}; 

template <typename Task>
void TaskProcessor<Task>::ProcessTask(void* vTask)
{
    Task *task = static_cast<Task*>(vTask);

    SetCommonAttr(task);
    std::string reqMethod = GetRequestMethod(task);
    std::string reqFunc = GetRequestFunc(task);

    auto it = rest_functor_map_.find(reqFunc);
    if (it == rest_functor_map_.end())
    {
        FuncUndefined(task);
    }
    else
    {
        auto obj = it->second;
        auto itt = obj.find(reqMethod);
        if (itt == obj.end())
        {
            itt = obj.find("");
            if (itt == obj.end())
            {
                FuncMethodUndefined(task);
                return;
            }
            
        }

        itt->second(task);
    }
}

using HttpTaskProcessor = TaskProcessor<WFHttpTask>;
using HttpFunc = std::function< void(WFHttpTask* )>;

/*!
    * \fn GetRequestFunc
    * \brief Http特化的获取请求uri
    * \param[in] task Http任务
    * \return 请求键值
    * \remarks 返回第二个斜杠之后的所有字符，如果只有根斜杠，则返回空
    *           例 /abc 返回FUNC_PREFIX  /abc/ 返回FUNC_PREFIX
    *               /abc/def/g 返回(FUNC_PREFIX + def/g)
    */
template <>
std::string HttpTaskProcessor::GetRequestFunc(WFHttpTask* task)
{
    std::string req_func = task->get_req()->get_request_uri()+1;

    size_t first_slash = req_func.find_first_of("/");

    if (first_slash != std::string::npos)
    {
        return FUNC_PREFIX +req_func.substr(first_slash +1);
    }
    else
    {
        return FUNC_PREFIX;
    }
}

template <>
std::string HttpTaskProcessor::GetRequestMethod(WFHttpTask* task)
{
    return std::string(task->get_req()->get_method());
}

template <>
void HttpTaskProcessor::SetCommonAttr(WFHttpTask* task)
{
    protocol::HttpResponse* resp = task->get_resp();

    resp->set_http_version("HTTP/1.1");
    resp->set_status_code("200");
    resp->set_reason_phrase("OK");

    resp->add_header_pair("Content-Type", "application/json");
    resp->add_header_pair("Server", "WFHttpServer");
};


template <>
void HttpTaskProcessor::FuncUndefined(WFHttpTask* task)
{
    auto* resp = task->get_resp();
    wf_http_utils::ProcessRestFailure(resp, kRestStatusInvalidApi);
}

template <>
void HttpTaskProcessor::FuncMethodUndefined(WFHttpTask* task)
{
    auto* resp = task->get_resp();
    wf_http_utils::ProcessRestFailure(resp, kRestStatusUnsupportedApiMethod);
}


template <>
void HttpTaskProcessor::FuncRoot(WFHttpTask* task)
{
    FuncUndefined(task);
}