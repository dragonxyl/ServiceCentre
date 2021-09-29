#include "CommuService.hh"

#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"

#include "../interface/iTaskDispatcher.hh"



CommuService::CommService()
{
}

CommuService::~CommService()
{
	if (pHttpServer)
	{
		pHttpServer->stop();
	}
}

void CommuService::Initial()
{
	LoadConfigFile();

	UpdateFunctors();

	http_process_t proc = std::bind(&CommService::process, this, std::placeholders::_1);
	pHttpServer.reset(new WFHttpServer(proc));
	pHttpServer->start(sConfigFile.server_port);
}

void CommuService::LoadConfigFile()
{
	sConfigFile.server_port = 20855;
}

void CommuService::setProcessor( std::string taskId, ITaskProcessor<TaskType>* tp, ProcessType pt)
{
	functor_map["/task/startTask"] = std::bind(&CommService::process, this, std::placeholders::_1);
}


