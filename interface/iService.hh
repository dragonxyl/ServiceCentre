#pragma once
#include <string>

#include "../misc/define.hh"
/*!
	* \class IService
	* \brief 服务接口
	*/
struct IServiceCentre;

struct IService
{

	/*!
		* \fn GetName
		* \brief 获得服务名
		* \return 服务名字符串指针
		* \remarks 服务名不能为空（空指针、空字符串）
		*/
	virtual const std::string GetName() const = 0;

	/*!
		* \fn Load
		* \brief 服务载入
		* \param[in] pOwner 服务中心接口
		* \return 若载入成功，返回true，否则返回false
		*/
	virtual bool Load(IServiceCentre* pOwner) = 0;

	/*!
		* \fn Initialize
		* \brief 服务初始化，当系统初始化时调用，此时服务可以查询当前注册到服务中心的各项服务
		* \param[in] pOwner 服务中心接口
		* \return 若初始化成功，返回true，否则返回false
		* \remarks 若初始化失败，则Start/Stop都不会被调用
		*/
	virtual bool Initialize() = 0;

	/*!
		* \fn Start
		* \brief 服务开启，当所有服务加载后、业务正式启动前调用
		* \param[in] pOwner 服务中心接口
		* \return 若开启成功，返回true，否则返回false
		* \remarks 若开启失败，则Stop不会被调用，直接被Release
		*/
	virtual bool Start() = 0;
	/*!
		* \fn Stop
		* \brief 服务停止，当服务将要被卸载时调用
		* \param[in] pOwner 服务中心接口
		* \return 若停止成功，返回true，否则返回false
		* \remarks 若停止失败，会提示错误，不会被Release
		*/
	virtual bool Stop() = 0;

	/*!
		* \fn Finalize
		* \brief 服务结束，当系统结束时调用，此时服务可以反注册各项接口，并与系统断开连接
		* \param[in] pOwner 服务中心接口
		*/
	virtual void Finalize() = 0;

	/*!
	* \fn Unload
	* \brief 服务卸载
	*/
    virtual void Unload() = 0;
};
