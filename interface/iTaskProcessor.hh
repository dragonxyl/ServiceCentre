#pragma once

#include "../misc/define.hh"

#include "iSender.hh"
/*!
	* \interface ITaskProcessor
	* \brief ������ӿڣ��� ��������� ʵ��
	**/
struct ITaskProcessor
{
	virtual ~ITaskProcessor() {};

	/*!
		* \fn void ProcessTask(void* task)
		* \brief ��������
		* \remarks ������������ʵ�ָýӿ�
		* �����ʵ�ַ�ʽ��ͬ�����첽���ű�
		* ͬ������ProcessTask�д������
		* �첽��ProcessTask��������ض������ڵ�ǰseries���½�һ��task���Ѷ�Ӧ�Ĵ��������ض����ݴ�����ĳ�Ա������ͨ��dispatcherӳ��������ҹ������󶨵����task��
		* \param[in] task ����ģ��ָ�����������͵�ָ��
		**/
	virtual void ProcessTask(void* task) = 0;

    virtual void SetSender(ISender* pSender) = 0;
};

