#pragma once
#include "CommuService.hh"

#include <fstream>

#include "../3rdParty/njson/optional_json.hh"

#include "../interface/iServiceCentre.hh"
#include "../Base/TaskDispatcher.hh"
#include "../Base/Receiver.hh"
#include "../Base/Sender.hh"
#include "../misc/define.hh"


bool CommuService::Initialize() 
{
    LoadConfigFile();
    GenerateSender();
    GenerateDispatchers();
    GenerateReceivers();

    for(auto it: receivers_)
    {
        it->Initialize();
    }

    return true; 
};

bool CommuService::Start()
{
    for(auto it: receivers_)
    {
        it->Start();
    }

    return true;
}

bool CommuService::Stop()
{
    for(auto it: receivers_)
    {
        it->Stop();
    }

    return true;
}

void CommuService::Finalize()
{
    for(auto it: receivers_)
    {
        it->Finalize();
    } 
    receivers_.clear();
    dispatchers_.clear();

    delete p_sender_;
    p_sender_ = NULL;
}

void CommuService::LoadDefaultConfig()
{
    printf("Open config file:%s failed. Default config is loaded.\n", COMMU_CONF_FILE);

    std::vector<RouteConf> router;
    router.emplace_back(RouteConf{ "task", "TaskService",nullptr });
    router.emplace_back(RouteConf{ "algorithm", "AlgorithmService",nullptr });

    commu_conf_.dispatchers.emplace_back(DispatchersConf{0,std::move(router)});
    commu_conf_.receivers.emplace_back(ReceiversConf{ 20855, nullptr, {0} });
}


void CommuService::LoadConfigFile()
{
    using nlohmann::json;

    std::string path(p_service_centre_->get_exe_path_());
    path += COMMU_CONF_FILE;

    bool succ_flag = false;
    std::ifstream ifs(path);
    if (ifs)
    {
        json j;
        ifs >> j;
        ifs.close();

        try
        {
            j.get_to<decltype(commu_conf_)>(commu_conf_);
            succ_flag = true;
        }
        catch (const std::exception& e)
        {
            printf("File:%s parse error:%s.\n", path.c_str(),e.what());
        }
    }

    if (!succ_flag)
    {
        LoadDefaultConfig();
    }
}

void CommuService::SetDispatcherProcessor(std::shared_ptr<ITaskDispatcher> pDisp, const std::string& ProcKey, const std::string& ServiceName, ProcessType pt)
{
    IService *p_service = p_service_centre_->LookForService(ServiceName);
    if (p_service)
    {
        ITaskProcessor* p_task_proc = dynamic_cast<ITaskProcessor*>(p_service);
        if (p_task_proc)
        {
            p_task_proc->SetSender(p_sender_); //TODO 考虑一个服务在多个dispatcher中被引用时的情况
            pDisp->setProcessor(ProcKey, p_task_proc, pt);
        }
    }
}

//construct Dispatchers from the config file
void CommuService::GenerateDispatchers()
{
    IService* p_service = p_service_centre_->LookForService("DefaultService");
    ITaskProcessor* p_task_proc = dynamic_cast<ITaskProcessor*>(p_service);

    for (const auto& d_i : commu_conf_.dispatchers)
    {
        dispatchers_.emplace_back(std::dynamic_pointer_cast<ITaskDispatcher>(std::make_shared<TaskDispatcher>(d_i.index, p_task_proc)));
        for (const auto& r_i : d_i.routers)
        {
            ProcessType pt = PROCESS_SYNC;
            if (r_i.type)
            {
                if (*(r_i.type) == "ASYNC" || *(r_i.type) == "async")
                {
                    pt = PROCESS_ASYNC;
                }
            }

            SetDispatcherProcessor(dispatchers_.back(), r_i.key, r_i.service, pt);
        }
        dspatcher_index_to_obj_[d_i.index] = dispatchers_.back();
    }
}

//construct Receivers from the config file
void CommuService::GenerateReceivers()
{
    for (const auto& r_i : commu_conf_.receivers)
    {
        if (!r_i.peer_response_timeout)
        {
            receivers_.emplace_back(std::dynamic_pointer_cast<IReceiver>(std::make_shared<HttpReceiver>(r_i.port)));
        }
        else
        {
            WFServerParams params(HttpReceiver::SERVER_PARAMS_CONFIG_DEFAULT);
            params.peer_response_timeout = *(r_i.peer_response_timeout);

            receivers_.emplace_back(std::dynamic_pointer_cast<IReceiver>(std::make_shared<HttpReceiver>(r_i.port,params)));
        }

        //绑定接收者和处理器
        for (const auto& d_i : r_i.dispatchers_index)
        {
            receivers_.back()->AddDispatcher(dspatcher_index_to_obj_[d_i]);
        }
    }

    //修改全局任务参数
    struct WFGlobalSettings settings = GLOBAL_SETTINGS_DEFAULT;
    settings.endpoint_params.connect_timeout = 20 * 1000;
    settings.endpoint_params.response_timeout = 20 * 1000;;
    WORKFLOW_library_init(&settings);
}

//To construct Sender from the config file
void CommuService::GenerateSender()
{
    std::string name(p_service_centre_->get_exe_path_());
    name += DNS_CONF_FILE;

    p_sender_ = new Sender(name.c_str());
}

