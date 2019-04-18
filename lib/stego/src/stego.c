//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#include "stego.h"
#include "bitmap.h"
#include "util.h"

unsigned char _get_bit(char byte, int bit_number) {
    return (unsigned char) ((byte >> bit_number) & 1);
}

void _store_length(long size, unsigned char *data) {
    data[0] = (unsigned char) ((size >> 24) & 0xFF);
    data[1] = (unsigned char) ((size >> 16) & 0xFF);
    data[2] = (unsigned char) ((size >> 8) & 0xFF);
    data[3] = (unsigned char) (size & 0xFF);
}

long _get_length(const unsigned char *data) {
    long length = 0;
    length = (long) (data[3] & 0xFF);
    length = length | (long) ((data[2] & 0xFF) << 8);
    length = length | (long) ((data[1] & 0xFF) << 16);
    length = length | (long) ((data[0] & 0xFF) << 24);

    return length;
}

/*
Takes a text, a (char pointer or char array?) original image and a (char pointer or char array?) destination image
Opens the text source, calls read_bitmap to populate a bitmap struct with the origional image data 
Takes a single char from source text, splits it into 8 bits, and changes last bit of 8 bytes of image data
to the 8 bits in the char of the source text.
Iterates to the next char in the source text until EOF of source file is reached, then writes the image bitmap 
into the destination image 
*/
void *encode(void* arg) {
    //TODO: add size of text source to encoded image
    
    thread_data_t *input = (thread_data_t *) arg;
    
    char *text_source = input->text_source;
    char *original_image = input->original_image;
    char *destination_image = input->destination_image;
        
    //unsure error_message use?
    char *error_message = NULL;
    char buff;
    int index = 4;
    unsigned char mask = 1;
    FILE *text_file = _open_file(text_source, "r");
    long text_size = _get_text_length(text_file);
    
    //takes the input image and populates a bitmap with its data 
    Bitmap *image = read_bitmap(original_image, &error_message);
    _store_length(text_size, image->data);
    
    //conducts some bitwise shift operations to separate single char from text into 8 bits 
    //and spread it between 8 bytes in the image file, depositing it in the last bit of each image byte  
    do {
        buff = (char) fgetc(text_file);
        for (int i = 0; i < 8; i++) {
            image->data[index] = (image->data[index] & ~mask) | (_get_bit(buff, i) & mask);
            index++;
        }
    } while (!feof(text_file));

    //writes (bitmap) image to destination image 
    write_bitmap(destination_image, image, &error_message);
    bool is_valid = check_bitmap_header(image->header, "sw_poster_copy.bmp");
    return (void*) is_valid;
}

/*
Takes an (char array) image source and (char array) destination text
Populates the image source into a bitmap struct
Shaves off the last bit in 8 bytes of the image file and populates a char with the bits it shaves off;
continues into for message length until done
*/
void *decode(void* arg) {
    
    thread_data_t *input = (thread_data_t *) arg;
    
    char *image_source = input->image_source;
    char *text_destination = input->text_destination;
    
    char *error_message = NULL;
    unsigned char buff = 0;
    unsigned char mask = 1;
    long message_length;
    Bitmap *image = read_bitmap(image_source, &error_message);
    FILE *text_file = _open_file(text_destination, "w");
    message_length = 1228746;
    for (int index = 4; index < message_length * 8; index++) {
        buff = buff | ((image->data[index] & mask) << ((index - 4) % 8));
        if (((index - 4) % 8) > 6) {
            fputc(buff, text_file);
            buff = 0;
        }
    }

    bool is_valid = true;
    return (void*) is_valid;
}

//TODO: decode //Looks like decode is complete?
