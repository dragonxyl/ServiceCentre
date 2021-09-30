#pragma once

#include "iService.hh"
/*!
	* \class IService
	* \brief ����ӿ�
	*/
struct IServiceCentre;
struct ITaskDispatcher;

struct IReceiver: public IService
{

	/*!
		* \fn GetName
		* \brief ��÷�����
		* \return �������ַ���ָ��
		* \remarks ����������Ϊ�գ���ָ�롢���ַ�����
		*/
	virtual const std::string GetName() const {return "";};

	/*!
		* \fn Load
		* \brief ��������
		* \param[in] pOwner �������Ľӿ�
		* \return ������ɹ�������true�����򷵻�false
		*/
	virtual bool Load(IServiceCentre* pOwner ){return true;};

	/*!
	* \fn Unload
	* \brief ����ж��
	*/
    virtual void Unload(){};
    
    virtual void AddDispatcher(std::shared_ptr<ITaskDispatcher> disp) = 0;
};