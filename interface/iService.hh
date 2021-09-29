#pragma once
#include <string>

#include "../misc/define.hh"
/*!
	* \class IService
	* \brief ����ӿ�
	*/
struct IServiceCentre;

struct IService
{

	/*!
		* \fn GetName
		* \brief ��÷�����
		* \return �������ַ���ָ��
		* \remarks ����������Ϊ�գ���ָ�롢���ַ�����
		*/
	virtual const std::string GetName() const = 0;

	/*!
		* \fn Load
		* \brief ��������
		* \param[in] pOwner �������Ľӿ�
		* \return ������ɹ�������true�����򷵻�false
		*/
	virtual bool Load(IServiceCentre* pOwner) = 0;

	/*!
		* \fn Initialize
		* \brief �����ʼ������ϵͳ��ʼ��ʱ���ã���ʱ������Բ�ѯ��ǰע�ᵽ�������ĵĸ������
		* \param[in] pOwner �������Ľӿ�
		* \return ����ʼ���ɹ�������true�����򷵻�false
		* \remarks ����ʼ��ʧ�ܣ���Start/Stop�����ᱻ����
		*/
	virtual bool Initialize() = 0;

	/*!
		* \fn Start
		* \brief �������������з�����غ�ҵ����ʽ����ǰ����
		* \param[in] pOwner �������Ľӿ�
		* \return �������ɹ�������true�����򷵻�false
		* \remarks ������ʧ�ܣ���Stop���ᱻ���ã�ֱ�ӱ�Release
		*/
	virtual bool Start() = 0;
	/*!
		* \fn Stop
		* \brief ����ֹͣ��������Ҫ��ж��ʱ����
		* \param[in] pOwner �������Ľӿ�
		* \return ��ֹͣ�ɹ�������true�����򷵻�false
		* \remarks ��ֹͣʧ�ܣ�����ʾ���󣬲��ᱻRelease
		*/
	virtual bool Stop() = 0;

	/*!
		* \fn Finalize
		* \brief �����������ϵͳ����ʱ���ã���ʱ������Է�ע�����ӿڣ�����ϵͳ�Ͽ�����
		* \param[in] pOwner �������Ľӿ�
		*/
	virtual void Finalize() = 0;

	/*!
	* \fn Unload
	* \brief ����ж��
	*/
    virtual void Unload() = 0;
};
