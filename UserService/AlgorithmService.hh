#pragma once


#include "../Base/CommTaskProcService.hh"

#define DEFINE_PREFIXED_FUNC_ALGOSERV(NAME) DEFINE_PREFIXED_FUNC_HTTP(AlgorithmService,NAME)

namespace user_service {

class AlgorithmService:public HttpTaskProcService
{

    // using std::function< void(TaskType* )> = func;
public:
    AlgorithmService(const char* serviceName) : HttpTaskProcService(serviceName) {};
    virtual ~AlgorithmService() = default;


    //IService½Ó¿Ú
    //virtual bool Initialize();

    //virtual bool Start();

    //virtual bool Stop();

    //virtual void Finalize();

protected:
    virtual void SetFunctors() override;


private:

    DECLARE_PREFIXED_FUNC_HTTP(model);
    DECLARE_PREFIXED_FUNC_HTTP(add);
    DECLARE_PREFIXED_FUNC_HTTP(delete);
    DECLARE_PREFIXED_FUNC_HTTP(start);
    DECLARE_PREFIXED_FUNC_HTTP(stop);
    DECLARE_PREFIXED_FUNC_HTTP(query);
    DECLARE_PREFIXED_FUNC_HTTP(server);

    /*
    void INTERFACEtest(WFHttpTask* task);
    void INTERFACEadd(WFHttpTask* task);
    void INTERFACEdelete(WFHttpTask* task);
    void INTERFACEstart(WFHttpTask* task);
    void INTERFACEstop(WFHttpTask* task);
    void INTERFACErefresh(WFHttpTask* task);*/

    void HttpProxyHelper(WFHttpTask* ori_task, const std::string& url, const char* method = nullptr);

}; 
}
