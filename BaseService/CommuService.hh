#pragma once
#include "workflow/WFHttpServer.h"
#include <unordered_map>
#include <set>

#include "../Base/BaseService.hh"

#include "../interface/iTaskDispatcher.hh"
#include "../interface/iReceiver.hh"
#include "../interface/iSender.hh"

#include "../misc/static_reflection.hh"

struct RouteConf
{
    std::string        key;
    std::string        service;
    std::unique_ptr<std::string>    type;
};

struct DispatchersConf
{
    unsigned int            index;
    std::vector<RouteConf>  routers;
};

struct ReceiversConf
{
    unsigned short          port;
    std::unique_ptr<int>    peer_response_timeout;
//    std::unique_ptr<int>    keep_alive_timeout;
    std::vector<unsigned int> dispatchers_index;
};

struct CommuConf
{
    std::vector<DispatchersConf>    dispatchers;
    std::vector<ReceiversConf>      receivers;
};

DEFINE_STRUCT_SCHEMA(RouteConf,
    DEFINE_STRUCT_FIELD(key),
    DEFINE_STRUCT_FIELD(service),
    DEFINE_STRUCT_FIELD(type)
);

DEFINE_STRUCT_SCHEMA(DispatchersConf,
    DEFINE_STRUCT_FIELD(index),
    DEFINE_STRUCT_FIELD(routers)
);

DEFINE_STRUCT_SCHEMA(ReceiversConf,
    DEFINE_STRUCT_FIELD(port),
    DEFINE_STRUCT_FIELD(peer_response_timeout),
    DEFINE_STRUCT_FIELD(dispatchers_index)
);

DEFINE_STRUCT_SCHEMA(CommuConf,
    DEFINE_STRUCT_FIELD(dispatchers),
    DEFINE_STRUCT_FIELD(receivers)
);


class CommuService:public BaseService
{
public:
    CommuService(const char* serviceName) :BaseService(serviceName), p_sender_(nullptr){ };
    ~CommuService() = default;


    //IService½Ó¿Ú
    virtual bool Initialize();

    virtual bool Start();

    virtual bool Stop();

    virtual void Finalize();

private:
    void LoadDefaultConfig();
    void LoadConfigFile();

    void SetDispatcherProcessor(std::shared_ptr<ITaskDispatcher> pDisp, const std::string& ProcKey, const std::string& ServiceName, ProcessType pt);

    void GenerateDispatchers();

    void GenerateReceivers();

    void GenerateSender();


private:

    std::vector<std::shared_ptr<ITaskDispatcher> > dispatchers_;
    std::vector<std::shared_ptr<IReceiver> > receivers_;

    std::unordered_map<unsigned, std::weak_ptr<ITaskDispatcher> > dspatcher_index_to_obj_;
   
    ISender* p_sender_;

    CommuConf commu_conf_;
};