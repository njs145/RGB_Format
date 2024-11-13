#ifndef __yuv_h
#define __yuv_h
#include <stdlib.h>
#include <stdio.h>

#include "../BitMap/BitMap.h"

typedef struct{
    __uint8_t y;
    __uint8_t u;
    __uint8_t v;
}t_yuv444;

typedef struct{
    __uint8_t y;
    __uint8_t uv;
}t_yuyv422;

extern void YUV_convert_RGB888toYUV444(__uint32_t size_yuv, char *RGB24_buff, char *yuv444_buff);
extern void YUV_convert_YUV444toYUV444p(__uint32_t size_yuv, char *yuv444_buff, char *yuv444p_buff);
extern void YUV_convert_YUV444toYUYV422(__uint32_t size_yuv, char *yuv444_buff, char *yuyv422_buff);
extern void YUV_convert_YUV444toYUYV422p(__uint32_t size_yuv, char *yuv444_buff, char *yuyv422p_buff, t_DIB *DIB);
extern void YUV_convert_YUV444toYUV422p(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB);
extern void YUV_convert_YUV444toYUV420p(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB);
extern void YUV_convert_YUV444toNV12(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB);
extern void YUV_convert_YUV444toNV21(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB);
#endif