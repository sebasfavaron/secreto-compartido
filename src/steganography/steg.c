//
// Created by sebas on 06/06/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <stdint.h>

/// Write "secret" bit by bit into the LSB (Least Significant Bits)
/// of "dest", a pixelarray
/// If LSB2 boolean is true, the secret message will be encoded onto
/// the two least significant bits instead of one
void encode_bits(uint8_t *secret, uint8_t *dest, int secret_length, int dest_length, bool LSB2){

    if(secret_length > dest_length){
        printf("Secret longer that the image to hide it in. Exiting..\n");
        exit(0);
    }
    int byte_pos = 0; //from 0 to 7, tracks where you are in the current byte read from secret
    int secret_pos = 0, LSB_number = 1;
    if(LSB2){
        LSB_number = 2;
    }
    for(int dest_pos=0; dest_pos < dest_length; dest_pos++){
        uint8_t secret_char;
        byte_pos %= 8;
        if(byte_pos == 0){
            if(secret_pos > secret_length){
                break;
            }
            secret_char = secret[secret_pos++];
        }

        uint8_t channel = dest[dest_pos];

        /// insert next bit from secret in next least significant bit
        for(int i=0; i<LSB_number; i++) {
            byte_pos %= 8;
            unsigned long bit = (secret_char >> (7 - byte_pos)) & 1UL; // extract bit from secret
            if (bit == 1) {
                // set bit (set to 1)
                channel = channel | (1 << i);
            } else {
                // clear bit (set to 0)
                channel = channel & ~(1 << i);
            }
            if(i < LSB_number-1){
                byte_pos++;
            }
        }

        dest[dest_pos] = channel;
        byte_pos++;
    }
}

/// Take bmp file "output" and insert "secret" (stream of chars) into the least
/// significant bits (LSB) of each channel (R,G,B) of each pixel of "output"
/// (function may assume that output is bmp of 24 bits_per_pixel, as
/// specified in the cryptography assignment)
/// If LSB2 boolean is true, the secret message will be encoded onto
/// the two least significant bits instead of one
void encode(uint8_t* output, uint8_t* secret, int secret_len, bool LSB2){

    /// 1. write outputs pixelarray in a char* out_pixelarray
    FILE *p_output = fopen(output,"rb+");
    bitmap *p_output_bitmap = get_header(p_output);
    int width = p_output_bitmap->bitmapinfoheader.width; // is also the width of secret
    int height = p_output_bitmap->bitmapinfoheader.height; // is also the height of secret
    int bytes_per_pixel = p_output_bitmap->bitmapinfoheader.bitsperpixel/8;
    int out_offset = p_output_bitmap->fileheader.fileoffset_to_pixelarray;
    int out_size = p_output_bitmap->bitmapinfoheader.imagesize;
    if(bytes_per_pixel != 3){
        printf("Output bits per pixel must be 24. Exiting..\n");
        exit(0);
    }
    uint8_t *out_pixelarray = (uint8_t *) get_pixelarray(p_output_bitmap, p_output);

    /// 2. write secret bit by bit in the LSB of out_pixelarray
    encode_bits(secret, out_pixelarray, secret_len, out_size, LSB2);

    /// 3. rewrite out_pixelarray into output file
    fseek(p_output, out_offset, SEEK_SET);
    fwrite(out_pixelarray, 1, out_size, p_output);

    /// 4. free memory
    fclose(p_output);
    free(p_output_bitmap);
    free(out_pixelarray);
}

/// Extract secret bit by bit from the LSB of dest, a pixelarray
uint8_t* decode_bits(uint8_t *carrier, int carrier_length, int secret_length, bool LSB2){
    uint8_t *secret = calloc(1, secret_length);

    int byte_pos = 0; //from 0 to 7, tracks where you are in the current byte write to secret
    int secret_pos = 0, LSB_number = 1;
    if(LSB2){
        LSB_number = 2;
    }
    for(int carrier_pos=0; carrier_pos<carrier_length; carrier_pos++){
        byte_pos %= 8;
        if(byte_pos == 0){
            if(secret_pos > secret_length){
                return secret;
            }
            secret_pos++;
        }

        /// extract bit number "byte_pos" from next LSB onto secret[secret_pos] in the next bit available
        if(byte_pos == 0){
            secret[secret_pos] = 0;
        }
        uint8_t channel = carrier[carrier_pos];

        for (int i = 0; i < LSB_number; ++i) {
            byte_pos %= 8;
            unsigned long channel_bit = (channel >> i) & 1UL;
            secret[secret_pos] |= channel_bit; // set last bit to the last bit of channel
            if(byte_pos != 7){
                secret[secret_pos] <<= 1;
            }
            if(i < LSB_number-1){
                byte_pos++;
            }
        }

        byte_pos++;
    }
    return secret;
}

