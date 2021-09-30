
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

#define DECL_HELPER(X) std::decay<decltype(*X)>::type //通过类指针获取类名
#define BINDCLASSFUNC1(cls,f) std::bind(&cls::f, this, std::placeholders::_1)

#define BINDMEMFUNC1(f) BINDCLASSFUNC1(DECL_HELPER(this),f)//在类成员函数中绑定当前类的非静态成员函数

#define	NOTE    /* 需要注意的代码  */
#define	TODO    /* 尚未实现的接口、类、算法等  */
#define	FOR_DBG /* 标记为调试方便而临时增加的代码  */
#define	IN      /* 输入参数  */
#define	OUT     /* 输出参数  */

#define IO_Delay_us(x)	{for(unsigned z = 0; z < 31*x; z++);}
#define IO_Delay_ms(x)	{for(unsigned z = 0; z < 30757*x; z++);}
#define IO_Delay_s(x)	{for(unsigned z = 0; z < 29615831*x; z++);}

#define DEBUG

#ifdef DEBUG
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

