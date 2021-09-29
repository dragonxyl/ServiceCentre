#pragma once
#include <unordered_map>
#include <mutex>

#include "ServiceCentre.hh"
#include "Base/TaskProcessor.hh"


/*!
	* \class ServiceCentre
	* \brief 服务中心
	*/
class ServiceCentreApp:public ServiceCentre, public HttpTaskProcessor
{
public:
	/*!
		* \fn ServiceCentre
		* \brief 构造函数，同时解析应用程序参数
		* \param[in] argc 参数数量
		* \param[in] argv 参数列表
		* \remarks 程序的第一个参数是程序路径，该参数在传入此处时已被剔除。
		*/
	ServiceCentreApp(int argc, const char *argv[]):ServiceCentre(argc,argv){};
	~ServiceCentreApp() = default;
	
    /*!
     * \fn Load
     * \brief 加载服务
     * \remarks 为服务中心加载服务后进行初始化
     */
    void Load();
    
	/*!
		* \fn Start
		* \brief 开始工作
		*/
	void Start();

	/*!
		* \fn Stop
		* \brief 停止工作
		*/
	void Stop();
	
	/*!
		* \fn Unload
		* \brief 卸载服务中心所有服务
		*/
	void Unload();

};
