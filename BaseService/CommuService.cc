#include "CommuService.hh"
#include "../interface/iServiceCentre.hh"

#include "../Base/TaskDispatcher.hh"
#include "../Base/Receiver.hh"

bool CommuService::Initialize() 
{
    LoadConfigFile();
    GenarateDispatchers();
    GenarateReceivers();

    for(auto it: m_Receivers)
    {
        it->Initialize();
    }
    
    return true; 
};

bool CommuService::Start()
{
    for(auto it: m_Receivers)
    {
        it->Start();
    }

    return true;
}

bool CommuService::Stop()
{
    for(auto it: m_Receivers)
    {
        it->Stop();
    }

    return true;
}

void CommuService::Finalize()
{
    for(auto it: m_Receivers)
    {
        it->Finalize();
    } 
    m_Receivers.clear();
    m_Dispatchers.clear();
}

void CommuService::LoadConfigFile()
{
    receiverToDispers[0].emplace( 0 );
}

//fllow the config file to construct Dispatchers
void CommuService::GenarateDispatchers()
{
    unsigned num = 1;
    for(unsigned i = 0; i<num; ++i)
    {
        m_Dispatchers.emplace_back(std::dynamic_pointer_cast<ITaskDispatcher>(std::make_shared<TaskDispatcher>(i)));

        SetDispatcherProcessor(m_Dispatchers[i],"task","TaskService");
        SetDispatcherProcessor(m_Dispatchers[i],"algorithm","AlgorithmService");

    }
}

//fllow the config file to construct Receivers
void CommuService::GenarateReceivers()
{
    unsigned num = 1;
    unsigned port = 20855;
    for(unsigned i = 0; i<num; ++i)
    {
        m_Receivers.emplace_back(std::dynamic_pointer_cast<IReceiver>(std::make_shared<HttpReceiver>(20855)));

        for(auto it : receiverToDispers[i])
        {
            m_Receivers[i]->AddDispatcher(m_Dispatchers[it]);
        }
    }
}

void CommuService::SetDispatcherProcessor(std::shared_ptr<ITaskDispatcher> pDisp, const std::string& ProcKey, const std::string& ServiceName)
{
    IService *pServ = m_pServiceCentre->LookForService(ServiceName);
    if(pServ)
    {
        ITaskProcessor *pTP1 = dynamic_cast<ITaskProcessor*>(pServ);       
        pDisp->setProcessor(ProcKey, pTP1);
    }
}
