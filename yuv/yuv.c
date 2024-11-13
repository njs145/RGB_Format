#ifndef __yuv_c
#define __yuv_c
/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <unistd.h>

#include "yuv.h"

/*---------------------------------------------------------------------------------------------------
************************************ GLOBAL VARIABLE DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------
************************************* LOCAL CONSTANT DEFINITIONS *************************************
----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------
*********************************** IMPORTED VARIABLE DECLARATIONS ***********************************
----------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------
************************************* LOCAL FUNCTION DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------
**************************************** FUNCTION DEFINITIONS ****************************************
----------------------------------------------------------------------------------------------------*/

void YUV_convert_RGB888toYUV444(__uint32_t size_yuv, char *RGB24_buff, char *yuv444_buff)
{
    __uint32_t loop;
    t_RGB24 *rgb24 = (t_RGB24 *)RGB24_buff;
    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;

    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuv444[loop].y = ( (  66 * rgb24[loop].Red + 129 * rgb24[loop].Green +  25 * rgb24[loop].Blue + 128) >> 8) +  16;
        yuv444[loop].u = ( ( -38 * rgb24[loop].Red -  74 * rgb24[loop].Green + 112 * rgb24[loop].Blue + 128) >> 8) + 128;
        yuv444[loop].v = ( ( 112 * rgb24[loop].Red -  94 * rgb24[loop].Green -  18 * rgb24[loop].Blue + 128) >> 8) + 128;
    }
}

void YUV_convert_YUV444toYUV444p(__uint32_t size_yuv, char *yuv444_buff, char *yuv444p_buff)
{
    __uint32_t loop;
    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;

    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuv444p_buff[(size_yuv / 3) * 0 + loop] = yuv444[loop].y;
        yuv444p_buff[(size_yuv / 3) * 1 + loop] = yuv444[loop].u;
        yuv444p_buff[(size_yuv / 3) * 2 + loop] = yuv444[loop].v;
    }
}

void YUV_convert_YUV444toYUYV422(__uint32_t size_yuv, char *yuv444_buff, char *yuyv422_buff)
{
    __uint32_t loop;
    __uint32_t count = 0;
    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;
    t_yuyv422 *yuyv422 = (t_yuyv422 *)yuyv422_buff;


    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuyv422[loop].y = yuv444[loop].y;

        if(loop % 2)
        {
            yuyv422[loop].uv = yuv444[count * 2].v;
            count ++;
        }
        else
        {
            yuyv422[loop].uv = yuv444[count * 2].u;
        }
    }
}

void YUV_convert_YUV444toYUV422p(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB)
{
    __uint32_t loop;
    __uint32_t count = 0; 
    __uint32_t raw = 0; 
    __uint32_t size_yuv420p = 0;

    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;
    char *yuv420p = yuv420p_buff;

    size_yuv420p = (size_yuv / 2);

    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuv420p[((size_yuv420p / 2) * 0 + loop)] = yuv444[loop].y;
        /* (loop == 짝수) && (loop == 2번째 행) */
        if(((loop % 2) == 0))
        {
            /* u(or v)의 offset + count */
            yuv420p[(size_yuv420p / 6) * 4 + count] = yuv444[loop].u;
            yuv420p[(size_yuv420p / 6) * 5 + count] = yuv444[loop].v;
            count ++;
        }
    }
}

void YUV_convert_YUV444toYUV420p(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB)
{
    __uint32_t loop;
    __uint32_t count = 0; 
    __uint32_t raw = 0; 
    __uint32_t size_yuv420p = 0;

    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;
    char *yuv420p = yuv420p_buff;

    size_yuv420p = (size_yuv / 2);

    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuv420p[((size_yuv420p / 2) * 0 + loop)] = yuv444[loop].y;
        if((loop % DIB->biWidth == 0) && (loop != 0))
        {
            raw ++;
        }
        /* (loop == 짝수) && (loop == 2번째 행) */
        if(((loop % 2) == 0) && ((raw % 2) == 0))
        {
            /* u(or v)의 offset + count */
            yuv420p[(size_yuv420p / 6) * 4 + count] = yuv444[loop].u;
            yuv420p[(size_yuv420p / 6) * 5 + count] = yuv444[loop].v;
            count ++;
        }
    }
}

void YUV_convert_YUV444toNV12(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB)
{
    __uint32_t loop;
    __uint32_t count = 0; 
    __uint32_t raw = 0; 
    __uint32_t size_yuv420p = 0;

    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;
    char *yuv420p = yuv420p_buff;

    size_yuv420p = (size_yuv / 2);

    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuv420p[((size_yuv420p / 2) * 0 + loop)] = yuv444[loop].y;
        if((loop % DIB->biWidth == 0) && (loop != 0))
        {
            raw ++;
        }
        /* (loop == 짝수) && (loop == 2번째 행) */
        if(((loop % 2) == 0) && ((raw % 2) == 0))
        {
            /* u(or v)의 offset + count */
            yuv420p[(size_yuv420p / 6) * 4 + (count * 2)] = yuv444[loop].u;
            yuv420p[(size_yuv420p / 6) * 4 + (count * 2) + 1] = yuv444[loop].v;
            count ++;
        }
    }
}

void YUV_convert_YUV444toNV21(__uint32_t size_yuv, char *yuv444_buff, char *yuv420p_buff, t_DIB *DIB)
{
    __uint32_t loop;
    __uint32_t count = 0; 
    __uint32_t raw = 0; 
    __uint32_t size_yuv420p = 0;

    t_yuv444 *yuv444 = (t_yuv444 *)yuv444_buff;
    char *yuv420p = yuv420p_buff;

    size_yuv420p = (size_yuv / 2);

    for(loop = 0; loop < (size_yuv / 3); loop ++)
    {
        yuv420p[((size_yuv420p / 2) * 0 + loop)] = yuv444[loop].y;
        if((loop % DIB->biWidth == 0) && (loop != 0))
        {
            raw ++;
        }
        /* (loop == 짝수) && (loop == 2번째 행) */
        if(((loop % 2) == 0) && ((raw % 2) == 0))
        {
            /* u(or v)의 offset + count */
            yuv420p[(size_yuv420p / 6) * 4 + (count * 2)] = yuv444[loop].v;
            yuv420p[(size_yuv420p / 6) * 4 + (count * 2) + 1] = yuv444[loop].u;
            count ++;
        }
    }
}
#endif