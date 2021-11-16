#pragma once
#include "TaskDispatcher.hh"
#include "workflow/WFTaskFactory.h"
#include "workflow/workflow.h"

TaskDispatcher::TaskDispatcher(const unsigned& key, ITaskProcessor* default_proc) :DisperKey(key)
{
    setProcessor("",default_proc);
};

TaskDispatcher::~TaskDispatcher()
{
    std::unique_lock<std::shared_mutex> lck(mtx);
    m_dispatch_map.clear();
}


void TaskDispatcher::setProcessor(const std::string& ProcKey, ITaskProcessor* tp, ProcessType pt)
{
    //ע�⶯̬���ؿ��ܷ����ڲ�ͬ�߳��� ��������ֹ�ڴ�ʱ��ж��
    std::unique_lock<std::shared_mutex> lck(mtx);
    const auto it = m_dispatch_map.find(ProcKey);

    if(it != m_dispatch_map.end())
    {
        //Ĭ�ϴ���ģ�鸲��
        if (ProcKey.empty())
        {
            m_dispatch_map[""] = TaskProcInfo(tp, pt);
        }
        else
        {
            DBG("taskKey: %s collide", ProcKey.c_str());
        }
    }
    else
    {
        m_dispatch_map.emplace(ProcKey, TaskProcInfo(tp,pt));
    }
    
}

void TaskDispatcher::unsetProcessor(const std::string& ProcKey, ITaskProcessor* tp)
{
    //ע��ж�ؿ��ܷ����ڲ�ͬ�̼߳�������ֹ�ڴ�ʱ��ж��
    std::unique_lock<std::shared_mutex> lck(mtx);
    auto it = m_dispatch_map.find(ProcKey);
    if(it != m_dispatch_map.end())
    {
        m_dispatch_map.erase(it);
    }

}

void TaskDispatcher::Dispatch(const std::string& taskKey, void* task)
{
    TaskProcInfo tempTpi;

    //ע��ж�ؿ��ܷ����ڲ�ͬ�̼߳�������ֹ�ڴ�ʱ��ж��
    {
        std::shared_lock<std::shared_mutex> lck(mtx);
        auto it = m_dispatch_map.find(taskKey);
        if(it!= m_dispatch_map.end()) 
        {
            tempTpi = it->second;
        }
        //�޷�ʶ���key���͵�Ĭ�ϴ���ģ�鴦��
        else
        {
            auto it_default = m_dispatch_map.find("");
            if (it_default != m_dispatch_map.end())
            {
                tempTpi = it_default->second;
            }
            //����ÿ�������������û��ָ����Ĭ�ϴ���ģ�飬����ָ��ͳһ��Ĭ�ϴ���ģ�飬��ʱ˵����������������
            else
            {
                DBG("cannot find Default Processor for unknown key:%s", taskKey.c_str());
                return;
            }
        }
    }

    if(tempTpi.type == PROCESS_SYNC)
    {
        tempTpi.TaskProcessorPointer->ProcessTask(task);
    }
    //�������ʵ���첽����
    else// if(tempTpi.type == PROCESS_ASYNC)
    {
        WFCounterTask *counter =  WFTaskFactory::create_counter_task(1, nullptr);
        WFGoTask *goTask = WFTaskFactory::create_go_task("go", &TaskDispatcher::ProcessGo, this,tempTpi, task,counter);
        
        SeriesWork *series = series_of((SubTask*)task);
        *series << counter;
        goTask->start();
    }
}

void TaskDispatcher::ProcessGo(TaskProcInfo procInfo, void* task, void* context)
{
    procInfo.TaskProcessorPointer->ProcessTask(task);
    static_cast<WFCounterTask*>(context)->count();
}

