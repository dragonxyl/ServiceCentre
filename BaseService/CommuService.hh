#pragma once
#include "workflow/WFHttpServer.h"
#include <unordered_map>
#include <set>

#include "../Base/BaseService.hh"


class Receiver;

class CommuService:public BaseService, public IHttpTaskDispatcher
{
public:
    CommuService(const char* serviceName) :BaseService(serviceName){};
	~CommuService() = default;


	//IService接口
	virtual bool Initialize();

	virtual bool Start();

	virtual bool Stop();

	virtual void Finalize();


	//IHttpTaskDispatcher接口
    virtual void setProcessor( std::string taskId, IHttpTaskProcessor* tp, ProcessType pt = PROCESS_SYNC);
	virtual void unsetProcessor(IHttpTaskProcessor* tp);

private:

	void LoadConfigFile();

//	void log_file_create_or_renew();

	void UpdateFunctors();
private:

    std::vector<Receiver*> m_re
    std::unordered_map<int, ITaskDispatcher<TASK>*>

}; 
