#ifndef __BitMap_c
#define __BitMap_c

/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "BitMap.h"
#include "../load/load.h"
/*---------------------------------------------------------------------------------------------------
************************************ GLOBAL VARIABLE DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

static t_BFH *BitMap_File_Header;
static t_DIB *BitMap_Info_Header;
static t_RGBA *pixel_data_RGBA = NULL;
static t_RGB24 *pixel_data_RGB24 = NULL;
static __uint16_t *pixel_data_RGB565 = NULL;
static __uint16_t *pixel_data_RGB555 = NULL;

static __uint32_t size_RGBA;
static __uint32_t size_RGB24;
static __uint32_t size_RGB565;
static __uint32_t size_RGB555;

/*----------------------------------------------------------------------------------------------------
************************************* LOCAL CONSTANT DEFINITIONS *************************************
----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------
*********************************** IMPORTED VARIABLE DECLARATIONS ***********************************
----------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------
************************************* LOCAL FUNCTION DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

static void BitMap_convert_RGBAtoRGB24(t_RGBA *RGBA_buff, char *RGB24_buff);
static void BitMap_Convert_RGB8BPPtoRGBA(char *RGBA_buff, char *RGB8BPP_buff, __uint32_t size_RGB8BPP, t_RGBA *ColorPalette);
static void BitMap_print_header(t_BFH *BitMap_File_Header, t_DIB *BitMap_Info_Header);
static void BitMap_print_pixeldata(void);

/*----------------------------------------------------------------------------------------------------
**************************************** FUNCTION DEFINITIONS ****************************************
----------------------------------------------------------------------------------------------------*/

