#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bitmap.h"
#include "stego.h"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_THREADS 4

#define FFMPEG_PATH "/usr/bin/ffmpeg"

pthread thread_store[NUM_THREADS];

void print_help(char *path){
    printf("*** Image Steganography by LSB substitution ***\n\n"
           "Usage:  \n"
           "%s [-e] <text file to encode> <source video> <destination video>\n"
           "%s [-d] <encoded video> <decoded file>\n\n"
           "-e : Encode text in video\n"
           "-d : Decode text from video\n",
           path, path);
}

void encode_decode (int mode)
{
    if(mode){
        //TODO split up text file into portions equivalent to the number of frames? or some other form of parsing 
        
        //TODO spin off number of threads to conduct encode operation
        
        for(int i = 0; i < NUM_Threads; i++)   
            pthread_create(thread_store[i], NULL, *encode(), NULL);
        
        encode(argv[2], argv[3], argv[4]);
        
        // join threads
        
        //TODO fork then exec ffmpeg to join files back into a single ouput video (ensure same size and frame rate as source, use lossless codec)
        //put ffmpeg arguments here, theyre pretty long and detailed    
        
    } else {
        //TODO spin off number of threads to conduct decode operation
        
        for (int i = 0; i < NUM_THREADS; i++)
            pthread_create(thread_store[i], NULL, *decode(), NULL);
        
        //join threads
        
        //TODO should output a single text file, figure out how to join multiple txt files (mutex that doesnt ruin parrallelization?)
        
        decode(argv[2], argv[3]);
    }
}

/*
Splits the files into bmp files
*/
void split_ffmepg (char *video, int mode)   
{
    //fmpeg split video into component parts
    char instructions[] = { "-i big_buck_bunny_480p_stereo frame%010.bmp -hide_banner" };
    execv(FFMPEG_PATH, instructions[]);
}

/*
Joins the files into a single avi file a
*/
void join_ffmepg (char *video, int mode)   
{
    char instructions[] = { "" };
    execv(FFMPEG_PATH, instructions[]);
}

int main(int argc, char **argv) {

    pid_t pid; 
    
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
    
    //TODO POSSIBLE use ffprobe to populate file data and control variables (size of txt each BMP can store, etc) 
    //(requires parsing JSON or CSV)
    
    //TODO fork then exec ffmpeg to split up source file into component bmp files, wait for completion then encode/decode 
    pid = fork();
    
    if (pid == 0)
        split_ffmpeg( ,mode);
  
    else if(pid > 0)
        encode_decode( mode);
    
    //delete the temp bmp files from directory
    //execv rm -rf *.bmp
    
    return EXIT_SUCCESS;
}
