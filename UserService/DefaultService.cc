#pragma once
#include"DefaultService.hh"

#include <io.h>
#include <stdio.h>

#include <chrono>
#include <ctime>
#include <memory>
#include <iomanip>
#include <sstream>

#include "../3rdParty/njson/optional_json.hh"

#include "../misc/wf_http_utils.hh"
#include "../misc/common_tools.hh"

namespace user_service {
using nlohmann::json;

static int MakeFinalDirectory(const std::string& normalized_path)
{
#ifdef WIN32
    return CreateDirectory(normalized_path.c_str(), NULL) ? 0 : (GetLastError() == ERROR_ALREADY_EXISTS ? 1 : -1);
#else
    return mkdir(normalized_path.c_str(), 0775) == 0 ? 0 : (errno == EEXIST ? 1 : -1);
#endif
}

/*
bool DefaultService::Initialize()
{
    SetFunctors();

    return true;
}

bool DefaultService::Start()
{
    return true;
}
*/

/// <summary>
/// 根目录下重写为所有后缀字符串
/// </summary>
/// <param name="task"></param>
/// <returns></returns>
std::string DefaultService::GetRequestFunc(WFHttpTask* task)
{
    return std::string(FUNC_PREFIX) + (task->get_req()->get_request_uri() + 1);
}

void DefaultService::SetFunctors()
{
    ADDFUNC(PREFIXED_FUNC_NAME(result));
}

DEFINE_PREFIXED_FUNC_DEFAULTSERV(result)
//void DefaultService::_INTERFACE_result(WFHttpTask* task)
{
    auto* req = task->get_req();
    auto* resp = task->get_resp();

    const void* body;
    size_t len;
    std::shared_ptr<json> jBody;
    std::string ImageString, ImageString1;

    if (req->get_parsed_body(&body, &len))
    {
        try
        {
            jBody =  std::make_shared<json>(json::parse((char*)body));

            if (jBody->find("ResultImage") != jBody->end())
                jBody->at("ResultImage").get_to(ImageString);
            if (jBody->find("candid_image") != jBody->end())
                jBody->at("candid_image").get_to(ImageString1);
        }
        //JSON回复解析异常
        catch (const std::exception& e)
        {
            jBody.reset();
            return wf_http_utils::ProcessJsonException(resp,e);
        }
    }

    std::string image_url = SaveImagesSync(ImageString);
    std::string image_url1 = SaveImagesSync(ImageString1);

    std::string send_result_url = pSender_->GetHostAddr("BusinessLayer") + "/device/getAlarmImgUrl";
    WFHttpTask* send_result_task = WFTaskFactory::create_http_task(send_result_url, 0, 0, nullptr);
    auto* send_result_req = send_result_task->get_req();

    //Head
    send_result_req->set_method(HttpMethodPost);
    send_result_req->set_header_pair("Content-Type", "application/json");
    //Body
    json send_body;
    try
    {
        send_body["status"] = jBody->at("Status").get<int>();

        auto vec = jBody->at("ResultString").get<std::vector<std::string>>();
        if (!vec.empty())
        {
            send_body["resultString"] = *(vec.begin());
        }
        else
        {
            send_body["resultString"] = "";
        }
        
        send_body["resultImageUrl"] = std::move(image_url);
        send_result_req->append_output_body(send_body.dump());
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "%s\n", e.what());
    }


    send_result_task->start();

    //resp->append_output_body(send_body.dump());
    //返回成功
    //wf_http_utils::ProcessRestSuccess(resp);
}


std::mutex DefaultService::mutex1_;
std::atomic<unsigned char> DefaultService::image_index = 0;


/*!
* \fn SaveImagesSync
* \brief 同步图片存储
* \param[in] 图片base64编码
* \return 图片的url地址
*/
std::string DefaultService::SaveImagesSync(const std::string& base64_image_string)
{
    if (base64_image_string.empty())
    {
        return "";
    }

    unsigned out_byte_num = 0;
    unsigned char index = 0;
    static std::string static_path = std::string(path_) + std::string(RESULT_DIR);

    std::string bitstream_imgae_string = base64Decode(base64_image_string.c_str(), base64_image_string.length(), out_byte_num);
    if (bitstream_imgae_string.size() <= 0)
    {
        std::cout << "Empty temperature_map json string!" << std::endl;
        return std::string("");
    }
    else
    {
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y%m%d%H%M%S");
        std::string image_path_ = ss.str().substr(0, 8) + "/";
        image_path_.reserve(100);

        //回传地址中的图片url
        std::string url_path = "/images/" + image_path_;
        url_path.reserve(50);
        //完整路径名称
        image_path_.insert(0, static_path);

        //检查文件夹是否存在
        if (_access(image_path_.c_str(), 0) == -1)
        {
            std::lock_guard<std::mutex> lcg(mutex1_);
            if (_access(image_path_.c_str(), 0) == -1)
            {
                MakeFinalDirectory(image_path_);
            }
        }

        //生成文件名
        std::string image_name = ss.str();

        index = image_index.fetch_add(1);
        std::string s_num = std::to_string(index);
        s_num.insert(s_num.begin(), 3 - s_num.size(), '0');

        image_name.append(s_num);
        image_name.append(".jpg");

        url_path += image_name;
        image_path_ += image_name;

        std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(image_path_.c_str(), "wb+"), &fclose);
        if (fp)
        {
            fwrite(bitstream_imgae_string.c_str(), sizeof(char), bitstream_imgae_string.length(), fp.get());
        }

        return url_path;
    }
}



}
