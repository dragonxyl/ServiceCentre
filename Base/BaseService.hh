#pragma once
#include "../interface/iServiceCentre.hh"
#include "../interface/iService.hh"

class BaseService:public IService
{
public:
    BaseService(const char* serviceName) :m_ServiceName(serviceName), m_pServiceCentre(nullptr) {};

	//IService½Ó¿Ú
	virtual const std::string GetName() const
    {
        return m_ServiceName;
    }
	virtual bool Load(IServiceCentre* pOwner)
    {
        if(!pOwner) return false;
        
        m_pServiceCentre = pOwner;
        std::cout << "Service: " << m_ServiceName << " Loaded." << std::endl;
        
        return true;
    }
    virtual bool Initialize() { return true; };

    virtual bool Start() { return true; };

    virtual bool Stop() { return true; };

	virtual void Finalize(){};
    
	virtual void Unload()
    {
        m_pServiceCentre = NULL;
        std::cout << "Service: " << m_ServiceName << " Unloaded." << std::endl;
    }
    
protected:
    IServiceCentre* m_pServiceCentre; 

    const std::string m_ServiceName;
}; 
