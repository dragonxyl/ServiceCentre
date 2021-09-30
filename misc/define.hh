
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

#define DECL_HELPER(X) std::decay<decltype(*X)>::type //ͨ����ָ���ȡ����
#define BINDCLASSFUNC1(cls,f) std::bind(&cls::f, this, std::placeholders::_1)

#define BINDMEMFUNC1(f) BINDCLASSFUNC1(DECL_HELPER(this),f)//�����Ա�����а󶨵�ǰ��ķǾ�̬��Ա����

#define	NOTE    /* ��Ҫע��Ĵ���  */
#define	TODO    /* ��δʵ�ֵĽӿڡ��ࡢ�㷨��  */
#define	FOR_DBG /* ���Ϊ���Է������ʱ���ӵĴ���  */
#define	IN      /* �������  */
#define	OUT     /* �������  */

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

