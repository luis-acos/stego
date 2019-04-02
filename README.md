# Simple Image Steganography using LSB encoding

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
./isteg [-e] <text file to encode> <source bitmap> <destination bitmap>
./isteg [-d] <encoded bitmap> <decoded file>

-e : Encode text in image
-d : Decode text from image
~~~~
