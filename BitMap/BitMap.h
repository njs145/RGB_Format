#include "stdlib.h"

typedef struct{
    __uint32_t bfSize;
    union {
        __uint32_t t_BFH_Field;
        struct
        {
            __uint32_t bfReserved1 :16;
            __uint32_t bfReserved2 :16;
        }t_BFH_Bit;
    }t_BFH_16Bit;
    __uint32_t bfOffBits;
}t_BFH;

typedef struct{
    __uint32_t biSize; 
    __uint32_t biWidth; 
    __uint32_t biHeight;
     union 
     {
        __uint32_t t_DIB_Field;
        struct 
        {
            __uint32_t biPlanes :16;
            __uint32_t biBitCount :16;
        }t_DIB_Bit;
     }t_DIB_16Bit;

    __uint32_t biCompression; 
    __uint32_t biSizeImage; 
    __uint32_t biXPelsPerMeter; 
    __uint32_t biYPelsPerMeter; 
    __uint32_t biClrUsed; 
    __uint32_t biClrImportant;
}t_DIB;

typedef struct{
    __uint8_t Blue;
    __uint8_t Green;
    __uint8_t Red;
    __uint8_t Alpha;
}t_RGBA;

typedef struct{
    __uint8_t Blue;
    __uint8_t Green;
    __uint8_t Red;
}t_RGB24;

extern void BitMap_Extract_RGB_Data(const char *__restrict__ BitMap_filename, const char *__restrict__ RGBA_output_filename, const char *__restrict__ RGB24_output_filename);