#include <stdlib.h>
#include <stdio.h>

typedef struct{
    __uint8_t y;
    __uint8_t u;
    __uint8_t v;
}t_yuv444;

extern void YUV_convert_RGB888toYUV444(__uint32_t size_yuv, char *RGB24_buff, char *yuv444_buff);
extern void YUV_convert_YUV444toYUV444p(__uint32_t size_yuv, char *yuv444_buff, char *yuv444p_buff);