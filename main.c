/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "BitMap/BitMap.h"
#include "yuv/yuv.h"
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

int main(void)
{
    int fp_test_RGB565, fp_test_RGB555, fp_test_yuv444, fp_test_yuv444p, fp_test_yuyv422, fp_test_yuv422p, fp_test_yuv420p, fp_test_nv12, fp_test_nv21;
    char *buff_RGB24, *buff_RGB565, *buff_RGB555, *buff_yuv444, *buff_yuv444p, *buff_yuyv422, *buff_yuv422p, *buff_yuv420p, *buff_nv12, *buff_nv21 = NULL;
    __uint32_t size_RGB24, size_RGB565, size_RGB555, size_yuv444, size_yuv444p, size_yuyv422, size_yuv422p, size_yuv420p, size_nv12, size_nv21;
    t_DIB DIB;

    fp_test_RGB565 = open("Build/output/RGB565.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_RGB555 = open("Build/output/RGB555.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuv444 = open("Build/output/yuv444.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuv444p = open("Build/output/yuv444p.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuyv422 = open("Build/output/yuyv422.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuv422p = open("Build/output/yuv422p.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuv420p = open("Build/output/yuv420p.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_nv12 = open("Build/output/nv12.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_nv21 = open("Build/output/nv21.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    /* BitMap에서 RGB888 추출 */
    BitMap_Extract_RGB_Data("SampleImage/Bitmap_picture_DOG.bmp", &buff_RGB24, &size_RGB24, &DIB);

    /* RGB565 추출 */
    size_RGB565 = (size_RGB24 / 24) * 16;
    buff_RGB565 = malloc(size_RGB565);
    BitMap_convert_RGB888toRGB565(TYPE_RGB565_LITTLE, size_RGB24, buff_RGB24, buff_RGB565);
    write(fp_test_RGB565, (char *)buff_RGB565,  size_RGB565);
    free(buff_RGB565);

    /* RGB555 추출 */
    size_RGB555 = (size_RGB24 / 24) * 16;
    buff_RGB555 = malloc(size_RGB555);
    memset(buff_RGB555, 0, size_RGB555);

    BitMap_convert_RGB888toRGB555(TYPE_RGB555_LITTLE, size_RGB24, buff_RGB24, buff_RGB555);
    write(fp_test_RGB555, (char *)buff_RGB555,  size_RGB555);
    free(buff_RGB555);

    /* yuv444 추출 */
    size_yuv444 = size_RGB24;
    buff_yuv444 = malloc(size_yuv444);
    memset(buff_yuv444, 0, size_yuv444);
    YUV_convert_RGB888toYUV444(size_yuv444, buff_RGB24, buff_yuv444);
    write(fp_test_yuv444, (char *)buff_yuv444, size_RGB24);
    

    /* yuv444p 추출 */
    size_yuv444p = size_RGB24;
    buff_yuv444p = malloc(size_yuv444p);
    memset(buff_yuv444p, 0, size_yuv444p);
    YUV_convert_YUV444toYUV444p(size_yuv444p, buff_yuv444, buff_yuv444p);
    write(fp_test_yuv444p, buff_yuv444p, size_RGB24);
    free(buff_yuv444p);

    /* yuyv422 추출 */
    size_yuyv422 = (size_yuv444 / 3) * 2;
    buff_yuyv422 = malloc(size_yuyv422);
    memset(buff_yuyv422, 0, size_yuyv422);
    YUV_convert_YUV444toYUYV422(size_yuv444, buff_yuv444, buff_yuyv422);
    write(fp_test_yuyv422, buff_yuyv422, size_yuyv422);
    free(buff_yuyv422);

    /* yuv422p 추출 */
    size_yuv422p = (size_yuv444 / 3) * 2;
    buff_yuv422p = malloc(size_yuv444);
    memset(buff_yuv422p, 0, size_yuv422p);
    YUV_convert_YUV444toYUV422p(size_yuv444, buff_yuv444, buff_yuv422p, &DIB);
    write(fp_test_yuv422p, buff_yuv422p, size_yuv422p);
    free(buff_yuv422p);

    /* yuv420p 추출 */
    size_yuv420p = (size_yuv444 / 2);
    buff_yuv420p = malloc(size_yuv420p);
    memset(buff_yuv420p, 0x0, size_yuv420p);
    YUV_convert_YUV444toYUV420p(size_yuv444, buff_yuv444, buff_yuv420p, &DIB);
    write(fp_test_yuv420p, buff_yuv420p, size_yuv420p);
    free(buff_yuv420p);

    /* nv12 추출 */
    size_nv12 = (size_yuv444 / 2);
    buff_nv12 = malloc(size_nv12);
    memset(buff_nv12, 0x0, size_nv12);
    YUV_convert_YUV444toNV12(size_yuv444, buff_yuv444, buff_nv12, &DIB);
    write(fp_test_nv12, buff_nv12, size_nv12);
    free(buff_nv12);

    /* nv21 추출 */
    size_nv21 = (size_yuv444 / 2);
    buff_nv21 = malloc(size_nv21);
    memset(buff_nv21, 0x0, size_nv21);
    YUV_convert_YUV444toNV21(size_yuv444, buff_yuv444, buff_nv21, &DIB);
    write(fp_test_nv21, buff_nv21, size_nv21);
    free(buff_nv21);


    free(buff_yuv444);

    /* 맨 마지막 */
    free(buff_RGB24);
}