#pragma once


#include "../Base/CommTaskProcService.hh"

namespace user_service {

class FileService:public HttpTaskProcService
{

public:
    FileService(const char* serviceName);
    virtual ~FileService() = default;


    //IService�ӿ�
    virtual bool Initialize();

    //virtual bool Start();

    //virtual bool Stop();

    virtual void Finalize();

protected:
    virtual void SetFunctors() override;

    //��ʹ�ø�Ŀ¼����
    virtual std::string GetRequestFunc(WFHttpTask* task) override
    {
        return "";
    }

private:
    void GetImages(WFHttpTask* task);


    std::string image_path_;

    const char* p_404_image_;
    size_t      file_size_;
}; 

}
