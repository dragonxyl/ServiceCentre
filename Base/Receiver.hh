#pragma once
#include "../interface/iTaskDispatcher.hh"
#include <vector>
#include "workflow/WFServer.h"
#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"

template<class REQ, class RESP> 
class Receiver final
{
    using TASK = WFNetworkTask<REQ, RESP>;

public:
    void AddDispatcher(ITaskDispatcher<TASK>* disp);

    bool Initialize();

    bool Start();

    bool Stop();

    void Finalize();
private:
    struct ServerConfig
    {
        WFServerParams server_params;
        unsigned short server_port;
    }serverConfig;

    static constexpr struct ServerConfig SERVER_CONFIG_DEFAULT =
    { {
            /*	.max_connections		=	*/	2000,
            /*	.peer_response_timeout	=	*/	10 * 1000,
            /*	.receive_timeout		=	*/	-1,
            /*	.keep_alive_timeout		=	*/	60 * 1000,
            /*	.request_size_limit		=	*/	(size_t)-1,
            /*	.ssl_accept_timeout		=	*/	10 * 1000
            },
        /*	.server_port    		=	*/  80 };

    void serverProcHandler(TASK* task);

    std::unique_ptr<WFServer<REQ, RESP> > pServer;

    //所有分发器 数组
    std::vector<ITaskDispatcher<TASK>*> m_dispatchers;

};

using HttpReceiver = Receiver<protocol::HttpRequest,protocol::HttpResponse>;

template<class REQ, class RESP>
bool Receiver<REQ,RESP>::Initialize()
{
    TODO//get server config
    serverConfig = SERVER_CONFIG_DEFAULT;
    pServer.reset(new WFServer<REQ,RESP>(serverConfig.server_params, BINDMEMFUNC1(serverProcHandler)));
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
    if(pServer) pServer->Stop;
}
template<class REQ, class RESP>
void Receiver<REQ,RESP>::Finalize()
{
    pServer.reset();
}

template<class REQ, class RESP>
void Receiver<REQ,RESP>::serverProcHandler(TASK* task)
{
    for(auto it : m_dispatchers)
    {
        it->Dispatch(task);
    }
}

