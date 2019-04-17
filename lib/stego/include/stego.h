//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#ifndef STEGO_H
#define STEGO_H

#include <stdbool.h>

void _store_length(long size, unsigned char *data);
long _get_length(const unsigned char *data);
void *encode(void* arg);
void *decode(coid* arg);

#endif //STEGO_H
