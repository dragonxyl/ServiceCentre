#pragma once
#include "workflow/WFHttpServer.h"
#include <unordered_map>
#include <set>

#include "../Base/BaseService.hh"
#include "../interface/iTaskDispatcher.hh"
#include "../interface/iReceiver.hh"


class CommuService:public BaseService
{
public:
    CommuService(const char* serviceName) :BaseService(serviceName){};
	~CommuService() = default;


	//IService½Ó¿Ú
	virtual bool Initialize();

	virtual bool Start();

	virtual bool Stop();

	virtual void Finalize();

private:

	void LoadConfigFile();

	void GenarateDispatchers();

	void GenarateReceivers();

	void SetDispatcherProcessor(std::shared_ptr<ITaskDispatcher> pDisp, const std::string& ProcKey, const std::string& ServiceName);


private:

    std::vector<std::shared_ptr<ITaskDispatcher> > m_Dispatchers;
    std::vector<std::shared_ptr<IReceiver> > m_Receivers;

    std::unordered_map<int, std::set<int> > receiverToDispers;

}; 
