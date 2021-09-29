#pragma once


#include "../Base/CommTaskProcService.hh"

namespace UserService {

	class TaskService :public HttpTaskProcService
	{

		// using std::function< void(TaskType* )> = func;
	public:
		TaskService(const char* serviceName) : HttpTaskProcService(serviceName) {};
		TaskService() = default;


		//IService½Ó¿Ú
//		virtual bool Initialize();

		virtual bool Start();

	protected:
		virtual void SetFunctors();

	private:

		void FUNCtest(WFHttpTask* task);
	};
}
