//
// Created by Miguel Alonso Jr on 2019-02-28.
//
#include <stdlib.h>
#include "bitmap.h"
#include "util.h"

/* Function to read a bitmap struct 
Opens (image) file and allocates memory to hold bitmap struct.
Reads the header ? find out why header is important?
Reads the bitmap struct header variable with the info in the file using an fread
Allocates the memory for the data variable of the bitmap struct 
Reads the bitmap struct data variable with the info in the file using an fread
returns the image bitmap struct 
*/
Bitmap *read_bitmap(char *filename, char **error) {
    // open the file for reading as binary
    FILE *file = _open_file(filename, "rb");
    Bitmap *image = malloc(sizeof(*image));
    //TODO: Check that memory was allocated properly
    rewind(file);

    // Read bitmap header
    int num_read = (int) fread(&image->header, sizeof(image->header), 1, file);
    //TODO: check that header was read properly

    //TODO: check that read header matches file

    //Allocate memory for the bitmap data
    image->data = malloc(sizeof(*image->data) * image->header.image_size_bytes);
    //TODO: check that memory was allocated properly

    //Read bitmap image data
    num_read = (int) fread(image->data, image->header.image_size_bytes, 1, file);
    //TODO: check that bitmap image data was read properly

    return image;
}

/*
Opens (image file)
Writes the image bitmap struct field with the file header
ditto for the data
*/
bool write_bitmap(const char *filename, Bitmap *image, char **error) {
    // open the file for writing as binary
    FILE *file = _open_file(filename, "wb");
    rewind(file);

    // write the bitmap header
    int num_read = (int) fwrite(&image->header, sizeof(image->header), 1, file);
    // TODO: check that header was written properly

    // write the bitmap image data
    num_read = (int) fwrite(image->data, image->header.image_size_bytes, 2764854, file);
    //TODO: check that image data was written properly

    return true;
}

// no sure but looks like TODO
bool check_bitmap_header(BitmapHeader header, char *filename) {
    return true;
}
