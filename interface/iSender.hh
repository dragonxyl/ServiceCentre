#pragma once
#include <string>

#define AlgorithmManager "1"
#define ScheduleCentre "2"
#define AlgorithmGateway "3"
#define VideoAnalyser "4"

/*!
	* \class ISender
	* \brief �����߽ӿ�
	*/
struct ISender
{
	virtual ~ISender() {};

	/*!
		* \fn GetHostAddr
		* \brief ���Ŀ��������ַ
		* \param[in] hostName ��������
		* \return ������ַ ���� http://192168.1.2:8080
		*/
    virtual std::string GetHostAddr(const std::string& hostName) = 0;

	/*!
	* \fn SetHostAddr
	* \brief ����Ŀ��������ַ
	* \param[in] hostName ��������
	* \param[in] hostAddr ������ַ
	* \remaks ���hostAddrΪ����ɾ����Ӧ������
	*/
    virtual void SetHostAddr(const std::string& hostName, const std::string& hostAddr) = 0;
};



