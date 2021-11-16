#pragma once
#include "../Base/CommTaskProcService.hh"

#include <mutex>
#include <atomic>
#define DEFINE_PREFIXED_FUNC_DEFAULTSERV(NAME) DEFINE_PREFIXED_FUNC_HTTP(DefaultService,NAME)

namespace user_service {
/*!
    * \class DefaultService
    * \brief Ĭ��uri������ 
    * \remarks ���http����http://xxx.xxx.xxx.xxx:xx/yyy����yyyģ�鲻���ڵ�����²Ż���д���
    */
class DefaultService:public HttpTaskProcService
{

    // using std::function< void(TaskType* )> = func;
public:
    DefaultService(const char* serviceName) : HttpTaskProcService(serviceName) {};
    virtual ~DefaultService() = default;


    //IService�ӿ�
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
    * \brief ͬ��ͼƬ�洢
    * \param[in] ͼƬbase64����
    * \return ͼƬ��url��ַ
    */
    std::string SaveImagesSync(const std::string& base64_image_string);

    static std::mutex mutex1_;
    static std::atomic<unsigned char> image_index;
}; 

}
