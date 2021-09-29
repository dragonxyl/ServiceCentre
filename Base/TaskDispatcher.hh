#pragma once
#include "../interface/iTaskDispatcher.hh"
#include <shared_mutex>
#include <unordered_map>

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
 * </pre>
 *
 **/

template <typename TASK>
class TaskDispatcher : public ITaskDispatcher<TASK>
{
public:

	struct TaskProcInfo
	{
		//std::string taskId;
		ProcessType type;
		ITaskProcessor<TASK>* TaskProcessorPointer;
		TaskProcInfo() = default;
		TaskProcInfo(ITaskProcessor<TASK>* tpp, ProcessType tp) :TaskProcessorPointer(tpp), type(tp) {};
	};

	TaskDispatcher() = default;
	~TaskDispatcher();

	/*!
	 * \fn unsigned getDisperKey
	 * \brief ��ȡ�ַ�������
	 * \remarks �ڷ���������ֹͣ���������ǰ������ȷ�ע��ýӿ�
	 **/
	 //virtual const unsigned getDisperIndex();

	 /*!
	  * \fn void setProcessor
	  * \brief ���� ������ӿ�
	  * \remarks
	  * \param[in] taskKey �����ֵ
	  * \param[in] task ����������ָ��
	  * \param[in] task ������ʽ
	  **/
	virtual void setProcessor(const std::string& taskKey, ITaskProcessor<TASK>* tp, ProcessType pt = PROCESS_SYNC);

	/*!
	 * \fn void unsetProcessor(ITaskProcessor<T>* tp)
	 * \brief ������ ������ӿ�
	 * \remarks �ڷ���������ֹͣ���������ǰ������ȷ�ע��ýӿ�
	 * \param[in] taskKey �����ֵ
	 * \param[in] task ����������ָ��
	 **/
	virtual void unsetProcessor(const std::string& taskKey, ITaskProcessor<TASK>* tp);

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
	virtual void Dispatch(TASK* task);

private:
	/*!
	 * \fn void GetRequestProc
	 * \brief ��ȡ�������߼�ֵ
	 * \remarks
	 * \param[in] task ����ָ��
	 * \retrun  ���ݵ�ǰ�����ȡ�Ĵ����߼�ֵ
	 **/
	virtual std::string GetRequestProc(TASK* task) { return ""; };

	void ProcessGo(TaskProcInfo procInfo, TASK* task, void* context);

	//��д��
	std::shared_mutex mtx;
	//�д����ƣ��ڶ�������Ҫ��չΪvector<TaskProcInfo<TASK> >
	//��ʵ��һ��key ��Ӧ��� ���������δ��� 
	std::unordered_map<std::string, TaskProcInfo > m_dispatch_map;

};


template <typename TASK>
TaskDispatcher<TASK>::~TaskDispatcher()
{
    std::unique_lock<std::shared_mutex> lck(mtx);
    m_dispatch_map.clear();
}

template <typename TASK>
void TaskDispatcher<TASK>::setProcessor(const std::string& taskKey, ITaskProcessor<TASK>* tp, ProcessType pt)
{
    //ע�⶯̬���ؿ��ܷ����ڲ�ͬ�߳��� ��������ֹ�ڴ�ʱ��ж��
    std::unique_lock<std::shared_mutex> lck(mtx);
    const auto it = m_dispatch_map.find(taskKey);

    if(it != m_dispatch_map.end())
    {
        DBG("taskKey: %s collide",taskKey.c_str());
    }
    else
    {
        m_dispatch_map.emplace(taskKey, TaskProcInfo(tp,pt));
    }
    
}

template <typename TASK>
void TaskDispatcher<TASK>::unsetProcessor(const std::string& taskKey, ITaskProcessor<TASK>* tp)
{
    //ע��ж�ؿ��ܷ����ڲ�ͬ�̼߳�������ֹ�ڴ�ʱ��ж��
    std::unique_lock<std::shared_mutex> lck(mtx);
    auto it = m_dispatch_map.find(taskKey);
    if(it != m_dispatch_map.end())
    {
        m_dispatch_map.erase(it);
    }

}

template <typename TASK>
void TaskDispatcher<TASK>::Dispatch(TASK* task)
{
    TaskProcInfo tempTpi;
    std::string key = GetRequestProc(task);
    
    if(key.empty())
    {
        DBG("Key info is empty");
    }

    //ע��ж�ؿ��ܷ����ڲ�ͬ�̼߳�������ֹ�ڴ�ʱ��ж��
    {
        std::shared_lock<std::shared_mutex> lck(mtx);
        auto it = m_dispatch_map.find(key);
        if(it!= m_dispatch_map.end()) 
        {
            tempTpi.TaskProcessorPointer = it->second.TaskProcessorPointer;
            tempTpi.type                 = it->second.type;
        }
        else
        {
            DBG("cannot find Processor for apply with key:%s",key.c_str());
            return;
        }
    }

    if(tempTpi.type == PROCESS_SYNC)
    {
        tempTpi.TaskProcessorPointer->ProcessTask(task);
    }
    else if(tempTpi.type == PROCESS_ASYNC)
    {
        WFCounterTask *counter =  WFTaskFactory::create_counter_task(1, nullptr);
        WFGoTask *goTask = WFTaskFactory::create_go_task("go", &TaskDispatcher::ProcessGo, this,tempTpi, task,counter);
        
        SeriesWork *series = series_of(task);
        *series << counter;
        goTask->start();
    }
    else
    {
        //PROCESS_SCRIPT
    }
}

template <typename TASK>
void TaskDispatcher<TASK>::ProcessGo(TaskProcInfo procInfo, TASK* task, void* context)
{
    procInfo.TaskProcessorPointer->ProcessTask(task);
    static_cast<WFCounterTask*>(context)->count();
}


using HttpTaskDispatcher = TaskDispatcher<WFHttpTask>;

/*!
 * \fn void GetRequestProc
 * \brief http�����ȡ�������߼�ֵ
 * \remarks /algorithm �򷵻�algorithm
 *          /task/abc  �򷵻�task
 * \param[in] task ����ָ��
 * \retrun  ���ݵ�ǰ�����ȡ�Ĵ����߼�ֵ
 **/
template <>
std::string HttpTaskDispatcher::GetRequestProc(WFHttpTask* task)
{
    const std::string reqFunc = task->get_req()->get_request_uri();

    if("/" == reqFunc) return "";

    std::string str = reqFunc.substr(1);
    
    std::size_t found = str.find_first_of("/");
    
    if (found!=std::string::npos)
    {
        return str.substr(0,found);
    }
    else
    {
        return std::move(str);
    }
}



