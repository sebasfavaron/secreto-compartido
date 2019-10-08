#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include "bmp.h"


/// Prints out bmp header info. Will reset file pointer (fp) to the beggining of the file
bitmap* get_header(FILE *fp){
    bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
    fseek(fp, 0, SEEK_SET);
    fread(pbitmap, sizeof(bitmap), 1, fp);
    fseek(fp, 0, SEEK_SET); //reset pointer to beggining of file

    return pbitmap;
}

void print_header_info(FILE *fp, char *file_name){
    bitmap *pbitmap = get_header(fp);

    printf("/-----------------------------------------------------\\\n");
    printf("|\tFile name: %s\n", file_name);
    printf("|\tSignature: %s\n", pbitmap->fileheader.signature);
    printf("|\tFile size: %d\n", pbitmap->fileheader.filesize);
    printf("|\tOffset to pixarray: %d\n", pbitmap->fileheader.fileoffset_to_pixelarray);
    printf("|\tDib header size: %d\n", pbitmap->bitmapinfoheader.dibheadersize);
    printf("|\tWidth: %d\n", pbitmap->bitmapinfoheader.width);
    printf("|\tHeight: %d\n", pbitmap->bitmapinfoheader.height);
    printf("|\tPlanes: %d\n", pbitmap->bitmapinfoheader.planes);
    printf("|\tBits per pixel: %d\n", pbitmap->bitmapinfoheader.bitsperpixel);
    printf("|\tCompression: %d\n", pbitmap->bitmapinfoheader.compression);
    printf("|\tImage size: %d\n", pbitmap->bitmapinfoheader.imagesize);
    printf("|\tY pixel per meter: %x\n", pbitmap->bitmapinfoheader.ypixelpermeter);
    printf("|\tX pixel per meter: %x\n", pbitmap->bitmapinfoheader.xpixelpermeter);
    printf("|\tNum color pallette: %d\n", pbitmap->bitmapinfoheader.numcolorspallette);
    printf("\\-----------------------------------------------------/\n");
}

/// Sets color of one pixel on "24 bits per pixel" images
void set_pixel_color_24(uint8_t *pixelbuffer, int width, int x, int y, int r, int g, int b) {
    int pos = y*width + x;
    pixelbuffer[3*pos + 0] = b;
    pixelbuffer[3*pos + 1] = g;
    pixelbuffer[3*pos + 2] = r;
}

/// Sets color of one pixel on "8 bits per pixel" images
void set_pixel_color_8(uint8_t *pixelbuffer, int width, int x, int y, int c) {
    int pos = y*width + x;
    pixelbuffer[pos] = c;
}

/// Gets color of one pixel on "24 bits per pixel" images. Returns it as a pixel struct
pixel get_pixel_color_24(uint8_t *pixelbuffer, int width, int x, int y){
    pixel pix;
    int pos = y*width + x;

    pix.b = pixelbuffer[3*pos + 0];
    pix.g = pixelbuffer[3*pos + 1];
    pix.r = pixelbuffer[3*pos + 2];

    return pix;
}

/// Gets color of one pixel on "8 bits per pixel" images
int get_pixel_color_8(uint8_t *pixelbuffer, int width, int x, int y){
    int pos = y*width + x;

    return pixelbuffer[pos];
}

/// Example function of how one could modify an existing image
/// Called by modify_bmp()
void modify_aux(uint8_t* pixelarray, int width, int height, int bytes_per_pixel) {
    int x,y;
    switch (bytes_per_pixel){
        case 1:
            for (y = 0; y < height; ++y) {
                for (x = 0; x < width; ++x) {
                    set_pixel_color_8(pixelarray, width, x, y, y*width + x);
                }
            }
            break;
        case 3:
            for (y = 0; y < height; ++y) {
                for (x = 0; x < width; ++x) {
                    if(y%5 == 0) set_pixel_color_24(pixelarray, width, x, y, 150, 150, 150);
                }
            }
            break;
        default:
            printf("Only implemented for 8 and 24 bits per pixel. Exiting..\n");
            exit(0);
    }
}

