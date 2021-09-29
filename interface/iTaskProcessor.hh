#pragma once
#include "workflow/WFTaskFactory.h"

#include "../misc/define.hh"

/*!
	* \interface ITaskProcessor
	* \brief ������ӿڣ��� ��������� ʵ��
	**/
template<typename TASK>
struct ITaskProcessor
{
	/*!
		* \fn void ProcessTask(TASK* task)
		* \brief ��������
		* \remarks ������������ʵ�ָýӿ�
		* �����ʵ�ַ�ʽ��ͬ�����첽���ű�
		* ͬ������ProcessTask�д������
		* �첽��ProcessTask��������ض������ڵ�ǰseries���½�һ��task���Ѷ�Ӧ�Ĵ��������ض����ݴ�����ĳ�Ա������ͨ��dispatcherӳ��������ҹ������󶨵����task��
		* \param[in] task ����ģ��ָ�����������͵�ָ��
		**/
	virtual void ProcessTask(TASK* task) = 0;
};

using IHttpTaskProcessor = ITaskProcessor<WFHttpTask>;
