#pragma once
#include "iTaskProcessor.hh"

/*!
	* \interface ITaskDispatcher
	* \brief ����ַ��ӿ�
	**/

enum ProcessType
{
	PROCESS_SYNC,
	PROCESS_ASYNC,
	PROCESS_SCRIPT
};

struct ITaskDispatcher
{
    virtual const unsigned getDisperKey() const = 0;
	/*!
	 * \fn void setProcessor( std::string taskId, ITaskProcessor<T>* tp)
	 * \brief ���� ������ӿ�
	 * \remarks �����ʵ�ַ�ʽ��ͬ�����첽���ű�
	 * ͬ������ProcessTask�д������
	 * �첽��ProcessTask��������ض������ڵ�ǰseries���½�һ��task���Ѷ�Ӧ�Ĵ��������ض����ݴ�����ĳ�Ա������ͨ��dispatcherӳ��������ҹ������󶨵����task��
	 * \param[in] taskId ����uri����Ҫ����ƥ������
	 * \param[in] task ����������ָ��
	 **/
	virtual void setProcessor(const std::string& ProcKey, ITaskProcessor* tp, ProcessType pt = PROCESS_SYNC) = 0;

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief ������ ������ӿ�
	 * \remarks �ڷ���������ֹͣ���������ǰ������ȷ�ע��ýӿ�
	 * \param[in] task ����������ָ��
	 **/
	virtual void unsetProcessor(const std::string& ProcKey, ITaskProcessor* tp) = 0;

    /*!
	 * \fn void Dispatch(ITaskProcessor<T>* tp)
	 * \brief �ɷ�����
	 * \remarks ͨ���˽ӿ�ʵ�������ɷ�
	 * \param[in] task ����������ָ��
	 **/
	virtual void Dispatch(const std::string& taskKey, void* task) = 0;

};

