#pragma once

#include "Sender.hh"

#include <fstream>

#include "../3rdParty/njson/optional_json.hh"

#include "../misc/define.hh"

Sender::Sender(const char* file_name):conf_file_full_name_(file_name)
{
    //CreateBuiltInBoostMap
    builtInBoostMap_["ScheduleCentre"]      = ScheduleCentre;
    builtInBoostMap_["SubScheduleCentre"]   = SubScheduleCentre;
    builtInBoostMap_["AlgorithmManager"]    = AlgorithmManager;
    builtInBoostMap_["VideoAnalyser"]       = VideoAnalyser;
    builtInBoostMap_["AlgorithmGateway"]    = AlgorithmGateway;

    InitFromConfigFile();
}

std::string Sender::GetHostAddr(const std::string& hostName)
{
    std::shared_lock<std::shared_mutex> lck(mtx_);
    const auto& it = route_map_.find(hostName);
    if(it!= route_map_.end()) 
    {
        return it->second;
    }
    else
    {
        DBG("cannot find hostAddr for hostName:%s", hostName.c_str());
        return "";
    }
}

void Sender::SetHostAddr(const std::string& host_name, const std::string& host_addr)
{
    if(host_name.empty())
    {
        DBG("hostName is empty when set route_map");
        return;
    }
    
    std::string key = host_name;
    if (builtInBoostMap_.find(key) != builtInBoostMap_.end())
    {
        key = builtInBoostMap_[key];
    }

    //删除某个键值
    if(host_addr.empty())
    {
        auto it = route_map_.find(key);
        if(it!= route_map_.end()) 
        {
            std::unique_lock<std::shared_mutex> lck(mtx_);
            route_map_.erase(it);
        }
        else
        {
            DBG("host_addr is empty when set routeMap");
        }

        return;
    }

    //注意动态加载可能发生在不同线程中 加锁，防止在此时被卸载
    std::unique_lock<std::shared_mutex> lck(mtx_);
    route_map_[key] = host_addr;//相同则覆盖
}

void Sender::InitFromDefaultConfig()
{
    route_map_.clear();

    std::string hostAddr = "http://127.0.0.1:80";

    SetHostAddr(ScheduleCentre, "http://127.0.0.1:20866");
    SetHostAddr(SubScheduleCentre, "http://127.0.0.1:20855");
    SetHostAddr(AlgorithmManager, "http://192.168.20.187:30001");
    SetHostAddr(VideoAnalyser, "http://192.168.20.166:30001");
    SetHostAddr(AlgorithmGateway, hostAddr);
}
#include <typeinfo>
void Sender::InitFromConfigFile()
{
    using nlohmann::json;

    bool succ_flag = false;
    std::ifstream ifs(conf_file_full_name_);
    if (ifs)
    {
        json j;
        ifs >> j;
        ifs.close();

        try
        {
            for (auto& it : j["hosts"])
            {
                std::string key = it["host_name"].get<std::string>();
                SetHostAddr(key, it["url"].get<std::string>());
                succ_flag = true;
            }
        }
        catch (const std::exception& e)
        {
            printf("File:%s parse error:%s.\n", conf_file_full_name_.c_str(), e.what());
            succ_flag = false;
        }
    }

    if (!succ_flag)
    {
        InitFromDefaultConfig();
    }
}


