# Simple Image Steganography using LSB encoding
-Now updated with video steg!

Usage Notes 

  -Currently using 24 fps 854x480 video source file included in the build/bin directory;
  Plan is to work with these specs in mind, add versatility as addititional feature 

## To Compile

~~~~
mkdir build
cd build
cmake ..
make
~~~~

## Usage

~~~~
*** Image Steganography by LSB substitution ***

cd bin

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

