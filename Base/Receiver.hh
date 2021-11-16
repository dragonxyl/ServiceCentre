#pragma once
#include "../interface/iTaskDispatcher.hh"
#include "../interface/iReceiver.hh"

#include "workflow/WFServer.h"
#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"

#include <shared_mutex>
#include <unordered_map>

template<class REQ, class RESP> 
class Receiver: public IReceiver
{
    using TASK = WFNetworkTask<REQ, RESP>;

public:
    struct ServerConfig
    {
        WFServerParams server_params;
        unsigned short server_port;
    }serverConfig;

    static constexpr struct WFServerParams SERVER_PARAMS_CONFIG_DEFAULT =
    {
        /*  .max_connections        =   */  2000,
        /*  .peer_response_timeout  =   */  20 * 1000,
        /*  .receive_timeout        =   */  -1,
        /*  .keep_alive_timeout     =   */  60 * 1000,
        /*  .request_size_limit     =   */  (size_t)-1,
        /*  .ssl_accept_timeout     =   */  10 * 1000
    };

    static constexpr struct ServerConfig SERVER_CONFIG_DEFAULT =
    { SERVER_PARAMS_CONFIG_DEFAULT,
        /*    .server_port            =    */  80 };

    Receiver() { serverConfig = SERVER_CONFIG_DEFAULT; };
    Receiver(unsigned short port) { serverConfig.server_port = port; serverConfig.server_params = SERVER_PARAMS_CONFIG_DEFAULT; };
    Receiver(unsigned short port, WFServerParams params) { serverConfig.server_port = port; serverConfig.server_params = params; }


    void AddDispatcher(std::weak_ptr<ITaskDispatcher> disp);

    bool Initialize();

    bool Start();

    bool Stop();

    void Finalize();
private:
    void serverProcHandler(TASK* task);
    
    virtual std::string GetRequestProc(TASK* task) { return ""; };

    std::unique_ptr<WFServer<REQ, RESP> > pServer;

    //所有分发器 数组
    std::vector<std::weak_ptr<ITaskDispatcher> > m_dispatchers;
    std::set<unsigned> m_dispatchersSet;
};

template<class REQ, class RESP>
bool Receiver<REQ,RESP>::Initialize()
{
    pServer = std::make_unique<WFServer<REQ,RESP> >(&serverConfig.server_params, BINDMEMFUNC1(serverProcHandler));
    return true;
}

template<class REQ, class RESP>
bool Receiver<REQ,RESP>::Start()
{
    if (pServer)
    {
        pServer->start(serverConfig.server_port);
        return true;
    }
    else
    {
        return false;
    }
}

template<class REQ, class RESP>
bool Receiver<REQ,RESP>::Stop()
{
    if(pServer) pServer->stop();
    return true;
}
template<class REQ, class RESP>
void Receiver<REQ,RESP>::Finalize()
{
    pServer.reset();
    m_dispatchers.clear();
}

template<class REQ, class RESP>
void Receiver<REQ,RESP>::AddDispatcher(std::weak_ptr<ITaskDispatcher> disp)
{
    if(m_dispatchersSet.find(disp.lock()->getDisperKey()) == m_dispatchersSet.end())
    {
        m_dispatchers.emplace_back(disp);
        m_dispatchersSet.emplace(disp.lock()->getDisperKey());
    }
    else
    {
        DBG("DisperKey %u collides.",disp.lock()->getDisperKey());
    }
}

template<class REQ, class RESP>
void Receiver<REQ,RESP>::serverProcHandler(TASK* task)
{
    for(auto it : m_dispatchers)
    {
        if(!it.expired())
        {
            std::shared_ptr<ITaskDispatcher> sp = it.lock();
            sp->Dispatch(GetRequestProc(task),(void*)task);
        }
    }
}


using HttpReceiver = Receiver<protocol::HttpRequest,protocol::HttpResponse>;


/*!
 * \fn void GetRequestProc
 * \brief http任务获取任务处理者键值
 * \remarks /algorithm 则返回algorithm
 *          /task/abc  则返回task
 * \param[in] task 任务指针
 * \retrun  根据当前任务获取的处理者键值
 **/
template <>
std::string HttpReceiver::GetRequestProc(WFHttpTask* task)
{
    std::string reqFunc = task->get_req()->get_request_uri()+1;

    std::size_t found = reqFunc.find_first_of("/");
    
    if (found!=std::string::npos)
    {
        return reqFunc.substr(0,found);
    }
    else
    {
        return reqFunc;
    }
}