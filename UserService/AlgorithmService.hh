#pragma once


#include "../Base/CommTaskProcService.hh"

namespace UserService {

    class AlgorithmService:public HttpTaskProcService
    {

       // using std::function< void(TaskType* )> = func;
    public:
    	AlgorithmService(const char* serviceName) : HttpTaskProcService(serviceName) {};
    	~AlgorithmService() = default;


    	//IService½Ó¿Ú
 //   	virtual bool Initialize();

    	virtual bool Start();

    	//virtual bool Stop();

    	//virtual void Finalize();

    protected:
    	virtual void SetFunctors();

    private:

        void FUNCtest(WFHttpTask* task);
    }; 
}
