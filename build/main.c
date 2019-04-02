#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bitmap.h"
#include "stego.h"
#include <pthread.h>

#DEFINE NUM_THREADS 4 

pthread thread_store[NUM_THREADS];

void print_help(char *path){
    printf("*** Image Steganography by LSB substitution ***\n\n"
           "Usage:  \n"
           "%s [-e] <text file to encode> <source bitmap> <destination bitmap>\n"
           "%s [-d] <encoded bitmap> <decoded file>\n\n"
           "-e : Encode text in image\n"
           "-d : Decode text from image\n",
           path, path);
}


int main(int argc, char **argv) {

    if ( argc != 5 && argc != 4 ) {
        print_help(argv[0]);
        exit(1);
    }

    int mode;

    if (!strcmp(argv[1], "-e"))
        mode = 1;
    else if (!strcmp(argv[1], "-d"))
        mode = 0;
    else {
        print_help(argv[0]);
        exit(1);
    }
    
    //TODO fork then exec ffmpeg to split up source file into component bmp files 
    
    //TODO split up text file into portions equivalent to the number of frames 
    
    //TODO spin off number of threads to conduct steg operations 
    
    //TODO 

    if(mode){
        encode(argv[2], argv[3], argv[4]);
    } else{
        decode(argv[2], argv[3]);
    }

    return EXIT_SUCCESS;
}

