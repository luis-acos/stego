# Simple Image Steganography using LSB encoding
-Now updated with maybe video steg!

Usage Notes 

  -Currently using 24 fps 854x480 source file (https://download.blender.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_stereo.avi);
  plan is to work with these specs in mind, add versatility as addititional feature 

## To Compile

~~~~
mkdir build
cd build
cmake ..
cmake --build .
~~~~

## Usage

~~~~
*** Image Steganography by LSB substitution ***

Usage:  
./vsteg [-e] <text file to encode> <source video> <destination video>
./vsteg [-d] <encoded video> <decoded file>

-e : Encode text in image
-d : Decode text from image
~~~~

### Helper Method Signatures 

~~~
bool encode(char *text_source, char *original_image, char *destination_image)

bool decode(char *image_source, char *text_destination)
~~~

