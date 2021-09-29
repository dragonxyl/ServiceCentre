#pragma once
#include <unordered_map>
#include <mutex>

#include "ServiceCentre.hh"
#include "Base/TaskProcessor.hh"


/*!
	* \class ServiceCentre
	* \brief ��������
	*/
class ServiceCentreApp:public ServiceCentre, public HttpTaskProcessor
{
public:
	/*!
		* \fn ServiceCentre
		* \brief ���캯����ͬʱ����Ӧ�ó������
		* \param[in] argc ��������
		* \param[in] argv �����б�
		* \remarks ����ĵ�һ�������ǳ���·�����ò����ڴ���˴�ʱ�ѱ��޳���
		*/
	ServiceCentreApp(int argc, const char *argv[]):ServiceCentre(argc,argv){};
	~ServiceCentreApp() = default;
	
    /*!
     * \fn Load
     * \brief ���ط���
     * \remarks Ϊ�������ļ��ط������г�ʼ��
     */
    void Load();
    
	/*!
		* \fn Start
		* \brief ��ʼ����
		*/
	void Start();

	/*!
		* \fn Stop
		* \brief ֹͣ����
		*/
	void Stop();
	
	/*!
		* \fn Unload
		* \brief ж�ط����������з���
		*/
	void Unload();

};
