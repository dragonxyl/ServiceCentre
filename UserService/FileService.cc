#pragma once
#include"DefaultService.hh"
#include "../misc/wf_http_utils.hh"
#include "../misc/common_tools.hh"
#include "../3rdParty/njson/optional_json.hh"

#include <chrono>
#include <ctime>
#include <memory>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include "FileService.hh"

namespace user_service {
using nlohmann::json;


FileService::FileService(const char* serviceName) : HttpTaskProcService(serviceName)
{
    image_path_ = "";

    p_404_image_= NULL;
    file_size_  = 0;
}

bool FileService::Initialize()
{
    SetFunctors();

    image_path_ = std::string(path_) + RESULT_DIR;

    std::string image_404_path = image_path_ + "404.jpg";

    std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(image_404_path.c_str(), "rb"), &fclose);

    if (fp)
    {
        fseek(fp.get(), 0, SEEK_END);
        file_size_ = ftell(fp.get());
        fseek(fp.get(), 0, SEEK_SET);

        p_404_image_ = new char[file_size_];
        fread(const_cast<char*>(p_404_image_), sizeof(char), file_size_, fp.get());
    }

    return true;
}

void FileService::Finalize()
{
    if (p_404_image_ == nullptr)
    {
        delete p_404_image_;
        p_404_image_ = NULL;
    }
}

void FileService::SetFunctors()
{
    ADDKEYFUNC("", GetImages,HttpMethodGet);
}


void FileService::GetImages(WFHttpTask* task)
{
    auto* req = task->get_req();
    auto* resp = task->get_resp();

    std::string req_image = req->get_request_uri()+1;
    size_t first_slash = req_image.find_first_of("/");
    bool error_flag = true;

    if (first_slash != std::string::npos)
    {
        req_image = image_path_ + req_image.substr(first_slash+1);

        std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(req_image.c_str(), "rb"), &fclose);

        if (fp)
        {
            fseek(fp.get(), 0, SEEK_END);
            size_t file_size = ftell(fp.get());
            fseek(fp.get(), 0, SEEK_SET);

            char* p = new char[file_size];

            fread(p, sizeof(char), file_size, fp.get());

            resp->set_header_pair("Content-Type", "image/jpeg");
            resp->append_output_body_nocopy(p, file_size);

            task->set_callback([p](WFHttpTask*) {delete p; });

            error_flag = false;
        }
    }
    else
    {
        error_flag = true;
    }

    if (error_flag)
    {
        protocol::HttpUtil::set_response_status(resp, HttpStatusNotFound);
        resp->set_header_pair("Content-Type", "image/jpeg");
        resp->append_output_body_nocopy(p_404_image_, file_size_);

       // wf_http_utils::ProcessRestFailure(resp, kRestStatusInvalidApi);
    }
}

}
