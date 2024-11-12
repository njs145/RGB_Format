/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

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
    int fp_test_RGB24, fp_test_RGB565, fp_test_RGB555, fp_test_yuv444, fp_test_yuv444p;
    char *buff_RGB24, *buff_RGB565, *buff_RGB555, *buff_yuv444, *buff_yuv444p = NULL;
    __uint32_t size_RGB24, size_RGB565, size_RGB555;

    fp_test_RGB565 = open("Build/output/RGB565.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_RGB555 = open("Build/output/RGB555.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuv444 = open("Build/output/yuv444.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_yuv444p = open("Build/output/yuv444p.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    BitMap_Extract_RGB_Data("SampleImage/Bitmap_picture_DOG.bmp", &buff_RGB24, &size_RGB24);

    /* RGB565 추출 */
    size_RGB565 = (size_RGB24 / 24) * 16;
    buff_RGB565 = malloc(size_RGB565);
    BitMap_convert_RGB888toRGB565(TYPE_RGB565_LITTLE, size_RGB24, buff_RGB24, buff_RGB565);
    write(fp_test_RGB565, (char *)buff_RGB565,  size_RGB565);
    free(buff_RGB565);

    /* RGB555 추출 */
    size_RGB555 = (size_RGB24 / 24) * 16;
    buff_RGB555 = malloc(size_RGB555);
    BitMap_convert_RGB888toRGB555(TYPE_RGB555_LITTLE, size_RGB24, buff_RGB24, buff_RGB555);
    write(fp_test_RGB555, (char *)buff_RGB555,  size_RGB555);
    free(buff_RGB555);

    /* yuv444 추출 */
    buff_yuv444 = malloc(size_RGB24);
    YUV_convert_RGB888toYUV444(size_RGB24, buff_RGB24, buff_yuv444);
    write(fp_test_yuv444, (char *)buff_yuv444, size_RGB24);
    

    /* yuv444p 추출 */
    buff_yuv444p = malloc(size_RGB24);
    YUV_convert_YUV444toYUV444p(size_RGB24, buff_yuv444, buff_yuv444p);
    write(fp_test_yuv444p, buff_yuv444p, size_RGB24);
    free(buff_yuv444p);
    free(buff_yuv444);

    /* 맨 마지막 */
    free(buff_RGB24);
}