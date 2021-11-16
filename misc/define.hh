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

#define DECL_HELPER(X) std::decay<decltype(*X)>::type //ͨ����ָ���ȡ����
#define BINDCLASSFUNC1(cls,f) std::bind(&cls::f, this, std::placeholders::_1)

#define BINDMEMFUNC1(f) BINDCLASSFUNC1(DECL_HELPER(this),f)//�����Ա�����а󶨵�ǰ��ķǾ�̬��Ա����

#define NOTE    /* ��Ҫע��Ĵ���  */
#define TODO    /* ��δʵ�ֵĽӿڡ��ࡢ�㷨��  */
#define FOR_DBG /* ���Ϊ���Է������ʱ���ӵĴ���  */
#define IN      /* �������  */
#define OUT     /* �������  */

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

//�޵ݹ���ڱ����ڼ�����ַ��������ĳ���
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