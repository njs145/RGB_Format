/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "BitMap.h"

/*---------------------------------------------------------------------------------------------------
************************************ GLOBAL VARIABLE DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

static t_BFH *BitMap_File_Header;
static t_DIB *BitMap_Info_Header;
static t_RGBA *pixel_data_RGBA = NULL;
static t_RGB24 *pixel_data_RGB24 = NULL;
static __uint32_t size;
static __uint32_t format_RGBA_Size;
static __uint32_t format_RGB24_Size;

/*----------------------------------------------------------------------------------------------------
************************************* LOCAL CONSTANT DEFINITIONS *************************************
----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------
*********************************** IMPORTED VARIABLE DECLARATIONS ***********************************
----------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------
************************************* LOCAL FUNCTION DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

static void BitMap_print_header(t_BFH *BitMap_File_Header, t_DIB *BitMap_Info_Header);
static void BitMap_print_pixeldata(void);
static void BitMap_convert_RGBAtoRGB24(void);
static void RGB_fread(const char *__restrict__ __filename, FILE *fp, char **buf, __uint32_t *size);

/*----------------------------------------------------------------------------------------------------
**************************************** FUNCTION DEFINITIONS ****************************************
----------------------------------------------------------------------------------------------------*/

void BitMap_Extract_RGB_Data(const char *__restrict__ BitMap_filename, const char *__restrict__ RGBA_output_filename, const char *__restrict__ RGB24_output_filename)
{
    FILE *fp, *fp_test_RGBA, *fp_test_RGB24;

    char *buf = NULL;

    char *buf_RGBA = NULL;
    char *buf_RGB24 = NULL;

    __uint32_t RGBA_Size;
    __uint32_t RGB24_Size;
    
    RGB_fread(BitMap_filename, fp, &buf, &size);

    /* Bitmap 헤더 추출 */
    BitMap_File_Header = (t_BFH*)(buf + sizeof(__uint16_t));
    BitMap_Info_Header = (t_DIB*)(buf + sizeof(__uint16_t) + sizeof(t_BFH));

    BitMap_print_header(BitMap_File_Header, BitMap_Info_Header);

    if(BitMap_Info_Header->t_DIB_16Bit.t_DIB_Bit.biBitCount == 0x20)
    {
        /* Bitmap 데이터 추출 */
        pixel_data_RGBA = (t_RGBA *)(buf + BitMap_File_Header->bfOffBits);

        /* 사이즈 추출 */
        format_RGBA_Size = (size - BitMap_File_Header->bfOffBits);
        format_RGB24_Size = (format_RGBA_Size / 4) * 3;
        BitMap_convert_RGBAtoRGB24();

        fp_test_RGBA = fopen(RGBA_output_filename, "w");
        fp_test_RGB24 = fopen("Bitmap_picture_RGB24.RAW", "w");

        fputs((char *)pixel_data_RGBA, fp_test_RGBA);
        fputs((char *)pixel_data_RGB24, fp_test_RGB24);

        RGB_fread(RGBA_output_filename, fp_test_RGBA, &buf_RGBA, &RGBA_Size);
        RGB_fread("Bitmap_picture_RGB24.RAW", fp_test_RGB24, &buf_RGB24, &RGB24_Size);

        BitMap_print_pixeldata();
    }
    else
    {
        /* Bitmap 데이터 추출 */
        pixel_data_RGB24 = (t_RGB24 *)(buf + BitMap_File_Header->bfOffBits);

        /* 사이즈 추출 */
        format_RGB24_Size = (size - BitMap_File_Header->bfOffBits);

        fp_test_RGB24 = fopen("Bitmap_picture_RGB24.RAW", "w");

        fputs((char *)pixel_data_RGB24, fp_test_RGB24);
        
        RGB_fread("Bitmap_picture_RGB24.RAW", fp_test_RGB24, &buf_RGB24, &RGB24_Size);
    }

    free(buf);
    free(buf_RGBA);
    free(buf_RGB24);
}

static void BitMap_print_header(t_BFH *BitMap_File_Header, t_DIB *BitMap_Info_Header)
{
    printf("bfSize: 0x%x\n",BitMap_File_Header->bfSize);
    printf("bfOffBits: 0x%x\n",BitMap_File_Header->bfOffBits);
    printf("biSize: 0x%x\n",BitMap_Info_Header->biSize);
    printf("biWidth: 0x%x\n",BitMap_Info_Header->biWidth);
    printf("biHeight: 0x%x\n",BitMap_Info_Header->biHeight);
    printf("biPlanes: 0x%x\n",BitMap_Info_Header->t_DIB_16Bit.t_DIB_Bit.biPlanes);
    printf("biBitCount: 0x%x\n",BitMap_Info_Header->t_DIB_16Bit.t_DIB_Bit.biBitCount);
    printf("biCompression: 0x%x\n",BitMap_Info_Header->biCompression);
    printf("biSizeImage: 0x%x\n",BitMap_Info_Header->biSizeImage);
    printf("biXPelsPerMeter: 0x%x\n",BitMap_Info_Header->biXPelsPerMeter);
    printf("biYPelsPerMeter: 0x%x\n",BitMap_Info_Header->biYPelsPerMeter);
    printf("biClrUsed: 0x%x\n",BitMap_Info_Header->biClrUsed);
    printf("biClrImportant: 0x%x\n",BitMap_Info_Header->biClrImportant);
}

static void BitMap_print_pixeldata(void)
{
    __uint32_t lastindex = (format_RGBA_Size / 4) - 1;
    printf("RGBA : Blue: 0x%x Green: 0x%x Red: 0x%x Alpha: 0x%x\n",pixel_data_RGBA[lastindex].Blue, pixel_data_RGBA[lastindex].Green, pixel_data_RGBA[lastindex].Red, pixel_data_RGBA[lastindex].Alpha);
}

static void BitMap_convert_RGBAtoRGB24(void)
{
    __uint32_t loop;

    pixel_data_RGB24 = malloc(format_RGB24_Size);

    for(loop = 0; loop < (format_RGBA_Size / 4); loop ++)
    {
        pixel_data_RGB24[loop].Blue = pixel_data_RGBA[loop].Blue;
        pixel_data_RGB24[loop].Green = pixel_data_RGBA[loop].Green;
        pixel_data_RGB24[loop].Red = pixel_data_RGBA[loop].Red;
    }

    printf("RGB24: Blue: 0x%x Green: 0x%x Red: 0x%x\n",pixel_data_RGB24[loop - 1].Blue, pixel_data_RGB24[loop - 1].Green, pixel_data_RGB24[loop - 1].Red);
}

static void RGB_fread(const char *__restrict__ __filename, FILE *fp, char **buf, __uint32_t *size)
{
    /* 파일 포인터 받아오기 */
    fp = fopen(__filename, "r");

    /* 파일 사이즈 구하기. */
    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* 파일 사이즈 만큼 메모리 할당. */
    *buf = malloc(*size);

    /* 파일 포인터로부터 파일 읽기 */
    fread(*buf, sizeof(char), *size, fp);
}