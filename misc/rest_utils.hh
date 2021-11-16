#pragma once


#include <string>
#include <tuple>

#include "define.hh"
#include "meta_string.hh"


/// <summary>
/// �������ʱ�Զ�ƴ���ַ��������ɺ�
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
/// �û����壬��ʱֻ֧���������
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
    //���ش���
    kRestStatusInvalidApi,
    kRestStatusUnsupportedApiMethod,
    kRestStatusInvalidArgument,

    //�ⲿ����
    kRestStatusRemoteHostUnavailable = 10,
    kRestStatusRemoteHostReturnInvalid,      //�ظ���������
    kRestStatusRemoteHostReturnUnknown,     //�ظ��������������
    kRestStatusRemoteHostReturnFailure,     //Զ����������ʧ��(ҵ��ʧ��)
    kRestStatusExCondUnsatisfied            //�ⲿ����������
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
     * \brief ��ȡ����JSON�ַ���
     * \remarks ֱ�ӻ�ȡ��Ӧcode�����������ַ���ָ���ַ���������´����ʺϿ���Ӧ��
     * \retrun  �ַ�������ָ��
     * \param[in] return_code JSON�������code��ֵ
     * \param[out] len ��ȡ���ַ�������
    **/    
    static const char* GetJsonMsg(unsigned return_code, size_t& len);

    /*!
     * \fn GenerateJsonMsg
     * \brief ����JSON�ַ������Զ���msg���Զ���error��
     * \remarks ���ݷ�����ʹ��������Զ�����JSON�ַ�����������������ַ���Ϊ�գ������������ֶ�
     * \retrun  ���ɵ��ַ������ȣ���������β��0
     * \param[out] buf �����ַ�����ָ���ַ
     * \param[in] buf_size buf�Ĵ�С����Ҫ���ٱ�֤�����ɻ������ַ������������ʱ����ݴ���ֵ���нضϴ���
     * \param[in] return_code JSON�������code��ֵ
     * \param[in] return_msg JSON��������msg
     * \param[in] return_error JSON��������error
     **/
    static size_t GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code, const char* return_msg, const char* return_error); 

    /*!
    * \fn GenerateJsonMsg
    * \brief ����JSON�ַ�����Ĭ��msg��������error��
    * \remarks ���ݷ�����ʹ��������Զ�����JSON�ַ�����������������ַ���Ϊ�գ������������ֶ�
    * \retrun  ���ɵ��ַ������ȣ���������β��0
    * \param[out] buf �����ַ�����ָ���ַ
    * \param[in] buf_size buf�Ĵ�С����Ҫ���ٱ�֤�����ɻ������ַ������������ʱ����ݴ���ֵ���нضϴ���
    * \param[in] return_code JSON�������code��ֵ
    **/
    static size_t GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code);

    /*!
     * \fn GenerateJsonMsg
     * \brief ����JSON�ַ�����Ĭ��msg���Զ���error��
     * \remarks ���ݷ�����ʹ��������Զ�����JSON�ַ�����������������ַ���Ϊ�գ������������ֶ�
     * \retrun  ���ɵ��ַ������ȣ���������β��0
     * \param[out] buf �����ַ�����ָ���ַ
     * \param[in] buf_size buf�Ĵ�С����Ҫ���ٱ�֤�����ɻ������ַ������������ʱ����ݴ���ֵ���нضϴ���
     * \param[in] return_code JSON�������code��ֵ
     * \param[in] return_error JSON��������error
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
