#pragma once
#include <string>

#define ScheduleCentre "1"
#define SubScheduleCentre "2"
#define AlgorithmManager "3"
#define VideoAnalyser "4"
#define AlgorithmGateway "5"

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
        * \return ������ַ ���� http://192.168.1.2:8080
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



