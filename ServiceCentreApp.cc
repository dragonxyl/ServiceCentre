#include <iostream>
#include <climits>

#include "ServiceCentreApp.hh"
#include "Services.hh"


void ServiceCentreApp::Load()
{
	std::string CommuSrvName = "CommuService";
    CommuService  *pCommu = new CommuService(CommuSrvName.c_str());
    RegistService(CommuSrvName, dynamic_cast<IService*>(pCommu));

	std::string AlgSrvName = "AlgorithmService";
    UserService::AlgorithmService * pAlg = new UserService::AlgorithmService(AlgSrvName.c_str());
    RegistService(AlgSrvName, dynamic_cast<IService*>(pAlg));

	std::string TaskSrvName = "TaskService";
    UserService::TaskService *pTask = new UserService::TaskService(TaskSrvName.c_str());
    RegistService(TaskSrvName, dynamic_cast<IService*>(pTask));
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
