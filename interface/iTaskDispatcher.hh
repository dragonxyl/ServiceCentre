#pragma once
#include "iTaskProcessor.hh"

/*!
	* \interface ITaskDispatcher
	* \brief 任务分发接口
	**/

enum ProcessType
{
	PROCESS_SYNC,
	PROCESS_ASYNC,
	PROCESS_SCRIPT
};

struct ITaskDispatcher
{
    virtual const unsigned getDisperKey() const = 0;
	/*!
	 * \fn void setProcessor( std::string taskId, ITaskProcessor<T>* tp)
	 * \brief 配置 任务处理接口
	 * \remarks 任务的实现方式：同步、异步、脚本
	 * 同步：在ProcessTask中处理完成
	 * 异步：ProcessTask函数针对特定请求在当前series中新建一个task，把对应的处理函数（特定数据处理类的成员函数，通过dispatcher映射表来查找工作）绑定到这个task上
	 * \param[in] taskId 任务uri（需要增加匹配正则）
	 * \param[in] task 任务处理对象的指针
	 **/
	virtual void setProcessor(const std::string& ProcKey, ITaskProcessor* tp, ProcessType pt = PROCESS_SYNC) = 0;

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief 反配置 任务处理接口
	 * \remarks 在服务中心在停止任务处理服务前，务必先反注册该接口
	 * \param[in] task 任务处理对象的指针
	 **/
	virtual void unsetProcessor(const std::string& ProcKey, ITaskProcessor* tp) = 0;

    /*!
	 * \fn void Dispatch(ITaskProcessor<T>* tp)
	 * \brief 派发任务
	 * \remarks 通过此接口实现任务派发
	 * \param[in] task 任务处理对象的指针
	 **/
	virtual void Dispatch(const std::string& taskKey, void* task) = 0;

};

