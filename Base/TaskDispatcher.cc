#pragma once
#include "TaskDispatcher.hh"
#include "workflow/WFTaskFactory.h"
#include "workflow/workflow.h"

TaskDispatcher::~TaskDispatcher()
{
    std::unique_lock<std::shared_mutex> lck(mtx);
    m_dispatch_map.clear();
}


void TaskDispatcher::setProcessor(const std::string& ProcKey, ITaskProcessor* tp, ProcessType pt)
{
    //注意动态加载可能发生在不同线程中 加锁，防止在此时被卸载
    std::unique_lock<std::shared_mutex> lck(mtx);
    const auto it = m_dispatch_map.find(ProcKey);

    if(it != m_dispatch_map.end())
    {
        DBG("taskKey: %s collide", ProcKey.c_str());
    }
    else
    {
        m_dispatch_map.emplace(ProcKey, TaskProcInfo(tp,pt));
    }
    
}

void TaskDispatcher::unsetProcessor(const std::string& ProcKey, ITaskProcessor* tp)
{
    //注意卸载可能发生在不同线程加锁，防止在此时被卸载
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

    if(taskKey.empty())
    {
        DBG("Key info is empty");
    }

    //注意卸载可能发生在不同线程加锁，防止在此时被卸载
    {
        std::shared_lock<std::shared_mutex> lck(mtx);
        auto it = m_dispatch_map.find(taskKey);
        if(it!= m_dispatch_map.end()) 
        {
            tempTpi = it->second;
        }
        else
        {
            DBG("cannot find Processor for apply with key:%s", taskKey.c_str());
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
        
        SeriesWork *series = series_of((SubTask*)task);
        *series << counter;
        goTask->start();
    }
    else
    {
        //PROCESS_SCRIPT
    }
}

void TaskDispatcher::ProcessGo(TaskProcInfo procInfo, void* task, void* context)
{
    procInfo.TaskProcessorPointer->ProcessTask(task);
    static_cast<WFCounterTask*>(context)->count();
}

