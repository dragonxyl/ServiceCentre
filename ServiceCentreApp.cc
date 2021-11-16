#pragma once
#include <iostream>
#include <climits>

#include "ServiceCentreApp.hh"
#include "Services.hh"

ServiceCentreApp::ServiceCentreApp(int argc, const char* argv[]) :ServiceCentre(argc, argv) 
{
    const char* commu_srv_name = "CommuService";
    RegistService(std::string(commu_srv_name), dynamic_cast<IService*>(new CommuService(commu_srv_name)));

    const char* default_srv_name = "DefaultService";
    RegistService(std::string(default_srv_name), dynamic_cast<IService*>(new HttpTaskProcService(default_srv_name)));
}

void ServiceCentreApp::Load()
{
    const char* usr_dft_srv_name = "UserDefaultService";
    RegistService(std::string(usr_dft_srv_name), dynamic_cast<IService*>(new user_service::DefaultService(usr_dft_srv_name)));

    const char* alg_srv_name = "AlgorithmService";
    RegistService(std::string(alg_srv_name), dynamic_cast<IService*>(new user_service::AlgorithmService(alg_srv_name)));

    const char* task_srv_name = "TaskService";
    RegistService(std::string(task_srv_name), dynamic_cast<IService*>(new user_service::TaskService(task_srv_name)));

    const char* file_srv_name = "FileService";
    RegistService(std::string(file_srv_name), dynamic_cast<IService*>(new user_service::FileService(file_srv_name)));
}

void ServiceCentreApp::Unload()
{
    for(auto& it: m_ServiceTable)
    {
        if(auto sp = it.second.ServicePointer)
        {
            sp->Unload();
            delete sp;
            sp = nullptr;
        }
    }

    m_ServiceTable.clear();
}

/*
    * 启动所有模块
    */
void ServiceCentreApp::Start()
{
    Load();

    Initialize();

    ServiceCentre::Start();

}

/*
    * 停止所有服务
    */
void ServiceCentreApp::Stop()
{
    // 通知所有服务停止
    ServiceCentre::Stop();
    
    Finalize();
    
    Unload();        
}
