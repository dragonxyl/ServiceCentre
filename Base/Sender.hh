#pragma once
#include <unordered_map>
#include <shared_mutex>

#include "../interface/iSender.hh"

class Sender:public ISender
{
public:
    Sender(const char* pathName);

    virtual std::string GetHostAddr(const std::string& hostName);

    virtual void SetHostAddr(const std::string& hostName, const std::string& hostAddr);

private:
    void InitFromDefaultConfig();
    void InitFromConfigFile();

private:
    std::unordered_map<std::string,std::string> route_map_;
    
    //¶ÁÐ´Ëø
    std::shared_mutex mtx_;

    const std::string conf_file_full_name_;
    std::unordered_map<std::string, std::string> builtInBoostMap_;

};