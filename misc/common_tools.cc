#ifndef __COMMON_TOOLS_CPP__
#define __COMMON_TOOLS_CPP__


#include "common_tools.hh"


static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

//解码表
const char DecodeTable[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    62, // '+'
    0, 0, 0,
    63, // '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
    0, 0, 0, 0, 0, 0,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
};


std::string base64Encode(const char *bytes_to_encode, unsigned int in_len)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;

}

//std::string Base64Codec::base64Decode(const char* Data,int DataByte,int& OutByte)
std::string base64Decode(const char* data_to_decode, unsigned int in_size, unsigned int& out_size)
{
    //返回值
    std::string strDecode;
    int nValue;
    int i= 0;
    while (i < in_size)
    {
        if (*data_to_decode != '\r' && *data_to_decode!='\n')
        {
            nValue = DecodeTable[*data_to_decode++] << 18;
            nValue += DecodeTable[*data_to_decode++] << 12;
            strDecode+=(nValue & 0x00FF0000) >> 16;
            out_size++;
            if (*data_to_decode != '=')
            {
                nValue += DecodeTable[*data_to_decode++] << 6;
                strDecode+=(nValue & 0x0000FF00) >> 8;
                out_size++;
                if (*data_to_decode != '=')
                {
                    nValue += DecodeTable[*data_to_decode++];
                    strDecode+=nValue & 0x000000FF;
                    out_size++;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            data_to_decode++;
            i++;
        }
    }
    return strDecode;
}


std::string get_compile_time_for_version(std::string main_ver_str)
{
    char whole_ver_str[500];
    sprintf(whole_ver_str, "%s, Released at: %s, %s", main_ver_str.c_str(), __DATE__, __TIME__);
    
    return std::string(whole_ver_str);
}


/* Swap bytes order for int32_t value */
int32_t bytes_swap_int32(int32_t int_value)
{
    int32_t int_swapped = ((int_value & 0x000000FF) << 24 |
                            (int_value & 0x0000FF00) << 8 |
                            (int_value & 0x00FF0000) >> 8 |
                            (int_value & 0xFF000000) >> 24);
                            
    return int_swapped;
}

/* Swap bytes order for float32 value */
float bytes_swap_float32_1(int value)
{
    union32IntFloat u_value;
    u_value.i_32 = value;
    
    //std::cout << "u_value.i_32 = " << u_value.i_32 << std::endl;
    //std::cout << "i_value.f_32 = " << u_value.f_32 << std::endl;
    u_value.i_32 = bytes_swap_int32(u_value.i_32);
    
    return  u_value.f_32;  //f_swap;
}

/* Swap bytes order for float32 value */
float bytes_swap_float32(float value)
{
    union32IntFloat u_value;
    u_value.f_32 = value;
    
    //std::cout << "u_value.i_32 = " << u_value.i_32 << std::endl;
    //std::cout << "i_value.f_32 = " << u_value.f_32 << std::endl;
    
    u_value.i_32 = bytes_swap_int32(u_value.i_32);
    
    return  u_value.f_32;  //f_swap;
}

#endif // !__COMMON_TOOLS_CPP__
