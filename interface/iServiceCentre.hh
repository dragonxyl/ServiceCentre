#pragma once
#include "iService.hh"

/*!
	* \class IServiceCentre
	* \brief �������Ľӿ�
	*/
struct IServiceCentre
{
	/*!
		* \fn GetArgument
		* \brief ��ȡָ������
		* \details ��ȡ����ʱ���������� -name=value/-name value/-name\n
		* �����ڸò���������value����Ϊ���������������"true"/"false"\n
		* ��û�иò���������NULL
		* \param[in] parameter �βΣ�����������
		* \return ʵ��ָ�룬���������ֵ
		*/
	virtual const std::string GetArgument(const std::string& parameter) const = 0;

	/*!
		* \fn RegistService
		* \brief ע�����
		* \details ����ע��󣬿�ͨ�� IModuleManager::Query �� sid Ϊ key ��ѯ�÷����ָ�롣
		* \param[in] serviceName ��������
		* \param[in] pService ָ���������ָ��
		* \return ���ע��ɹ�������true�����򷵻�false
		* \remarks ��ͬService��serviceName��ͬ��ͬһ��Service�����ظ�ע�ᣬ���򶼻�ע��ʧ�ܡ�
		*/
	virtual bool RegistService(const std::string& serviceName, IService *pService) = 0;
	/*!
		* \fn UnregistService
		* \brief ��ע�����
		* \details ���񱻷�ע���LookForService ���ٿ��Բ�ѯ�÷����ָ�롣
		* \param[in] serviceName ��������
		* \param[in] pService ָ���������ָ��
		* \return �����ע��ɹ�������true�����򷵻�false
		* \remarks ��ע��һ�������ڵķ���ᵼ�·�ע��ʧ�ܡ�
		*/
	virtual bool UnregistService(const std::string& serviceName, IService* pService) = 0;

	/*!
	* \fn LookForService
	* \brief ���ҷ���
	* \details ���񱻷�ע��󣬲��ٿ��Բ�ѯ�÷����ָ�롣
	* \param[in] serviceName ��������
	* \return ������ҳɹ������ط���ָ�룬���򷵻�NULL
	*/
	virtual IService* LookForService(const std::string& serviceName) = 0;


    
};
