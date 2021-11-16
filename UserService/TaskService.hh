#pragma once
#include <vector>

#include "../Base/CommTaskProcService.hh"
#include "../protocol/protocol.hh"

#define DEFINE_PREFIXED_FUNC_TASKSERV(NAME) DEFINE_PREFIXED_FUNC_HTTP(TaskService,NAME)

namespace user_service {

class TaskService :public HttpTaskProcService
{
    friend class TaskServiceImpl;
public:
    TaskService(const char* serviceName);
    virtual ~TaskService();

    //IService½Ó¿Ú
//    virtual bool Initialize() override;

//    virtual bool Start() override;

protected:
    virtual void SetFunctors() override;
    //virtual void funcRoot(WFHttpTask* task) override;
private:

    DECLARE_PREFIXED_FUNC_HTTP(test)
    {
        std::cout << "This is a test Function: " << __func__ << " in TaskServices with pid= " << GetCurrentThreadId() << std::endl;
    }

    DECLARE_PREFIXED_FUNC_HTTP(start);
    DECLARE_PREFIXED_FUNC_HTTP(stop);

private:
    //TaskServiceImpl* pImpl_;
    std::unique_ptr<TaskServiceImpl> p_impl_;

};
}