/// Given an image header and file pointer, return the pixels in an array of uint8_t
uint8_t* get_pixelarray(bitmap *pbitmap, FILE *fp){

    int bytes_per_pixel = pbitmap->bitmapinfoheader.bitsperpixel/8;
    int width = pbitmap->bitmapinfoheader.width;
    int height = pbitmap->bitmapinfoheader.height;
    uint32_t offset = pbitmap->fileheader.fileoffset_to_pixelarray;

    uint8_t *pixelarray = (uint8_t*)calloc(1, sizeof(uint8_t) * bytes_per_pixel * width * height);

    //read pixel array and write in pixelarray
    fseek(fp, offset, SEEK_SET); //jump to pixelarray
    fread(pixelarray, bytes_per_pixel * width * height, 1, fp);
    fseek(fp, 0, SEEK_SET); //reset pointer to beggining of file

    return pixelarray;
}

void set_palette(uint8_t *palette) {
    for(int i=0; i<1024; i++){
        palette[i] = i%256;
    }
}

/// Creates default bmp file from desired file name (if file exists, it will be overwritten)
void create_bmp_file(char* file_name, int width, int height, int bits_per_pixel) {
    FILE *fp = fopen(file_name,"wb");
    int bytes_per_pixel = bits_per_pixel/8;
    int pixel_buffer_size = width * height * bytes_per_pixel;
    bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
    uint8_t *pixelbuffer = (uint8_t*)malloc(pixel_buffer_size);
    strcpy(pbitmap->fileheader.signature,"BM");
    if(bytes_per_pixel == 3){
        pbitmap->fileheader.filesize = sizeof(bitmap)+pixel_buffer_size;
        pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
        pbitmap->bitmapinfoheader.numcolorspallette = 0;
    } else if(bytes_per_pixel == 1){
        pbitmap->fileheader.filesize = sizeof(bitmap)+ 1024 +pixel_buffer_size;
        pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap) + 1024;
        pbitmap->bitmapinfoheader.numcolorspallette = 256;
    } else {
        printf("Only implemented for 8 and 24 bits per pixel. Exiting..\n");
        exit(EXIT_SUCCESS);
    }
    pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = width;
    pbitmap->bitmapinfoheader.height = height;
    pbitmap->bitmapinfoheader.planes = 1;
    pbitmap->bitmapinfoheader.bitsperpixel = bits_per_pixel;
    pbitmap->bitmapinfoheader.compression = 0;
    pbitmap->bitmapinfoheader.imagesize = pixel_buffer_size;
    pbitmap->bitmapinfoheader.ypixelpermeter = 0;
    pbitmap->bitmapinfoheader.xpixelpermeter = 0;
    fwrite(pbitmap, 1, sizeof(bitmap), fp); // write header

    if(bytes_per_pixel == 1){
        uint8_t *palette = calloc(1, 1024 * sizeof(uint8_t));
        set_palette(palette);
        fwrite(palette, 1, sizeof(bitmap), fp); // write palette
        free(palette);
    }

    int x,y;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            if(bits_per_pixel == 24){
                // Paints image default purple color
                set_pixel_color_24(pixelbuffer, width, x, y, 100, 0, 300);
            } else {
                // Paints image gray
                set_pixel_color_8(pixelbuffer, width, x, y, 100);
            }
        }
    }

    fwrite(pixelbuffer, 1, pixel_buffer_size, fp); // write pixels
    fclose(fp);
    free(pbitmap);
    free(pixelbuffer);
}

/// Creates new bmp file (file from pointer fp) from other header and pixel array
void create_bmp_file_from_info(FILE *fp, bitmap *input_pbitmap, uint8_t *pixelarray) {
    fseek(fp, 0, SEEK_SET); //reset pointer to beggining of file
    fwrite(input_pbitmap, 1, sizeof(bitmap), fp);
    fwrite(pixelarray, 1, input_pbitmap->bitmapinfoheader.imagesize, fp);
    fseek(fp, 0, SEEK_SET); //reset pointer to beggining of file
}

