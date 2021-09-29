#pragma once
#include "version.hh"

/*!
	* \interface IHelper
	* \brief 帮助接口
	* \remarks 实现该接口以帮助完成辅助信息
	*/
struct IHelper
{
	/*!
		* \fn Version GetVersion() const
		* \brief 获取服务
		**/
	virtual Version GetVersion() const = 0;

	/*!
		* \fn PrintHelp
		* \brief 打印帮助
		*/
	virtual void PrintHelp() const = 0;
};