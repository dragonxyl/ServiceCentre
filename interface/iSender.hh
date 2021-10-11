#pragma once
#include <string>

#define AlgorithmManager "1"
#define ScheduleCentre "2"
#define AlgorithmGateway "3"
#define VideoAnalyser "4"

/*!
	* \class ISender
	* \brief 发送者接口
	*/
struct ISender
{
	virtual ~ISender() {};

	/*!
		* \fn GetHostAddr
		* \brief 获得目标主机地址
		* \param[in] hostName 主机名称
		* \return 主机地址 类似 http://192168.1.2:8080
		*/
    virtual std::string GetHostAddr(const std::string& hostName) = 0;

	/*!
	* \fn SetHostAddr
	* \brief 设置目标主机地址
	* \param[in] hostName 主机名称
	* \param[in] hostAddr 主机地址
	* \remaks 如果hostAddr为空则删除对应主机名
	*/
    virtual void SetHostAddr(const std::string& hostName, const std::string& hostAddr) = 0;
};



