#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bitmap.h"
#include "stego.h"
#include <pthread.h>

#define NUM_THREADS 4 

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
    
    if(mode){
        //TODO split up text file into portions equivalent to the number of frames? or some other form of parsing 
        //TODO spin off number of threads to conduct encode operation
        encode(argv[2], argv[3], argv[4]);
        //TODO fork then exec ffmpeg to join files back into a single ouput video (ensure same size and frame rate as source, use lossless codec)
        //put ffmpeg arguments here, theyre pretty long and detailed 
    } else{
        //TODO spin off number of threads to conduct decode operation
        //TODO should output a single text file, figure out how to join multiple threads (mutex that doesnt ruin parrallelization?)
        decode(argv[2], argv[3]);
    }
    
    return EXIT_SUCCESS;
}

