#pragma once

#include <stdint.h>
#include <sstream>

/*!
    * \struct Version
    * \brief �汾����
    * \remarks �汾����ת��Ϊ�ַ�����ʾΪ Major.Minor.Revision��
    * �������汾��Ϊ2���ΰ汾��Ϊ5���޶�Ϊ1023����汾�ַ�������Ϊ"2.5.1023"
    */
struct Version 
{
    unsigned short Major;        // ���汾��
    unsigned short Minor;        // �ΰ汾��
    unsigned int   Revision;    // �޶���

    Version(unsigned short major, unsigned short minor, unsigned int rev) :Major(major), Minor(minor), Revision(rev) {};
    Version(uint64_t ver)
    {
        Major = unsigned short(ver >> 48);
        Minor = unsigned short(ver >> 32);
        Revision = unsigned int(ver);
    };

    std::string ToString() const
    {
        std::stringstream ss;
        ss << Major <<"."<< Minor <<"."<< Revision;
        return ss.str();
    };

    uint64_t ToInteger() const
    {
        return (uint64_t(Major) << 48) | (uint64_t(Minor) << 32) | Revision;
    };
};