/// Extract hidden char* of size "secret_length" from "input"
uint8_t* decode(uint8_t* input, int secret_length, bool LSB2){
    FILE *p_input = fopen(input, "rb");

    bitmap *p_input_bitmap = get_header(p_input);
    int width  = p_input_bitmap->bitmapinfoheader.width;
    int height = p_input_bitmap->bitmapinfoheader.height;
    int input_bytes_per_pixel = p_input_bitmap->bitmapinfoheader.bitsperpixel/8;
    int input_offset = p_input_bitmap->fileheader.fileoffset_to_pixelarray;
    if(input_bytes_per_pixel != 1 && input_bytes_per_pixel != 3){
        printf("This steganography encoding algorithm only supports bmp files with 1 or 3 bytes per pixel. Exiting..");
        exit(0);
    }

    /// 1. read inputs pixelarray to a char* in_pixelarray
    fseek(p_input, input_offset, SEEK_SET);
    int in_size = p_input_bitmap->bitmapinfoheader.imagesize;
    uint8_t *in_pixelarray = (uint8_t *)calloc(1, in_size * sizeof(uint8_t));
    fread(in_pixelarray, in_size, 1, p_input);

    /// 2. decode secret from out_pixelarray
    uint8_t*secret_stream = decode_bits(in_pixelarray, width*height*input_bytes_per_pixel, secret_length, LSB2);

    /// 3. free memory
    fclose(p_input);
    free(in_pixelarray);

    return secret_stream;
}

// test to see that steganography is working independently
void run_paper(int argc , char *argv[]) {
    char *input = argv[1];
    char *secret = argv[2];

    int amount_partitions = argc - 3;
    char *outputs[amount_partitions];
    for (int i = 0; i < amount_partitions; i++) {
        outputs[i] = argv[i + 3];
        copy_bmp_file(input, outputs[i]);
    }

    bool LSB2 = false;

    FILE *p_secret = fopen(secret, "rb");
    bitmap *p_secret_bitmap = get_header(p_secret);
    int s_bytes_per_pixel = p_secret_bitmap->bitmapinfoheader.bitsperpixel/8;
    if(s_bytes_per_pixel != 1){
        printf("Secret bits per pixel must be 8. Exiting..\n");
        exit(0);
    }
    int width = p_secret_bitmap->bitmapinfoheader.width;
    int height = p_secret_bitmap->bitmapinfoheader.height;
    int s_pixelarray_size = s_bytes_per_pixel * width * height;
    int s_palette_size = 1024;
    int s_size = sizeof(bitmap) + s_palette_size + s_pixelarray_size; // 1024 is the color palette size (between header and pixelarray)
    uint8_t *secret_stream = (uint8_t *)calloc(1, s_size * sizeof(uint8_t));

    fseek(p_secret, 0, SEEK_SET);
    fread(secret_stream, s_size, 1, p_secret); // read secret header+palette+pixelarray into stream

    /// Encoding
    int remaining_size = s_size;
    for(int i=0; i<amount_partitions; i++){
        int encode_size = s_size/amount_partitions;
        if(remaining_size < encode_size){
            encode_size = remaining_size;
        }
        encode(outputs[i], secret_stream + s_size/amount_partitions*i, encode_size, LSB2);
        remaining_size -= encode_size;
    }
    fclose(p_secret);

    /// Decoding
    FILE *p_secret_decoded = fopen("decoded.bmp", "wb");
    fseek(p_secret_decoded, 0, SEEK_SET);
    for(int i=0; i<amount_partitions; i++){
        int partition_size;
        if(i < amount_partitions-1){
            partition_size = s_size/amount_partitions;
        } else {
            partition_size = remaining_size;
        }
        unsigned char *partition = decode(outputs[i], partition_size, LSB2);
        fwrite(partition, 1, partition_size, p_secret_decoded);
    }
    print_header_info(p_secret_decoded, secret);

    fclose(p_secret_decoded);
}

/// Make a copy of argv[1], encode in it the secret and decode it
/// The secret remains intact after the encoding-decoding process
void example(char *argv[]){
    char *copy_file = "copy.bmp";
    copy_bmp_file(argv[1], copy_file);
    bool LSB2 = false;
    uint8_t secret[] = "bet you dont know what im saying";
    encode(copy_file, secret, strlen(secret), LSB2);
    uint8_t *decoded;
    decoded = decode(copy_file, strlen(secret), LSB2);
    for(int i=0; i<strlen(secret)+1; i++){
        printf("%c", decoded[i]);
    }
    printf("\n");
}
