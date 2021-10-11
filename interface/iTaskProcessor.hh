#pragma once

#include "../misc/define.hh"

#include "iSender.hh"
/*!
	* \interface ITaskProcessor
	* \brief 任务处理接口，由 任务处理服务 实现
	**/
struct ITaskProcessor
{
	virtual ~ITaskProcessor() {};

	/*!
		* \fn void ProcessTask(void* task)
		* \brief 处理任务
		* \remarks 任务处理服务必须实现该接口
		* 任务的实现方式：同步、异步、脚本
		* 同步：在ProcessTask中处理完成
		* 异步：ProcessTask函数针对特定请求在当前series中新建一个task，把对应的处理函数（特定数据处理类的成员函数，通过dispatcher映射表来查找工作）绑定到这个task上
		* \param[in] task 根据模板指定的任务类型的指针
		**/
	virtual void ProcessTask(void* task) = 0;

    virtual void SetSender(ISender* pSender) = 0;
};

