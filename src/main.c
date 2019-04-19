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

#define NUM_THREADS 16

#define FFMPEG_PATH "/usr/bin/ffmpeg"
#define CAT_PATH "/bin/cat"
#define RM_PATH "/bin/rm"

pthread_t thread_store[NUM_THREADS];

//hardcoded to default values pending change by parsing function
int num_frames = 14315;
int chars_in_text = 399122;
int chars_per_frame = 25000;

char *output_store[] = {"x00.txt", "x01.txt", "x02.txt", "x03.txt", "x04.txt", "x05.txt", "x06.txt", "x07.txt",
		        "x08.txt", "x09.txt", "x10.txt", "x11.txt", "x12.txt", "x13.txt", "x14.txt", "x15.txt"};

char *frame_store[] = { "frame000000001.bmp", "frame000000002.bmp", "frame000000003.bmp", "frame000000004.bmp", 
                        "frame000000005.bmp", "frame000000006.bmp", "frame000000007.bmp", "frame000000008.bmp",
		        "frame000000009.bmp", "frame000000010.bmp", "frame000000011.bmp", "frame000000012.bmp", 
                        "frame000000013.bmp", "frame000000014.bmp", "frame000000015.bmp", "frame000000016.bmp"};

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
    printf("Splitting source text into multiple files.\n");
  
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

      char *text_store = malloc ( chars_per_frame * sizeof(char) );

      for(int j = 0; j < chars_per_frame; j++)
      	text_store[j] = fgetc(source_file);

	
      fprintf(text_files[i], text_store);

      fclose (text_files[i]);
      free(text_store);	
    }
    fclose(source_file);
}

void join_text(char *output_text)
{
    printf("Joining source text from multiple output files.\n");
  
    FILE *text_files[NUM_THREADS];
    FILE *source_file = fopen(output_text, "w");

    if (source_file == NULL)
    {
      printf("Problem with initial split text file read.");
      exit(EXIT_FAILURE);
    }

    char *text_store = malloc ( chars_per_frame * NUM_THREADS * sizeof(char) );
  
    for(int i = 0; i < NUM_THREADS; i++)
    {
      text_files[i] = fopen(output_store[i], "r");

      for(int j = 0; j < chars_per_frame; j++)
      	text_store[j + (chars_per_frame * i)] = fgetc(source_file);

      fclose (text_files[i]);   
    }
    fprintf(source_file, text_store);
    free(text_store);	
    fclose(source_file);           
}

/*
Splits the video file into bmp files
*/
void split_ffmpeg (char *video)   
{
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
    {
      printf("Parsing video into frames, this may take a while.\n");
    
      char *instructions[] = {"ffmpeg", "-i", video, "frame%09d.bmp", NULL };
      execvp(FFMPEG_PATH, instructions);
    }
}

/*
Joins the files into a single avi file
TO DO allow for custom inputs / modify fps, resolution, pixel format?
*/
void join_ffmpeg (char *output_video)   
{
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
    {
      printf("Joining frames into encoded video. Time to grab some tea.\n");
    
      char *instructions[] = {"ffmpeg", "-r", "25", "-s", "1280x720", "-i", "frame%09d.bmp", "-c:a", "copy", "-crf", "0", output_video, NULL };
      execvp(FFMPEG_PATH, instructions);
    }
}

void clean_up() 
{   
  printf("Deleting working frames from pwd.\n");
  system("rm frame*");
  
  printf("Deleting temporary text files from pwd.\n");
  system("rm x*");
}

void encode_decode (int mode, char **argv)
{  
    if(mode == 1){        
        
	split_text(argv[2]);    
	    
        split_ffmpeg(argv[3]);
      
        thread_data_t thr_data[NUM_THREADS];
        
            //spin off NUM_THREADS encode frames
            for(int i = 0; i < NUM_THREADS; i++)
            {
              thr_data[i].text_source = output_store[i]; 
              thr_data[i].original_image = frame_store[i]; 
              thr_data[i].destination_image = frame_store[i]; 
              pthread_create(&thread_store[i], NULL, (void*) encode, &thr_data[i]);
            }
      
            //join threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_join(thread_store[i], NULL);
        
        //TO DO fork then exec ffmpeg to join files back into a single ouput video (ensure same size and frame rate as source, use lossless codec)
        //put ffmpeg arguments here, theyre pretty long and detailed    
        
        join_ffmpeg(argv[4]);
        
    } else {       
        
        split_ffmpeg(argv[2]);
        
        thread_data_t thr_data[NUM_THREADS];
      
            //spin off NUM_THREADS decode frames
            for (int i = 0; i < NUM_THREADS; i++)
            {
              //creates a number of text files to store data into then closes them
              
              thr_data[i].image_source = frame_store[i];
              thr_data[i].text_destination = output_store[i];
              pthread_create(&thread_store[i], NULL, (void*) decode, &thr_data[i]);
            }

            //join threads
            for (int i = 0; i < NUM_THREADS; i++)
                pthread_join(thread_store[i], NULL);

        //TO DO should output a single text file, figure out how to join multiple txt files 
        //(mutex that doesnt ruin parrallelization?)
        join_text(argv[3]);
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
 
    encode_decode(mode, argv);  
  
    //delete the temp bmp files from directory
    //clean_up();
    
    return EXIT_SUCCESS;
}