void BitMap_Extract_RGB_Data(const char *__restrict__ BitMap_filename, char **RGB24_buf, __uint32_t *size, t_DIB *DIB)
{
    FILE *fp;
    int fp_test_RGBA = open("Build/output/RGBA.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fp_test_RGB24 = open("Build/output/RGB24.RAW", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char *buf, *RGB8BPP_buf = NULL;
    t_RGBA *RGBA_buf, *RGB8BPP_colorpalette = NULL;
    __uint32_t file_size; 
    
    RGB_fread(BitMap_filename, fp, &buf, &file_size);

    /* Bitmap 헤더 추출 */
    BitMap_File_Header = (t_BFH*)(buf + sizeof(__uint16_t));
    BitMap_Info_Header = (t_DIB*)(buf + sizeof(__uint16_t) + sizeof(t_BFH));

    DIB->biWidth = BitMap_Info_Header->biWidth;
    DIB->biHeight = BitMap_Info_Header->biHeight;

    BitMap_print_header(BitMap_File_Header, BitMap_Info_Header);

    switch(BitMap_Info_Header->t_DIB_16Bit.t_DIB_Bit.biBitCount)
    {
        case 32:
            /* RGBA 데이터 추출 */
            RGBA_buf = (t_RGBA *)(buf + BitMap_File_Header->bfOffBits);
    
            /* RGB24 사이즈 추출 */
            size_RGBA = (file_size - BitMap_File_Header->bfOffBits);
            *size = (size_RGBA / 4) * 3;
    
            *RGB24_buf = malloc(*size);
    
            /* RGB24 데이터 추출 */
            BitMap_convert_RGBAtoRGB24(RGBA_buf, *RGB24_buf);
        break;

        case 16:
            /* RGB24 데이터 추출 */
            *RGB24_buf = (char *)(buf + BitMap_File_Header->bfOffBits);

            /* RGB24 사이즈 추출 */
            *size = (file_size - BitMap_File_Header->bfOffBits);
        break;

        case 8:
            /* RGB8BPP 컬러 파레트 및 데이터 추출 */
            RGB8BPP_colorpalette = (t_RGBA *)(buf + BMP_HEADER_SIZE);
            RGB8BPP_buf = (char *)(buf + BitMap_File_Header->bfOffBits);

            /*RGBA 및 RGB24 사이즈 추출 */
            size_RGBA = (file_size - BitMap_File_Header->bfOffBits) * 4;
            *size = (file_size - BitMap_File_Header->bfOffBits) * 3;

            RGBA_buf = malloc(size_RGBA);
            *RGB24_buf = malloc(*size);

            /* RGBA로 변환 후 RGB24 데이터 추출 */
            BitMap_Convert_RGB8BPPtoRGBA((char *)RGBA_buf, RGB8BPP_buf, (size_RGBA / 4), RGB8BPP_colorpalette);
            BitMap_convert_RGBAtoRGB24(RGBA_buf, *RGB24_buf);

        break;

        
    }
    write(fp_test_RGBA, (char *)RGBA_buf, size_RGBA);
    write(fp_test_RGB24, (char *)*RGB24_buf, *size);
    free(RGBA_buf);
    free(buf);
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
    __uint32_t lastindex = (size_RGBA / 4) - 1;
    printf("RGBA : Blue: 0x%x Green: 0x%x Red: 0x%x Alpha: 0x%x\n",pixel_data_RGBA[lastindex].Blue, pixel_data_RGBA[lastindex].Green, pixel_data_RGBA[lastindex].Red, pixel_data_RGBA[lastindex].Alpha);
}

static void BitMap_convert_RGBAtoRGB24(t_RGBA *RGBA_buff, char *RGB24_buff)
{
    __uint32_t loop;
    t_RGBA *pixel_data_RGBA = RGBA_buff;
    t_RGB24 *pixel_data_RGB24 = (t_RGB24 *)RGB24_buff;

    for(loop = 0; loop < (size_RGBA / 4); loop ++)
    {
        pixel_data_RGB24[loop].Blue = pixel_data_RGBA[loop].Blue;
        pixel_data_RGB24[loop].Green = pixel_data_RGBA[loop].Green;
        pixel_data_RGB24[loop].Red = pixel_data_RGBA[loop].Red;
    }

    printf("RGB24: Blue: 0x%x Green: 0x%x Red: 0x%x\n",pixel_data_RGB24[loop - 1].Blue, pixel_data_RGB24[loop - 1].Green, pixel_data_RGB24[loop - 1].Red);
}

static void BitMap_Convert_RGB8BPPtoRGBA(char *RGBA_buff, char *RGB8BPP_buff, __uint32_t size_RGB8BPP, t_RGBA *ColorPalette)
{
    __uint32_t loop;
    __uint8_t RGB8BPP_data;
    t_RGBA *pixel_data_RGBA = (t_RGBA *)RGBA_buff;
    t_RGBA *colorpalette = ColorPalette;

    for(loop = 0; loop < size_RGB8BPP; loop ++)
    {
        RGB8BPP_data = RGB8BPP_buff[loop];
        pixel_data_RGBA[loop] = colorpalette[RGB8BPP_data];
    }
}

void BitMap_convert_RGB888toRGB565(__uint32_t type, __uint32_t size_RGB24, char *RGB24_buff, char *RGB565_buff)
{
    __uint32_t loop;
    t_RGB24 *pixel_data_RGB24 = (t_RGB24 *)RGB24_buff;
    __uint16_t *pixel_data_RGB565 = (__uint16_t *)RGB565_buff;
    __uint8_t rgb565_Blue, rgb565_Green, rgb565_Red;

    size_RGB565 = (size_RGB24 / 24) * 16;

    for(loop = 0; loop < (size_RGB24 / 3); loop ++)
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

void BitMap_convert_RGB888toRGB555(__uint32_t type, __uint32_t size_RGB24, char *RGB24_buff, char *RGB555_buff)
{
    __uint32_t loop;
    t_RGB24 *pixel_data_RGB24 = (t_RGB24 *)RGB24_buff;
    __uint16_t *pixel_data_RGB555 = (__uint16_t *)RGB555_buff;
    __uint8_t rgb555_Blue, rgb555_Green, rgb555_Red;

    size_RGB555 = (size_RGB24 / 24) * 16;

    for(loop = 0; loop < (size_RGB24 / 3); loop ++)
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

    printf("RGB555: 0x%x\n", pixel_data_RGB555[loop - 1]);
}
#endif