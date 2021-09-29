#pragma once
#include <unordered_map>
#include <mutex>

#include "interface/iServiceCentre.hh"
#include "interface/iHelper.hh"
/*!
	* \class ServiceCentre
	* \brief 服务中心
	*/
class ServiceCentre : public IServiceCentre
{
public:
	/*!
		* \fn ServiceCentre
		* \brief 构造函数，同时解析应用程序参数
		* \param[in] argc 参数数量
		* \param[in] argv 参数列表
		* \remarks 程序的第一个参数是程序路径，该参数在传入此处时已被剔除。
		*/
	ServiceCentre(int argc, const char *argv[]);
	~ServiceCentre() = default;
	
	//IServiceCentre接口

	// 获取参数
	virtual const std::string GetArgument(const std::string& parameter) const;
	// 注册服务
	virtual bool RegistService(const std::string& serviceName, IService* pService);
	// 反注册服务
	virtual bool UnregistService(const std::string& serviceName, IService* pService);
	// 查找服务
	virtual IService* LookForService(const std::string& serviceName);
	
	/*!
		* \fn Initialize
		* \brief 初始化所有服务
		*/
	void Initialize();
	/*!
		* \fn Finalize
		* \brief 清理所有服务
		*/	
    void Finalize();
	/*!
		* \fn Start
		* \brief 开启所有服务
		*/
	void Start();

	/*!
		* \fn Stop
		* \brief 停止所有服务
		*/
	void Stop();

private:
	/*!
		* \fn ParseArgs
		* \brief 解析程序参数
		* \param[in] argc 参数数量
		* \param[in] argv 参数列表
		* \remarks 程序的第一个参数是程序路径，该参数在传入此处时已被剔除。
		*/
	void ParseArgs(int argc, const char **argv);

	/*!
		* \fn InitializeServices
		* \brief 初始化所有服务
		*/
	void InitializeServices();
	/*!
		* \fn FinalizeServices
		* \brief 清理所有服务
		*/
	void FinalizeServices();
	/*!
		* \fn StartServices
		* \brief 启动所有服务
		*/
	void StartServices();

	/*!
		* \fn StopServices
		* \brief 停止所有服务
		*/
	void StopServices();

protected:
	/*!
		* \fn PrintModuleHelp
		* \brief 打印服务帮助信息
		*/
	void PrintServicesHelp();

protected:

	//////////////////////////////////////////////////////////////////////////
	//! 参数表
	std::unordered_map <std::string, std::string> m_Arguments;
	//////////////////////////////////////////////////////////////////////////
	//! 服务表
	struct ServiceEntry 
	{
		IService* ServicePointer;
		bool Initialized;
		bool Started;

		ServiceEntry() : ServicePointer(NULL), Initialized(false), Started(false) {}
		ServiceEntry(IService* pService) : ServicePointer(pService), Initialized(false), Started(false) {}
	};

	std::unordered_map <std::string, ServiceEntry> m_ServiceTable;

};
