#pragma once
#include "../interface/iServiceCentre.hh"
#include "../interface/iService.hh"

class BaseService:public IService
{
public:
    BaseService(const char* serviceName) :service_name_(serviceName), p_service_centre_(nullptr) {};
    virtual ~BaseService() = default;

    //IService½Ó¿Ú
    virtual const std::string GetName() const
    {
        return service_name_;
    }
    virtual bool Load(IServiceCentre* p_owner)
    {
        if(!p_owner) return false;
        
        p_service_centre_ = p_owner;

        path_ = p_service_centre_->get_exe_path_();

        std::cout << "Service: " << service_name_ << " Loaded." << std::endl;
        
        return true;
    }
    virtual bool Initialize() { return true; };

    virtual bool Start() { return true; };

    virtual bool Stop() { return true; };

    virtual void Finalize(){};
    
    virtual void Unload()
    {
        p_service_centre_ = NULL;
        std::cout << "Service: " << service_name_ << " Unloaded." << std::endl;
    }
    
protected:
    IServiceCentre* p_service_centre_; 

    const std::string service_name_;
    const char* path_;
}; 
