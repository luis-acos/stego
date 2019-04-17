//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#ifndef STEGO_H
#define STEGO_H

#include <stdbool.h>

typedef struct _thread_data_t{
  
  char *text_source;
  char *original_image;
  char *destination_image;
  char *image_source;
  char *text_destination;
  
} thread_data_t;

void _store_length(long size, unsigned char *data);
long _get_length(const unsigned char *data);
void *encode(void* arg);
void *decode(void* arg);

#endif //STEGO_H
