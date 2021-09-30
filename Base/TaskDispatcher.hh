#pragma once
#include "../interface/iTaskDispatcher.hh"
#include <shared_mutex>
#include <unordered_map>
#include <vector>
/*!
 * \page TaskDispatcher 任务分发器
 * <pre>
 * 
 * 任务分发器、接收者和处理者共同组成通信服务：
 * 任务分发器TaskDispatcher和接收者Receiver都属于通信服务，他们独立存在
 * 他们之间是多对多的映射关系：即一个接收者可以绑定多个任务分发器，同时每一个任务分发器可以被多个接收者使用
 * （由于任务是独立异步调用的，发送者只是提供简单的DNS功能，此处略过）
 * 
 * 对于任务分发器中，有如下说明：
 * 1. 任务：任务是一个任务所需要的全部上下文，
 *	        对于Http任务来说，就是一个Http请求，借助Workflow框架，由WFHttpTask定义
 * 2. 任务处理对象（处理者）：任务处理对象由任务处理服务实例化后产生，他们具有不同的名称索引
 * 3. 任务处理接口：任务处理服务通过实现ITaskProcessor::ProcessTask(TASK* task)接口与任务分发器交互
                    <b>注意：由于可能在多线程上被派发，任务处理服务必须确保自己是线程安全的</b>
 * 4. 任务分发器：任务分发器通过维护一个key到 vector<ITaskProcessor*>的HashMap来实现任务分发
 *                任务被依次分发到vector中的每个Processor，如果是同步任务，则每个处理者依次处理
 *				  <b>注意：在有多个处理者情况下，每个处理者必须自己保证对任务原始数据的读写权限情况</b>
 * 5. 任务处理方式：
 *    同步：任务始终位于任务分发器的Dispatch函数所在线程中处理，函数执行完成后会异步的自动给client回复
 *    异步：Dispatch函数针对特定请求在当前serverTaskSeries中新建一个counterTask，
 *          同时任务进入处理函数线程异步处理，处理完成时通过counterTask通知serverTaskSeries完成，之后会异步的自动给client回复
 *          当针对单一任务有多个异步处理时，会在所有处理完成后才进行服务器操作
 *    脚本：通过外部脚本来处理对应请求（暂时没实现）
 * 6. 任务分发器0 被所有接收者共用，用于临时增加处理项
 * 7. 需要考虑多个异步请求的并行处理
 * 8. 默认的任务被派发到同名的处理者，用户也可以设置任意的处理者来处理任务
 * </pre>
 *
 **/

class TaskDispatcher : public ITaskDispatcher
{
public:

	struct TaskProcInfo
	{
		std::string ProcessorId; //任务可能会派发给
		ProcessType type;
		ITaskProcessor* TaskProcessorPointer;
		TaskProcInfo() = default;
		TaskProcInfo(ITaskProcessor* tpp, ProcessType tp) :TaskProcessorPointer(tpp), type(tp) {};
	};

	TaskDispatcher(const unsigned& key):DisperKey(key){};
	virtual ~TaskDispatcher();

	/*!
	 * \fn unsigned getDisperKey
	 * \brief 获取分发器索引
	 * \remarks 在服务中心在停止任务处理服务前，务必先反注册该接口
	 **/
	 //virtual const unsigned getDisperIndex();
	virtual const unsigned getDisperKey() const { return DisperKey; };

	 /*!
	  * \fn void setProcessor
	  * \brief 配置 任务处理接口
	  * \remarks
	  * \param[in] taskKey 任务键值
	  * \param[in] task 任务处理对象的指针
	  * \param[in] task 任务处理方式
	  **/
	virtual void setProcessor(const std::string& ProcKey, ITaskProcessor* tp, ProcessType pt = PROCESS_SYNC);

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief 反配置 任务处理接口
	 * \remarks 在服务中心在停止任务处理服务前，务必先反注册该接口
	 * \param[in] taskKey 任务键值
	 * \param[in] task 任务处理对象的指针
	 **/
	virtual void unsetProcessor(const std::string& ProcKey, ITaskProcessor* tp);

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief 反配置所有任务处理接口
	 * \remarks 加锁后直接清空map
	 **/
	//virtual void unsetAll();

	/*!
	 * \fn void Dispatch(ITaskProcessor<T>* tp)
	 * \brief 派发任务
	 * \remarks 通过此接口实现任务派发
	 * \param[in] task 任务指针
	 * \param[in] const std::string& assignedProcessorKey = ""
				  指定的处理者键值，默认为空根据任务名称直接匹配，也可指定
	 **/
	 
	virtual void Dispatch(const std::string& taskKey, void* task);

private:
	/*!
	 * \fn void GetRequestProc
	 * \brief 获取任务处理者键值
	 * \remarks
	 * \param[in] task 任务指针
	 * \retrun  根据当前任务获取的处理者键值
	 **/

	void ProcessGo(TaskProcInfo procInfo, void* task, void* context);

	//读写锁
	std::shared_mutex mtx;
	//有待完善，第二个参数要扩展为vector<TaskProcInfo<TASK> >
	//以实现一个key 对应多个 处理者依次处理 
	std::unordered_map<std::string, TaskProcInfo > m_dispatch_map;

    const unsigned DisperKey;

};
