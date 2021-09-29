#include <iostream>
#include <climits>

#include "ServiceCentre.hh"

ServiceCentre::ServiceCentre( int argc, const char *argv[])
{
	ParseArgs(argc, argv);
}

/*
	* 在参数表中查询指定参数
	*/
const std::string ServiceCentre::GetArgument(const std::string& parameter) const
{
	auto it = m_Arguments.find(parameter);
	if (it != m_Arguments.end())
	{
		return it->second;
	}
	return "";
}


IService* ServiceCentre::LookForService(const std::string& serviceName)
{
	auto it = m_ServiceTable.find(serviceName);
	if (it != m_ServiceTable.end())
	{
		return it->second.ServicePointer;
	}
	return nullptr;
}

void ServiceCentre::Initialize()
{
	// 先初始化所有服务
	InitializeServices();
		
}

void ServiceCentre::Finalize()
{
	// 清理所有所有服务
	FinalizeServices();
		
}

/*
	* 启动所有模块
	*/
void ServiceCentre::Start()
{
	// 先启动所有服务
	StartServices();

}

/*
	* 停止所有服务
	*/
void ServiceCentre::Stop()
{

	// 通知所有服务停止
	StopServices();
		
}

/*
	* 解析程序参数
	*/
void ServiceCentre::ParseArgs( int argc, const char **argv )
{
	for (int i=0; i<argc; ++i)
	{
		std::string arg(argv[i]);
		std::string::size_type pos = arg.find_first_of('=');
		if (pos != std::string::npos)
		{
			//  -name=value形式
			std::string name = arg.substr(pos+1);
			std::string value = arg.substr(pos+1, arg.length() - pos - 1);
			m_Arguments[name] = value;
		}
		else if (i+1 < argc && argv[i+1][0] != '-')
		{
			// 下一个argv不是以 - 开头
			// 则为 -name value形式
			std::string value(argv[i+1]);
			m_Arguments[arg] = value;
			++i;
		}
		else
		{
			// -name，bool开关
			m_Arguments[arg] = "true";
		}
	}
}


bool ServiceCentre::RegistService(const std::string& serviceName, IService* pService)
{
	auto it = m_ServiceTable.find(serviceName);
	if (it != m_ServiceTable.end())
	{
		std::cout << "service collision." << it->first << std::endl;
		return false;
	}
	m_ServiceTable.emplace(serviceName, ServiceEntry(pService));
    pService->Load(this);

	return true;
}


bool ServiceCentre::UnregistService(const std::string& serviceName, IService* pService)
{
	auto it = m_ServiceTable.find(serviceName);
	if (it == m_ServiceTable.end())
	{
		std::cout << "service not found : " << it->first << std::endl;
		return false;
	}
	else if(it->second.Started)
	{
        std::cout << "service need to be stopped befor unregisting: " << it->first << std::endl;
        return false;
	}

    if(auto sp = it->second.ServicePointer)
    {
        sp->Unload();
        delete sp;
        sp = nullptr;
    }

	m_ServiceTable.erase(it);
	return true;
}

void ServiceCentre::PrintServicesHelp()
{
	for (auto it = m_ServiceTable.begin(); it != m_ServiceTable.end(); ++it)
	{
		IHelper *pHelper = dynamic_cast<IHelper*>(it->second.ServicePointer);
		if (pHelper)
		{
			std::cout << "(" << it->first << ")" << std::endl;
			pHelper->PrintHelp();
		}
	}
}

void ServiceCentre::InitializeServices()
{
	// 通知所有服务初始化
	for (auto it = m_ServiceTable.begin(); it != m_ServiceTable.end(); ++it)
	{
		auto Serv = it->second;
		if (Serv.ServicePointer && !Serv.Initialized)
		{
			Serv.ServicePointer->Initialize();
    		Serv.Initialized = true;
		}
	}
}

void ServiceCentre::FinalizeServices()
{
	// 通知所有服务初始化
	for (auto it = m_ServiceTable.begin(); it != m_ServiceTable.end(); ++it)
	{
		auto Serv = it->second;
		if (Serv.ServicePointer && !Serv.Started && Serv.Initialized)
		{
    		Serv.Initialized = false;
			Serv.ServicePointer->Finalize();
		}

	}
}

void ServiceCentre::StartServices()
{
	// 通知所有服务启动
	for (auto it = m_ServiceTable.begin(); it != m_ServiceTable.end(); ++it)
	{
		auto Serv = it->second;
		if (Serv.ServicePointer && Serv.Initialized && !Serv.Started)
		{
			Serv.ServicePointer->Start();
    		Serv.Started = true;
		}

	}
}


void ServiceCentre::StopServices()
{
	// 通知所有服务停止
	for (auto it = m_ServiceTable.begin(); it != m_ServiceTable.end(); ++it)
	{
		auto Serv = it->second;
		if (Serv.ServicePointer && Serv.Started)
		{
			Serv.Started = false;
			Serv.ServicePointer->Stop();
		}

	}
}
