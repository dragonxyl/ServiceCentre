#pragma once
#include <unordered_map>
#include <mutex>

#include "interface/iServiceCentre.hh"
#include "interface/iHelper.hh"
/*!
	* \class ServiceCentre
	* \brief ��������
	*/
class ServiceCentre : public IServiceCentre
{
public:
	/*!
		* \fn ServiceCentre
		* \brief ���캯����ͬʱ����Ӧ�ó������
		* \param[in] argc ��������
		* \param[in] argv �����б�
		* \remarks ����ĵ�һ�������ǳ���·�����ò����ڴ���˴�ʱ�ѱ��޳���
		*/
	ServiceCentre(int argc, const char *argv[]);
	~ServiceCentre() = default;
	
	//IServiceCentre�ӿ�

	// ��ȡ����
	virtual const std::string GetArgument(const std::string& parameter) const;
	// ע�����
	virtual bool RegistService(const std::string& serviceName, IService* pService);
	// ��ע�����
	virtual bool UnregistService(const std::string& serviceName, IService* pService);
	// ���ҷ���
	virtual IService* LookForService(const std::string& serviceName);
	
	/*!
		* \fn Initialize
		* \brief ��ʼ�����з���
		*/
	void Initialize();
	/*!
		* \fn Finalize
		* \brief �������з���
		*/	
    void Finalize();
	/*!
		* \fn Start
		* \brief �������з���
		*/
	void Start();

	/*!
		* \fn Stop
		* \brief ֹͣ���з���
		*/
	void Stop();

private:
	/*!
		* \fn ParseArgs
		* \brief �����������
		* \param[in] argc ��������
		* \param[in] argv �����б�
		* \remarks ����ĵ�һ�������ǳ���·�����ò����ڴ���˴�ʱ�ѱ��޳���
		*/
	void ParseArgs(int argc, const char **argv);

	/*!
		* \fn InitializeServices
		* \brief ��ʼ�����з���
		*/
	void InitializeServices();
	/*!
		* \fn FinalizeServices
		* \brief �������з���
		*/
	void FinalizeServices();
	/*!
		* \fn StartServices
		* \brief �������з���
		*/
	void StartServices();

	/*!
		* \fn StopServices
		* \brief ֹͣ���з���
		*/
	void StopServices();

protected:
	/*!
		* \fn PrintModuleHelp
		* \brief ��ӡ���������Ϣ
		*/
	void PrintServicesHelp();

protected:

	//////////////////////////////////////////////////////////////////////////
	//! ������
	std::unordered_map <std::string, std::string> m_Arguments;
	//////////////////////////////////////////////////////////////////////////
	//! �����
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
