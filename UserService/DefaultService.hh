#pragma once
#include "../Base/CommTaskProcService.hh"

#include <mutex>
#include <atomic>
#define DEFINE_PREFIXED_FUNC_DEFAULTSERV(NAME) DEFINE_PREFIXED_FUNC_HTTP(DefaultService,NAME)

namespace user_service {
/*!
    * \class DefaultService
    * \brief 默认uri处理函数 
    * \remarks 针对http链接http://xxx.xxx.xxx.xxx:xx/yyy，当yyy模块不存在的情况下才会进行处理
    */
class DefaultService:public HttpTaskProcService
{

    // using std::function< void(TaskType* )> = func;
public:
    DefaultService(const char* serviceName) : HttpTaskProcService(serviceName) {};
    virtual ~DefaultService() = default;


    //IService接口
//       virtual bool Initialize();

    //virtual bool Start();

    //virtual bool Stop();

    //virtual void Finalize();

protected:
    std::string GetRequestFunc(WFHttpTask* task) override;
    virtual void SetFunctors() override;


private:

    DECLARE_PREFIXED_FUNC_HTTP(result);

    /*!
    * \fn SaveImagesSync
    * \brief 同步图片存储
    * \param[in] 图片base64编码
    * \return 图片的url地址
    */
    std::string SaveImagesSync(const std::string& base64_image_string);

    static std::mutex mutex1_;
    static std::atomic<unsigned char> image_index;
}; 

}
