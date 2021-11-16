#pragma once

#include <stdint.h>
#include <sstream>

/*!
    * \struct Version
    * \brief 版本描述
    * \remarks 版本描述转换为字符串表示为 Major.Minor.Revision。
    * 例如主版本号为2，次版本号为5，修订为1023，则版本字符串描述为"2.5.1023"
    */
struct Version 
{
    unsigned short Major;        // 主版本号
    unsigned short Minor;        // 次版本号
    unsigned int   Revision;    // 修订号

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

