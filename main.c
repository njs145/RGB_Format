/*---------------------------------------------------------------------------------------------------
********************************************* INCLUDES **********************************************
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "Bitmap.h"

/*---------------------------------------------------------------------------------------------------
************************************ GLOBAL VARIABLE DEFINITIONS ************************************
---------------------------------------------------------------------------------------------------*/

static t_BFH *BitMap_File_Header;
static t_DIB *BitMap_Info_Header;
static t_BGRA *pixel_data_BGRA = NULL;
static t_BGR24 *pixel_data_BGR24 = NULL;
static __uint32_t size;
static __uint32_t format_BGRA_Size;
static __uint32_t format_BGR24_Size;

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
static void BitMap_convert_BGRAtoBGR24(void);

/*----------------------------------------------------------------------------------------------------
**************************************** FUNCTION DEFINITIONS ****************************************
----------------------------------------------------------------------------------------------------*/

int main(void)
{
    FILE *fp;
    char *buf = NULL;

    /* 파일 포인터 받아오기 */
    fp = fopen("Bitmap_picture_BGR32.bmp", "r");
    
    /* 파일 사이즈 구하기. */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* 파일 사이즈 만큼 메모리 할당. */
    buf = malloc(size);

    /* 파일 포인터로부터 파일 읽기 */
    fread(buf, sizeof(char), size, fp);

    /* Bitmap 헤더 추출 */
    BitMap_File_Header = (t_BFH*)(buf + sizeof(__uint16_t));
    BitMap_Info_Header = (t_DIB*)(buf + sizeof(__uint16_t) + sizeof(t_BFH));

    /* Bitmap 데이터 추출 */
    pixel_data_BGRA = (t_BGRA *)(buf + BitMap_File_Header->bfOffBits);

    /* 사이즈 추출 */
    format_BGRA_Size = (size - BitMap_File_Header->bfOffBits);
    format_BGR24_Size = (format_BGRA_Size / 4) * 3;

    BitMap_print_pixeldata();

    BitMap_convert_BGRAtoBGR24();

    // BitMap_print_header(BitMap_File_Header, BitMap_Info_Header);
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
    __uint32_t lastindex = (format_BGRA_Size / 4) - 1;
    printf("RGBA : Blue: 0x%x Green: 0x%x Red: 0x%x Alpha: 0x%x\n",pixel_data_BGRA[lastindex].Blue, pixel_data_BGRA[lastindex].Green, pixel_data_BGRA[lastindex].Red, pixel_data_BGRA[lastindex].Alpha);
}

static void BitMap_convert_BGRAtoBGR24(void)
{
    __uint32_t loop;

    pixel_data_BGR24 = malloc(format_BGR24_Size);

    for(loop = 0; loop < (format_BGRA_Size / 4); loop ++)
    {
        pixel_data_BGR24[loop].Blue = pixel_data_BGRA[loop].Blue;
        pixel_data_BGR24[loop].Green = pixel_data_BGRA[loop].Green;
        pixel_data_BGR24[loop].Red = pixel_data_BGRA[loop].Red;
    }

    printf("RGB24: Blue: 0x%x Green: 0x%x Red: 0x%x\n",pixel_data_BGR24[loop - 1].Blue, pixel_data_BGR24[loop - 1].Green, pixel_data_BGR24[loop - 1].Red);
}