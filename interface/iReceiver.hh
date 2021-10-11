#pragma once

#include "iService.hh"
/*!
	* \class IReceiver
	* \brief 接收者接口
	*/
struct IServiceCentre;
struct ITaskDispatcher;

struct IReceiver: public IService
{
	virtual ~IReceiver() {};
	/*!
		* \fn GetName
		* \brief 获得服务名
		* \return 服务名字符串指针
		* \remarks 服务名不能为空（空指针、空字符串）
		*/
	virtual const std::string GetName() const {return "IReceiver";};

	/*!
		* \fn Load
		* \brief 服务载入
		* \param[in] pOwner 服务中心接口
		* \return 若载入成功，返回true，否则返回false
		*/
	virtual bool Load(IServiceCentre* pOwner ){return true;};

	/*!
	* \fn Unload
	* \brief 服务卸载
	*/
    virtual void Unload(){};
    
    virtual void AddDispatcher(std::shared_ptr<ITaskDispatcher> disp) = 0;
};
