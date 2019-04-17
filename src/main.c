#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bitmap.h"
#include "stego.h"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_THREADS 8

#define FFMPEG_PATH "/usr/bin/ffmpeg"
#define SPLIT_PATH "usr/bin/split"
#define CAT_PATH "/bin/cat"
#define RM_PATH "/bin/rm"

pthread_t thread_store[NUM_THREADS];

//hardcoded to default values pending change by parsing function
int num_frames = 14315;
int chars_in_text = 399122;
int chars_per_frame = 51240;

char *output_store[] = {"x00.sws", "x01.sws", "x02.sws", "x03.sws", "x04.sws", "x05.sws", "x06.sws", "x07.sws"};

char *frame_store[] = { "frame000000001.bmp", "frame000000002.bmp", "frame000000003.bmp", "frame000000004.bmp", 
                                "frame000000005.bmp", "frame000000006.bmp", "frame000000007.bmp", "frame000000008.bmp"};

int frames_to_encode = NUM_THREADS; 

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
void parse_video_info(char *video) {}

/*
Splits the given text file into multiple char arrays for passing to encode/decode threads 
*/
void split_text(char *text)
{
    FILE *text_files[NUM_THREADS];
    FILE *source_file = fopen(text, "r");

    if (source_file == NULL)
    {
      printf("Problem with initial split text file read.");
      exit(EXIT_FAILURE);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
      text_files[i] = fopen(output_store[i], "w");

      for(int j = 0; j < chars_per_frame; j++)
          fputc ( fgetc(source_file), text_files[i] );

      fclose (text_files[i]);
    }
}

void join_text(char *output_text)
{
    printf ("Joining source text files into single output text file.");
  
    //test pending
    char *instructions[] = {"cat", "x00.sws", "x01.sws", "x02.sws", "x03.sws", "x04.sws", "x05.sws", 
                            "x06.sws", "x07.sws", ">", output_text, NULL};     
    execvp(CAT_PATH, instructions);
}

/*
Splits the video file into bmp files
*/
void split_ffmpeg (char *video)   
{
    printf("Parsing video into frames, this may take a while.\n");
    
    char *instructions[] = {"ffmpeg", "-i", video, "frame%09d.bmp", NULL };
    execvp(FFMPEG_PATH, instructions);
}

/*
Joins the files into a single avi file
TO DO allow for custom inputs / modify fps, resolution, pixel format?
*/
void join_ffmpeg (char *output_video)   
{
    printf("Joining frames into encoded video. Time to grab some tea.\n");
    
    char *instructions[] = {"ffmpeg", "-r", "24", "-s", "854x480", "-i", 
                            "frame%09d.bmp", "-vcodec", "ffv1", "-crf", "25", output_video, NULL };
    execvp(FFMPEG_PATH, instructions);
}

void clean_up() 
{   
  system("rm frame*");
  
  system("rm x*");
}

void encode_decode (int mode, char **argv)
{  
    if(mode){        
        char *instructions[3];
        
            //spin off NUM_THREADS encode frames
            for(int i = 0; i < NUM_THREADS; i++)
            {
              instructions[0] = output_store[i]; 
              instructions[1] = frame_store[i]; 
              instructions[2] = frame_store[i]; 
              pthread_create(&thread_store[i], NULL, (void*) encode, &instructions);
            }
      
            //join threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_join(thread_store[i], NULL);
        
        //TO DO fork then exec ffmpeg to join files back into a single ouput video (ensure same size and frame rate as source, use lossless codec)
        //put ffmpeg arguments here, theyre pretty long and detailed    
        join_ffmpeg(argv[4]);
        
    } else {       
        char *instructions[2];
      
            //spin off NUM_THREADS decode frames
            for (int i = 0; i < NUM_THREADS; i++)
            {
              //creates a number of text files to store data into 
              fopen(output_store[i], "w");
              
              instructions[0] = frame_store[i];
              instructions[1] = output_store[i];
              pthread_create(&thread_store[i], NULL, (void*) decode, &instructions);
            }

            //join threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_join(thread_store[i], NULL);

        //TO DO should output a single text file, figure out how to join multiple txt files 
        //(mutex that doesnt ruin parrallelization?)
        join_text(argv[2]);
    }
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
    
    //TO DO POSSIBLE use ffprobe to populate file data and control variables (size of txt each BMP can store, etc) 
    //(requires parsing JSON or CSV file)
    //parse_video_info();  
  
    int ffmpeg_status;
  
    pid_t ffmpeg_pid = fork();
  
    if(ffmpeg_pid < 0)
    {
        printf("Failure with ffmpeg execv call. Program exiting");
        exit(1);
    }
    else if (ffmpeg_pid > 0)
    {
        wait(&ffmpeg_status);
    } 
    else
      split_ffmpeg(argv[3]);
  
    split_text(argv[2]);
 
    //encode_decode(mode, argv);  
  
    //delete the temp bmp files from directory
    clean_up();
    
    return EXIT_SUCCESS;
}
