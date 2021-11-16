#pragma once

#include "rest_utils.hh"

#include <stdio.h>
#include <string.h>


/*!
 * \fn GetJsonMsg
 * \brief ��ȡ����JSON�ַ���
 * \remarks ֱ�ӻ�ȡ��Ӧcode�����������ַ���ָ���ַ���������´����ʺϿ���Ӧ��
 * \retrun  �ַ�������ָ��
 * \param[in] return_code JSON�������code��ֵ
 * \param[out] len ��ȡ���ַ�������
**/
const char* RestUtils::GetJsonMsg(unsigned return_code, size_t& len)
{
    len = GetRestReturnFullStringLen(return_code);
    return  GetRestReturnFullString(return_code);
}

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
        //ʹ��CODE��Ӧ��Ĭ��MSG��û����Ϊ��
        if (return_msg == NULL) {
            return_msg = GetRestReturnMsgString(return_code);
        }
   
        if (return_msg != "") {
            memcpy(buf + cursor, kRestReturnMsg, ConstexprStrLength(kRestReturnMsg));
            cursor += ConstexprStrLength(kRestReturnMsg);

            size_t msg_len = strlen(return_msg);
            //�ռ䲻�㣬�������error��Ϣ
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
        //δ�����Զ���msg��û��Ĭ�ϵ�msgֱ���˳�
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
 * \brief ����JSON�ַ�����Ĭ��msg��������error��
 * \remarks ���ݷ�����ʹ��������Զ�����JSON�ַ�����������������ַ���Ϊ�գ������������ֶ�
 * \retrun  ���ɵ��ַ������ȣ���������β��0
 * \param[out] buf �����ַ�����ָ���ַ
 * \param[in] buf_size buf�Ĵ�С����Ҫ���ٱ�֤�����ɻ������ַ������������ʱ����ݴ���ֵ���нضϴ���
 * \param[in] return_code JSON�������code��ֵ
 **/
size_t RestUtils::GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code)
{
    return GenerateJsonMsg(buf, buf_size, return_code, NULL, NULL);
}

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
size_t RestUtils::GenerateJsonMsg(char* buf, size_t buf_size, unsigned return_code, const char* return_error)
{
    return GenerateJsonMsg(buf, buf_size, return_code, NULL, return_error);
}