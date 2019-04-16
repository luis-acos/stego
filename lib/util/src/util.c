//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#include <stdlib.h>
#include "util.h"

/*
Accepts a file path and mode (r - read / w - write) and attempts to open the file using fopen
Outputs error and writes propmt into file if fopen was not successful 
Returns a file pointer to the opened file
*/

FILE *_open_file(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %i", filename);

        exit(EXIT_FAILURE);
    }
    return fp;
}

/*
Outputs pointer to argument error and frees up the image memory by calling _clean_up
*/
void _handle_error(char **error, FILE *fp, Bitmap *image) {
    fprintf(stderr, "ERROR: %s\n", *error);
    _clean_up(fp, image, error);

    exit(EXIT_FAILURE);
}

//POSSIBLE TO DO handle error on video file 
void _handle_error_video(char **error, FILE *fp, Bitmap *image) {}

//frees up the memory in the bitmap struct and the passed along error 
void _clean_up(FILE *fp, Bitmap *image, char **error) {
    if (fp != NULL) {
        fclose(fp);
    }
    _free_bmp(image);
    free(*error);
}

//frees up the memory contained in the bitmap passed along 
void _free_bmp(Bitmap *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}

//POSSIBLE TO DO free up memory in the video passed along
void _free_video(){}

/*
Accepts a file and scans file till SEEK_END for the length of the text
    then returns file pointer to the start with SEEK_SET
Returns size of the file as a long 
*/
long _get_text_length(FILE *filename) {
    fseek(filename, 0L, SEEK_END);
    long size = ftell(filename) + 1;
    fseek(filename, 0L, SEEK_SET);
    return (size);
}
