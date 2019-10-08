#ifndef CRIPTO_SECRETO_COMPARTIDO_BMP_H
#define CRIPTO_SECRETO_COMPARTIDO_BMP_H

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#pragma pack(push,1)
typedef struct{
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;
typedef struct{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;
typedef struct {
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
typedef struct {
    int r;
    int g;
    int b;
} pixel;
#pragma pack(pop)

bitmap* get_header(FILE *fp);
void print_header_info(FILE *fp, char *file_name);
void set_pixel_color_24(uint8_t *pixelbuffer, int width, int x, int y, int r, int g, int b);
void set_pixel_color_8(uint8_t *pixelbuffer, int width, int x, int y, int c);
pixel get_pixel_color_24(uint8_t *pixelbuffer, int width, int x, int y);
int get_pixel_color_8(uint8_t *pixelbuffer, int width, int x, int y);
void modify_aux(uint8_t* pixelarray, int width, int height, int bytes_per_pixel);
uint8_t* get_pixelarray(bitmap *pbitmap, FILE *fp);
void create_bmp_file(char* file_name, int width, int height, int bits_per_pixel);
void create_bmp_file_from_info(FILE *fp, bitmap *input_pbitmap, uint8_t *pixelarray);
void modify_bmp(char* file_name);
void copy_bmp_file(char* input, char* output);
void replace_pixelarray(char *file_name, uint8_t *new_pixelarray);
void set_palette(uint8_t *palette);

#endif //CRIPTO_SECRETO_COMPARTIDO_BMP_H
