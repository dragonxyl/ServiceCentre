#ifndef __COMMON_TOOLS_H__
#define __COMMON_TOOLS_H__

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>

union union32IntFloat
{
    int32_t i_32;
    float f_32;
};

std::string base64Encode(const char *bytes_to_encode, unsigned int in_len);

std::string base64Decode(const char* data_to_decode, unsigned int in_size, unsigned int& out_size);


std::string get_compile_time_for_version(std::string main_ver_str);

/* Swap bytes order for float32 value */
float bytes_swap_float32_1(int value);

/* Swap bytes order for float32 value */
float bytes_swap_float32(float value);


#endif 

