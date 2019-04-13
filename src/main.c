#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bitmap.h"
#include "stego.h"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 8

#define FFMPEG_PATH "/usr/bin/ffmpeg"
#define SPLIT_PATH "usr/bin/split"
#define RM_PATH "/bin/rm"

pthread thread_store[NUM_THREADS];

//hardcoded to default values pending change by parsing function
int num_frames = 14315;
int chars_in_text = 399122;
int chars_per_frame = 51240;

int frames_to_encode = NUM_THREADS; 

//int chars_per_frame = width * height; 

/*
Provides an explanation of command line arguments 
*/
void print_help(char *path){
    printf("*** Video Steganography by LSB substitution ***\n\n"
           "Usage:  \n"
           "%s [-e] <text file to encode> <source video> <destination video>\n"
           "%s [-d] <encoded video> <decoded file>\n\n"
           "-e : Encode text in video\n"
           "-d : Decode text from video\n",
           path, path);
}

/*
TO DO
Execv into ffprobe, produce info, then parse info into string array 
*/
void parse_video_info(char *video)
{
    return video;
}

/*
Splits the given text file into multiple smaller text files for passing to encode/decode strings 
*/
void split_text(char *text)
{
    printf ("Splitting source text into multiple text files.");
    
    //tested and works; note, the files may have different names depending on environment, have to test this in docker
    char instructions[] = {"input.txt -n 8 -d --additional-suffix=.txt"};     
    execv(SPLIT_PATH, instructions);
}

/*
Splits the video file into bmp files
*/
void split_ffmpeg (char *video, int mode)   
{
    printf("Parsing video into frames, this may take a while.\n");
    
    char instructions[] = { "-i big_buck_bunny_480p_stereo.avi frame%09d.bmp -hide_banner" };
    execv(FFMPEG_PATH, instructions);
}

/*
Joins the files into a single avi file
TO DO allow for custom inputs / modify fps, resolution, pixel format?
*/
void join_ffmpeg (char *video, int mode)   
{
    printf("Joining frames into encoded video. Time to grab some tea.\n");
    
    char instructions[] = { "-r 24 -s 854x480 -i frame%09d.png -vcodec ffv1 -crf 25 output.avi" };
    execv(FFMPEG_PATH, instructions);
}

void clean_up() 
{   
    pid_t pid;
    
    pid = fork();
    
    if (pid == 0)
    {
        printf("Deleting temp image files from pwd.\n");
            execv(RM_PATH, "-rf *.bmp");
    }
    else
    {
        //tested as working 
        printf("Deleting temp text files from pwd.\n);
            execv(RM_PATH, "-rf x00.txt x01.txt x02.txt x03.txt x04.txt x05.txt x06.txt x07.txt");
    }
}

void encode_decode (int mode, char **argv)
{  
    if(mode){        
        char instructions[] = { argv[2], argv[3], argv[4] };
        
            //spin off NUM_THREADS encode threads
            for(int i = 0; i < NUM_Threads; i++)   
                pthread_create(thread_store[i], NULL, *encode(), instructions);
            
            //join threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_join(thread_store[i], NULL);
        
        //TO DO fork then exec ffmpeg to join files back into a single ouput video (ensure same size and frame rate as source, use lossless codec)
        //put ffmpeg arguments here, theyre pretty long and detailed    
        join_ffmpeg();
        
    } else {       
        char instructions[] = {argv[2], argv[3]}

            //spin off NUM_THREADS decode threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_create(thread_store[i], NULL, *decode(), instructions);
        
            //join threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_join(thread_store[i], NULL);

        //TO DO should output a single text file, figure out how to join multiple txt files 
        //(mutex that doesnt ruin parrallelization?)
    }
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
    
    //TO DO POSSIBLE use ffprobe to populate file data and control variables (size of txt each BMP can store, etc) 
    //(requires parsing JSON or CSV)
    
    //parse_video_info(); 
    
    //TO DO fork then exec ffmpeg to split up source file into component bmp files, wait for completion then encode/decode 
    fork();
    pid = fork();
    
    if (pid == 0)
        split_ffmpeg(argv[1], mode);
     
    else if(pid > 0)
    {
        pid = fork();
        
        if (pid == 0)
            split_text(argv[1], mode);
        
        else
            encode_decode(mode, argv);
    }
    
    //delete the temp bmp files from directory
    clean_up();
    
    return EXIT_SUCCESS;
}