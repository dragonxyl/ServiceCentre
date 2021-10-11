#include "CommuService.hh"
#include "../interface/iServiceCentre.hh"

#include "../Base/TaskDispatcher.hh"
#include "../Base/Receiver.hh"

bool CommuService::Initialize() 
{
    LoadConfigFile();
    GenerateSender();
    GenerateDispatchers();
    GenerateReceivers();

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

    delete m_pSender;
    m_pSender = NULL;
}

void CommuService::LoadConfigFile()
{
    receiverToDispers[0].emplace( 0 );
}

void CommuService::SetDispatcherProcessor(std::shared_ptr<ITaskDispatcher> pDisp, const std::string& ProcKey, const std::string& ServiceName)
{
    IService *pServ = m_pServiceCentre->LookForService(ServiceName);
    if (pServ)
    {
        ITaskProcessor* pTP = dynamic_cast<ITaskProcessor*>(pServ);
        if (pTP)
        {
            pTP->SetSender(m_pSender);
            pDisp->setProcessor(ProcKey, pTP);
        }
    }
}

//To construct Dispatchers from the config file
void CommuService::GenerateDispatchers()
{
    unsigned num = 1;
    for(unsigned i = 0; i<num; ++i)
    {
        //add every dispatcher to the table
        m_Dispatchers.emplace_back(std::dynamic_pointer_cast<ITaskDispatcher>(std::make_shared<TaskDispatcher>(i)));

        //this should be done by the function of dispatcher to get info from config file for every single dispatcher
        SetDispatcherProcessor(m_Dispatchers[i],"task","TaskService");
        SetDispatcherProcessor(m_Dispatchers[i],"algorithm","AlgorithmService");

    }
}

//To construct Receivers from the config file
void CommuService::GenerateReceivers()
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

//To construct Sender from the config file
void CommuService::GenerateSender()
{
    m_pSender = new Sender();

    std::string hostAddr = "http://127.0.0.1:80";
    m_pSender->SetHostAddr(AlgorithmManager, hostAddr);
    m_pSender->SetHostAddr(ScheduleCentre, hostAddr);
    m_pSender->SetHostAddr(AlgorithmGateway, hostAddr);
    m_pSender->SetHostAddr(VideoAnalyser, hostAddr);
}

