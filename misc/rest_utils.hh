#pragma once


#include <string>
#include <tuple>

#include "define.hh"
#include "meta_string.hh"


/// <summary>
/// 定义编译时自动拼接字符串的生成宏
/// </summary>
#define RETURN_CHARARR_NAME(x) kRestReturn##x
#define RETURN_CHARARR_NAME_(x,y) RETURN_CHARARR_NAME(x),

#define DEF_RETURN_CHARARR(x,y) static constexpr char RETURN_CHARARR_NAME(x)[] = y;

#define DEF_RETURN_FULL_METASTR(x,y) (meta_string::str(RETURN_CHARARR_NAME(Code)) +  meta_string::str(#x) + meta_string::str(RETURN_CHARARR_NAME(Msg))\
                            + meta_string::str(RETURN_CHARARR_NAME(x)) + meta_string::str(RETURN_CHARARR_NAME(End))),

#define TUP_TO_CHARP(x,y) std::get<x>(kFullMetaTup).c_str,
#define TUP_TO_LEN(x,y) std::get<x>(kFullMetaTup).length,

#define DEF_RETURN_CHARARRS(ENUMS) \
    ENUMS(DEF_RETURN_CHARARR)                  \

#define DEF_RETURN_CHARP_ARR(ENUMS) \
    static const char* kRestReturnMsgString[] = {   \
    ENUMS(RETURN_CHARARR_NAME_)               \
    0}

#define DEF_RETURN_FULL_METASTR_TUPS(ENUMS) \
    static constexpr auto kFullMetaTup = std::make_tuple(   \
    ENUMS(DEF_RETURN_FULL_METASTR)                             \
    0)                                                  \

#define DEF_RETURN_FULL_CHARP_ARR(ENUMS) \
    static const char* kFullRestReturnString[] = {   \
    ENUMS(TUP_TO_CHARP)                             \
    0}

#define DEF_RETURN_FULL_LEN_ARR(ENUMS) \
    static const size_t kFullRestReturnStringLen[] = {   \
    ENUMS(TUP_TO_LEN)                             \
    0}

#define DEF_RETURN_SERIES(ENUM)\
DEF_RETURN_CHARARRS(ENUM)\
DEF_RETURN_CHARP_ARR(ENUM);\
DEF_RETURN_FULL_METASTR_TUPS(ENUM);\
DEF_RETURN_FULL_CHARP_ARR(ENUM);\
DEF_RETURN_FULL_LEN_ARR(ENUM)

#define FIXED_STRING(F)         \
    F(Code, R"({"code":")")     \
    F(Msg, R"(","msg":")")      \
    F(Error, R"(","error":")")  \
    F(End, "\"}")

DEF_RETURN_CHARARRS(FIXED_STRING);

/// <summary>
/// 用户定义，暂时只支持连续编号
/// </summary>
#define RETURN_STRING(F)                \
    F(0, "Success")                     \
    F(1, "Invalid API")            \
    F(2, "Unsupported API Method")      \
    F(3, "Invalid Argument")     \
    F(4, "U")    \
    F(5, "U")                     \
    F(6, "U")            \
    F(7, "U")      \
    F(8, "U")     \
    F(9, "U")    \
    F(10,"Remote Host Unavailable")    \
    F(11,"Remote Host Return Invalid Argument")           \
    F(12,"Remote Host Return Unknown")    \
    F(13,"Remote Host Return Failure")   \
    F(14,"External Condition Unsatisfied")     \
    F(15,"U")                    \
    F(16,"U")           \
    F(17,"U")     \
    F(18,"U")    \
    F(19,"U")

DEF_RETURN_SERIES(RETURN_STRING);

enum EnumRestStatusCode
{
    kRestStatusSuccess = 0,
    //本地错误
    kRestStatusInvalidApi,
    kRestStatusUnsupportedApiMethod,
    kRestStatusInvalidArgument,

    //外部错误
    kRestStatusRemoteHostUnavailable = 10,
    kRestStatusRemoteHostReturnInvalid,      //回复解析错误
    kRestStatusRemoteHostReturnUnknown,     //回复解析结果无意义
    kRestStatusRemoteHostReturnFailure,     //远程主机返回失败(业务失败)
    kRestStatusExCondUnsatisfied            //外部条件不满足
};

struct JsonReturnStruct
{
    std::string code;
    std::string msg;
    std::string error; //optional
};

class RestUtils
{
public:
    /*!
     * \fn GetJsonMsg
     * \brief 获取常量JSON字符串
     * \remarks 直接获取对应code的完整常量字符串指针地址，不生成新串，适合快速应用
     * \retrun  字符串常量指针
     * \param[in] return_code JSON返回体的code数值
     * \param[out] len 获取的字符串长度
    **/    
    static const char* GetJsonMsg(unsigned return_code, size_t& len);

    /*!
     * \fn GenerateJsonMsg
     * \brief 生成JSON字符串（自定义msg，自定义error）
     * \remarks 根据返回码和错误详情自动生成JSON字符串，如果错误详情字符串为空，则不生产错误字段
     * \retrun  生成的字符串长度，不包括结尾的0
     * \param[out] buf 接收字符串的指针地址
     * \param[in] buf_size buf的大小，需要至少保证能容纳基本的字符串结果，超出时会根据此数值进行截断处理
     * \param[in] return_code JSON返回体的code数值
     * \param[in] return_msg JSON返回体中msg
     * \param[in] return_error JSON返回体中error
     **/
    static size_t GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code, const char* return_msg, const char* return_error); 

    /*!
    * \fn GenerateJsonMsg
    * \brief 生成JSON字符串（默认msg，不包含error）
    * \remarks 根据返回码和错误详情自动生成JSON字符串，如果错误详情字符串为空，则不生产错误字段
    * \retrun  生成的字符串长度，不包括结尾的0
    * \param[out] buf 接收字符串的指针地址
    * \param[in] buf_size buf的大小，需要至少保证能容纳基本的字符串结果，超出时会根据此数值进行截断处理
    * \param[in] return_code JSON返回体的code数值
    **/
    static size_t GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code);

    /*!
     * \fn GenerateJsonMsg
     * \brief 生成JSON字符串（默认msg，自定义error）
     * \remarks 根据返回码和错误详情自动生成JSON字符串，如果错误详情字符串为空，则不生产错误字段
     * \retrun  生成的字符串长度，不包括结尾的0
     * \param[out] buf 接收字符串的指针地址
     * \param[in] buf_size buf的大小，需要至少保证能容纳基本的字符串结果，超出时会根据此数值进行截断处理
     * \param[in] return_code JSON返回体的code数值
     * \param[in] return_error JSON返回体中error
    **/
    static size_t GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code, const char* return_error);

private:
    static const char* GetRestReturnMsgString(unsigned no)
    {
        if (no >= sizeof(kRestReturnMsgString)) {
            return "";
        }
        else {
            return kRestReturnMsgString[no];
        }
    }

    static const char* GetRestReturnFullString(unsigned no)
    {
        if (no >= sizeof(kFullRestReturnString)) {
            return "";
        }
        else {
            return kFullRestReturnString[no];
        }
    }

    static const size_t GetRestReturnFullStringLen(unsigned no)
    {
        if (no >= sizeof(kFullRestReturnStringLen)) {
            return 0;
        }
        else {
            return kFullRestReturnStringLen[no];
        }
    }

};
