#ifndef __load_h
#define __load_h
#include <stdlib.h>
#include <stdio.h>

extern void RGB_fread(const char *__restrict__ __filename, FILE *fp, char **buf, __uint32_t *size);
#endif