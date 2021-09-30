#pragma once
#include "../interface/iTaskDispatcher.hh"
#include <shared_mutex>
#include <unordered_map>
#include <vector>
/*!
 * \page TaskDispatcher ����ַ���
 * <pre>
 * 
 * ����ַ����������ߺʹ����߹�ͬ���ͨ�ŷ���
 * ����ַ���TaskDispatcher�ͽ�����Receiver������ͨ�ŷ������Ƕ�������
 * ����֮���Ƕ�Զ��ӳ���ϵ����һ�������߿��԰󶨶������ַ�����ͬʱÿһ������ַ������Ա����������ʹ��
 * �����������Ƕ����첽���õģ�������ֻ���ṩ�򵥵�DNS���ܣ��˴��Թ���
 * 
 * ��������ַ����У�������˵����
 * 1. ����������һ����������Ҫ��ȫ�������ģ�
 *	        ����Http������˵������һ��Http���󣬽���Workflow��ܣ���WFHttpTask����
 * 2. ��������󣨴����ߣ�����������������������ʵ��������������Ǿ��в�ͬ����������
 * 3. ������ӿڣ����������ͨ��ʵ��ITaskProcessor::ProcessTask(TASK* task)�ӿ�������ַ�������
                    <b>ע�⣺���ڿ����ڶ��߳��ϱ��ɷ���������������ȷ���Լ����̰߳�ȫ��</b>
 * 4. ����ַ���������ַ���ͨ��ά��һ��key�� vector<ITaskProcessor*>��HashMap��ʵ������ַ�
 *                �������ηַ���vector�е�ÿ��Processor�������ͬ��������ÿ�����������δ���
 *				  <b>ע�⣺���ж������������£�ÿ�������߱����Լ���֤������ԭʼ���ݵĶ�дȨ�����</b>
 * 5. ������ʽ��
 *    ͬ��������ʼ��λ������ַ�����Dispatch���������߳��д�������ִ����ɺ���첽���Զ���client�ظ�
 *    �첽��Dispatch��������ض������ڵ�ǰserverTaskSeries���½�һ��counterTask��
 *          ͬʱ������봦�����߳��첽�����������ʱͨ��counterTask֪ͨserverTaskSeries��ɣ�֮����첽���Զ���client�ظ�
 *          ����Ե�һ�����ж���첽����ʱ���������д�����ɺ�Ž��з���������
 *    �ű���ͨ���ⲿ�ű��������Ӧ������ʱûʵ�֣�
 * 6. ����ַ���0 �����н����߹��ã�������ʱ���Ӵ�����
 * 7. ��Ҫ���Ƕ���첽����Ĳ��д���
 * 8. Ĭ�ϵ������ɷ���ͬ���Ĵ����ߣ��û�Ҳ������������Ĵ���������������
 * </pre>
 *
 **/

class TaskDispatcher : public ITaskDispatcher
{
public:

	struct TaskProcInfo
	{
		std::string ProcessorId; //������ܻ��ɷ���
		ProcessType type;
		ITaskProcessor* TaskProcessorPointer;
		TaskProcInfo() = default;
		TaskProcInfo(ITaskProcessor* tpp, ProcessType tp) :TaskProcessorPointer(tpp), type(tp) {};
	};

	TaskDispatcher(const unsigned& key):DisperKey(key){};
	virtual ~TaskDispatcher();

	/*!
	 * \fn unsigned getDisperKey
	 * \brief ��ȡ�ַ�������
	 * \remarks �ڷ���������ֹͣ���������ǰ������ȷ�ע��ýӿ�
	 **/
	 //virtual const unsigned getDisperIndex();
	virtual const unsigned getDisperKey() const { return DisperKey; };

	 /*!
	  * \fn void setProcessor
	  * \brief ���� ������ӿ�
	  * \remarks
	  * \param[in] taskKey �����ֵ
	  * \param[in] task ����������ָ��
	  * \param[in] task ������ʽ
	  **/
	virtual void setProcessor(const std::string& ProcKey, ITaskProcessor* tp, ProcessType pt = PROCESS_SYNC);

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief ������ ������ӿ�
	 * \remarks �ڷ���������ֹͣ���������ǰ������ȷ�ע��ýӿ�
	 * \param[in] taskKey �����ֵ
	 * \param[in] task ����������ָ��
	 **/
	virtual void unsetProcessor(const std::string& ProcKey, ITaskProcessor* tp);

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief ����������������ӿ�
	 * \remarks ������ֱ�����map
	 **/
	//virtual void unsetAll();

	/*!
	 * \fn void Dispatch(ITaskProcessor<T>* tp)
	 * \brief �ɷ�����
	 * \remarks ͨ���˽ӿ�ʵ�������ɷ�
	 * \param[in] task ����ָ��
	 * \param[in] const std::string& assignedProcessorKey = ""
				  ָ���Ĵ����߼�ֵ��Ĭ��Ϊ�ո�����������ֱ��ƥ�䣬Ҳ��ָ��
	 **/
	 
	virtual void Dispatch(const std::string& taskKey, void* task);

private:
	/*!
	 * \fn void GetRequestProc
	 * \brief ��ȡ�������߼�ֵ
	 * \remarks
	 * \param[in] task ����ָ��
	 * \retrun  ���ݵ�ǰ�����ȡ�Ĵ����߼�ֵ
	 **/

	void ProcessGo(TaskProcInfo procInfo, void* task, void* context);

	//��д��
	std::shared_mutex mtx;
	//�д����ƣ��ڶ�������Ҫ��չΪvector<TaskProcInfo<TASK> >
	//��ʵ��һ��key ��Ӧ��� ���������δ��� 
	std::unordered_map<std::string, TaskProcInfo > m_dispatch_map;

    const unsigned DisperKey;

};
