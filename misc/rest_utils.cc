#pragma once

#include "rest_utils.hh"

#include <stdio.h>
#include <string.h>


/*!
 * \fn GetJsonMsg
 * \brief 获取常量JSON字符串
 * \remarks 直接获取对应code的完整常量字符串指针地址，不生成新串，适合快速应用
 * \retrun  字符串常量指针
 * \param[in] return_code JSON返回体的code数值
 * \param[out] len 获取的字符串长度
**/
const char* RestUtils::GetJsonMsg(unsigned return_code, size_t& len)
{
    len = GetRestReturnFullStringLen(return_code);
    return  GetRestReturnFullString(return_code);
}

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
size_t RestUtils::GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code, const char* return_msg, const char* return_error)
{
    //code
    memcpy(buf, kRestReturnCode, ConstexprStrLength(kRestReturnCode));
    size_t cursor = ConstexprStrLength(kRestReturnCode);

    char num_buf[8];
    snprintf(num_buf, 8, "%u", return_code);
    size_t code_len = strlen(num_buf);

    memcpy(buf + cursor, num_buf, code_len);
    cursor += code_len;

    do{
        //msg
        //使用CODE对应的默认MSG，没有则为空
        if (return_msg == NULL) {
            return_msg = GetRestReturnMsgString(return_code);
        }
   
        if (return_msg != "") {
            memcpy(buf + cursor, kRestReturnMsg, ConstexprStrLength(kRestReturnMsg));
            cursor += ConstexprStrLength(kRestReturnMsg);

            size_t msg_len = strlen(return_msg);
            //空间不足，则不再添加error信息
            if (msg_len + cursor + ConstexprStrLength(kRestReturnEnd) + 1 > buf_size) {
                //1 for '0'
                msg_len = (buf_size - cursor - ConstexprStrLength(kRestReturnEnd) - 1);
                memcpy(buf + cursor, return_msg, msg_len);
                cursor += msg_len;
                break;
            }
            else {
                memcpy(buf + cursor, return_msg, msg_len);
                cursor += msg_len;
            }
        }
        //未传入自定义msg且没有默认的msg直接退出
        else
            break;

        //error
        if (return_error != NULL) {
            memcpy(buf + cursor, kRestReturnError, ConstexprStrLength(kRestReturnError));
            cursor += ConstexprStrLength(kRestReturnError);

            size_t error_len = strlen(return_error);
            if (error_len + cursor + ConstexprStrLength(kRestReturnEnd) + 1 > buf_size) {
                error_len = (buf_size - cursor - ConstexprStrLength(kRestReturnEnd) - 1);
            }

            memcpy(buf + cursor, return_error, error_len);
            cursor += error_len;
        }
    }while (0);

    //end
    memcpy(buf + cursor, kRestReturnEnd, ConstexprStrLength(kRestReturnEnd));
    cursor += ConstexprStrLength(kRestReturnEnd);

    buf[cursor] = 0;

    return cursor;
}

/*!
 * \fn GenerateJsonMsg
 * \brief 生成JSON字符串（默认msg，不包含error）
 * \remarks 根据返回码和错误详情自动生成JSON字符串，如果错误详情字符串为空，则不生产错误字段
 * \retrun  生成的字符串长度，不包括结尾的0
 * \param[out] buf 接收字符串的指针地址
 * \param[in] buf_size buf的大小，需要至少保证能容纳基本的字符串结果，超出时会根据此数值进行截断处理
 * \param[in] return_code JSON返回体的code数值
 **/
size_t RestUtils::GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code)
{
    return GenerateJsonMsg(buf, buf_size, return_code, NULL, NULL);
}

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
size_t RestUtils::GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code, const char* return_error)
{
    return GenerateJsonMsg(buf, buf_size, return_code, NULL, return_error);
}