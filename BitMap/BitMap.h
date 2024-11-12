#include "stdlib.h"

#define TYPE_RGB565_LITTLE  0
#define TYPE_RGB565_BIG     1

#define TYPE_RGB555_LITTLE  0
#define TYPE_RGB555_BIG     1

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

extern void BitMap_Extract_RGB_Data(const char *__restrict__ BitMap_filename, char **RGB24_buf, __uint32_t *size);
extern void BitMap_convert_RGB888toRGB565(__uint32_t type, __uint32_t size_RGB24, char *RGB24_buff, char *RGB565_buff);
extern void BitMap_convert_RGB888toRGB555(__uint32_t type, __uint32_t size_RGB24, char *RGB24_buff, char *RGB555_buff);