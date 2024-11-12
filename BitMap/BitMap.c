/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "BitMap.h"
#include <fcntl.h>
#include <unistd.h>

/*---------------------------------------------------------------------------------------------------
************************************ GLOBAL VARIABLE DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

static t_BFH *BitMap_File_Header;
static t_DIB *BitMap_Info_Header;
static t_RGBA *pixel_data_RGBA = NULL;
static t_RGB24 *pixel_data_RGB24 = NULL;
static __uint16_t *pixel_data_RGB565 = NULL;
static __uint16_t *pixel_data_RGB555 = NULL;
static __uint32_t size;
static __uint32_t format_RGBA_Size;
static __uint32_t format_RGB24_Size;
static __uint32_t format_RGB565_Size;
static __uint32_t format_RGB555_Size;

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
static void BitMap_convert_RGB888toRGB565(__uint32_t type);
static void BitMap_convert_RGB888toRGB555(__uint32_t type);

/*----------------------------------------------------------------------------------------------------
**************************************** FUNCTION DEFINITIONS ****************************************
----------------------------------------------------------------------------------------------------*/

void BitMap_Extract_RGB_Data(const char *__restrict__ BitMap_filename)
{
    FILE *fp; 
    int fp_test_RGBA, fp_test_RGB24, fp_test_RGB565, fp_test_RGB555;

    char *buf = NULL;

    char *buf_RGBA = NULL;
    char *buf_RGB24 = NULL;
    char *buf_RGB565 = NULL;

    __uint32_t RGBA_Size;
    __uint32_t RGB24_Size;
    __uint32_t RGB565_Size;

    fp_test_RGBA = open("Build/output/RGBA.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_RGB24 = open("Build/output/RGB24.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_RGB565 = open("Build/output/RGB565.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fp_test_RGB555 = open("Build/output/RGB555.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
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

        write(fp_test_RGBA, (char *)pixel_data_RGBA,  format_RGBA_Size);
        write(fp_test_RGB24, (char *)pixel_data_RGB24,  format_RGB24_Size);

        // RGB_fread(RGBA_output_filename, fp_test_RGBA, &buf_RGBA, &RGBA_Size);
        // RGB_fread(RGB24_output_filename, fp_test_RGB24, &buf_RGB24, &RGB24_Size);

        BitMap_print_pixeldata();
    }
    else
    {
        close(fp_test_RGBA);
        /* Bitmap 데이터 추출 */
        pixel_data_RGB24 = (t_RGB24 *)(buf + BitMap_File_Header->bfOffBits);

        /* 사이즈 추출 */
        format_RGB24_Size = (size - BitMap_File_Header->bfOffBits);

        write(fp_test_RGB24, (char *)pixel_data_RGB24,  format_RGB24_Size);
        
        // RGB_fread(RGB24_output_filename, fp_test_RGB24, &buf_RGB24, &RGB24_Size);
    }

    BitMap_convert_RGB888toRGB565(TYPE_RGB565_BIG);
    write(fp_test_RGB565, (char *)pixel_data_RGB565,  format_RGB565_Size);

    BitMap_convert_RGB888toRGB555(TYPE_RGB555_BIG);
    write(fp_test_RGB555, (char *)pixel_data_RGB555,  format_RGB555_Size);

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

static void BitMap_convert_RGB888toRGB565(__uint32_t type)
{
    __uint32_t loop;
    __uint8_t rgb565_Blue, rgb565_Green, rgb565_Red;

    format_RGB565_Size = (format_RGB24_Size / 24) * 16;

    pixel_data_RGB565 = malloc(format_RGB565_Size);

    for(loop = 0; loop < (format_RGB24_Size / 3); loop ++)
    {
        rgb565_Blue = (__uint8_t)(pixel_data_RGB24[loop].Blue >> 3);
        rgb565_Green = (__uint8_t)(pixel_data_RGB24[loop].Green >> 2);
        rgb565_Red = (__uint8_t)(pixel_data_RGB24[loop].Red >> 3);

        switch(type)
        {
            case TYPE_RGB565_LITTLE:
                pixel_data_RGB565[loop] = (__uint16_t)((rgb565_Blue << 11) | (rgb565_Green << 5) | rgb565_Red);
            break;

            case TYPE_RGB565_BIG:
                pixel_data_RGB565[loop] = (__uint16_t)(((rgb565_Green & 0x7) << 13) | (rgb565_Red << 8) | (rgb565_Blue << 3) | ((rgb565_Green & 0x38) >> 3));
            break;
        }
    }

    printf("RGB565: 0x%x\n", pixel_data_RGB565[loop - 1]);
}

static void BitMap_convert_RGB888toRGB555(__uint32_t type)
{
    __uint32_t loop;
    __uint8_t rgb555_Blue, rgb555_Green, rgb555_Red;

    format_RGB555_Size = (format_RGB24_Size / 24) * 16;

    pixel_data_RGB555 = malloc(format_RGB565_Size);

    for(loop = 0; loop < (format_RGB24_Size / 3); loop ++)
    {
        rgb555_Blue = (__uint8_t)(pixel_data_RGB24[loop].Blue >> 3);
        rgb555_Green = (__uint8_t)(pixel_data_RGB24[loop].Green >> 3);
        rgb555_Red = (__uint8_t)(pixel_data_RGB24[loop].Red >> 3);

        switch(type)
        {
            case TYPE_RGB555_LITTLE:
                pixel_data_RGB555[loop] = (__uint16_t)((rgb555_Blue << 10) | (rgb555_Green << 5) | rgb555_Red);
            break;

            case TYPE_RGB555_BIG:
                pixel_data_RGB555[loop] = (__uint16_t)(((rgb555_Green & 0x7) << 13) | (rgb555_Red << 8) | (rgb555_Blue << 2) | ((rgb555_Green & 0x18) >> 3));
            break;
        }
    }

    printf("RGB555: 0x%x\n", pixel_data_RGB565[loop - 1]);
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