/// Extracts pixel array to a uint8_t array, where it can be modified before it is written again to the file
void modify_bmp(char* file_name) {
    FILE *fp = fopen(file_name,"rb+");

    bitmap *pbitmap = get_header(fp);

    int bitsperpixel = pbitmap->bitmapinfoheader.bitsperpixel;
    if(bitsperpixel != 8 && bitsperpixel != 24){
        printf("Only implemented for 8 and 24 bits per pixel. Exiting..\n");
        exit(EXIT_FAILURE);
    }

    int width = pbitmap->bitmapinfoheader.width, height = pbitmap->bitmapinfoheader.height;
    int bytes_per_pixel = bitsperpixel/8;
    uint8_t *pixelarray = get_pixelarray(pbitmap, fp);

    // Modify pixelarray
    modify_aux(pixelarray, width, height, bytes_per_pixel);

    // Re-write pixelarray
    fseek(fp, pbitmap->fileheader.fileoffset_to_pixelarray, SEEK_SET); //jump to pixelarray
    fwrite(pixelarray, 1, bytes_per_pixel * width * height, fp);

    fclose(fp);
    free(pixelarray);
}

/// Takes bmp file "input" and creates a new file "output" with the same data
void copy_bmp_file(char* input, char* output){
    FILE *pinput = fopen(input,"rb");
    FILE *poutput = fopen(output,"wb");
    if(pinput == NULL){
        printf("Cannot open file %s.\n", input);
        fclose(pinput);
        exit(0);
    }
    if(poutput == NULL){
        printf("Cannot open file %s.\n", output);
        fclose(poutput);
        exit(0);
    }
    bitmap *pbitmap = get_header(pinput);
    uint8_t *pixelarray = get_pixelarray(pbitmap, pinput);

    create_bmp_file_from_info(poutput, pbitmap, pixelarray);

    fclose(pinput);
    fclose(poutput);
}

void replace_pixelarray(char *file_name, uint8_t *new_pixelarray) {
    FILE *fp = fopen(file_name,"rb+");

    bitmap *pbitmap = get_header(fp);

    int bitsperpixel = pbitmap->bitmapinfoheader.bitsperpixel;
    if(bitsperpixel != 8 && bitsperpixel != 24){
        printf("Only implemented for 8 and 24 bits per pixel. Exiting..\n");
        exit(EXIT_FAILURE);
    }

    int width = pbitmap->bitmapinfoheader.width, height = pbitmap->bitmapinfoheader.height;
    int bytes_per_pixel = bitsperpixel/8;
    uint8_t *pixelarray = get_pixelarray(pbitmap, fp);

    // Modify pixelarray
    int x,y;
    switch (bytes_per_pixel){
        case 1:
            for (y = 0; y < height; ++y) {
                for (x = 0; x < width; ++x) {
                    set_pixel_color_8(pixelarray, width, x, y, new_pixelarray[y*width + x]);
                }
            }
            break;
        case 3:
            for (y = 0; y < height; ++y) {
                for (x = 0; x < width; ++x) {
                    set_pixel_color_24(pixelarray, width, x, y, new_pixelarray[y*width + x + 0], new_pixelarray[y*width + x + 1], new_pixelarray[y*width + x + 2]);
                }
            }
            break;
        default:
            printf("Only implemented for 8 and 24 bits per pixel. Exiting..\n");
            exit(EXIT_FAILURE);
    }

    // Re-write pixelarray
    fseek(fp, pbitmap->fileheader.fileoffset_to_pixelarray, SEEK_SET); //jump to pixelarray
    fwrite(pixelarray, 1, bytes_per_pixel * width * height, fp);

    fclose(fp);
    free(pixelarray);

}