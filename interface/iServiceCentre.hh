#pragma once
#include "iService.hh"

/*!
	* \class IServiceCentre
	* \brief 服务中心接口
	*/
struct IServiceCentre
{
	/*!
		* \fn GetArgument
		* \brief 获取指定参数
		* \details 获取运行时参数，形如 -name=value/-name value/-name\n
		* 若存在该参数，返回value，若为开关类参数，返回"true"/"false"\n
		* 若没有该参数，返回NULL
		* \param[in] parameter 形参，参数的名字
		* \return 实参指针，代表参数的值
		*/
	virtual const std::string GetArgument(const std::string& parameter) const = 0;

	/*!
		* \fn RegistService
		* \brief 注册服务
		* \details 服务被注册后，可通过 IModuleManager::Query 以 sid 为 key 查询该服务的指针。
		* \param[in] serviceName 服务名称
		* \param[in] pService 指向服务对象的指针
		* \return 如果注册成功，返回true，否则返回false
		* \remarks 不同Service的serviceName不同，同一个Service不得重复注册，否则都会注册失败。
		*/
	virtual bool RegistService(const std::string& serviceName, IService *pService) = 0;
	/*!
		* \fn UnregistService
		* \brief 反注册服务
		* \details 服务被反注册后，LookForService 不再可以查询该服务的指针。
		* \param[in] serviceName 服务名称
		* \param[in] pService 指向服务对象的指针
		* \return 如果反注册成功，返回true，否则返回false
		* \remarks 反注册一个不存在的服务会导致反注册失败。
		*/
	virtual bool UnregistService(const std::string& serviceName, IService* pService) = 0;

	/*!
	* \fn LookForService
	* \brief 查找服务
	* \details 服务被反注册后，不再可以查询该服务的指针。
	* \param[in] serviceName 服务名称
	* \return 如果查找成功，返回服务指针，否则返回NULL
	*/
	virtual IService* LookForService(const std::string& serviceName) = 0;


    
};
