#pragma once
    
#include <stdio.h>
#include <functional>
#include <type_traits>
#include <memory>

#include <string>
#include <iostream>

#include <vector>
#include <map>
#include <set>
#include <unordered_map>

#define _STR(x) #x
#define STR(x)  _STR(x)

#define DECL_HELPER(X) std::decay<decltype(*X)>::type //通过类指针获取类名
#define BINDCLASSFUNC1(cls,f) std::bind(&cls::f, this, std::placeholders::_1)

#define BINDMEMFUNC1(f) BINDCLASSFUNC1(DECL_HELPER(this),f)//在类成员函数中绑定当前类的非静态成员函数

#define NOTE    /* 需要注意的代码  */
#define TODO    /* 尚未实现的接口、类、算法等  */
#define FOR_DBG /* 标记为调试方便而临时增加的代码  */
#define IN      /* 输入参数  */
#define OUT     /* 输出参数  */

#define IO_Delay_us(x)  {for(unsigned z = 0; z < 31*x; z++);}
#define IO_Delay_ms(x)  {for(unsigned z = 0; z < 30757*x; z++);}
#define IO_Delay_s(x)   {for(unsigned z = 0; z < 29615831*x; z++);}

#ifdef _DEBUG
#define DBG(...) \
do{ \
    fprintf(stdout, "[DEBUG] %s %s()(L%d): ", \
            __FILE__, __FUNCTION__, __LINE__); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
} while(0)
#else
#define DBG(...)
#endif

//无递归的在编译期间求得字符串常量的长度
template< size_t N >
constexpr size_t ConstexprStrLength(const char(&)[N])
{
    return N - 1;
}

#ifdef _WIN32
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#endif// _WIN32


#define COMMU_CONF_FILE "config/commu_conf.js"
#define DNS_CONF_FILE "config/DNS_conf.js"
#define RESULT_DIR "results/"