# WasmEdge_OpenCV
This repo contains the c++ source file for a basic openCV console application.

##Overview
The application takes two images ecah containing a single face and does the following:
a. resizes the two faces to the greater of the two faces;
a. creates a merged face with whose left half is the left half of the first image, and whose right half is the right half of the second image;
c. sharpens the merged face;
d. changes the merged face to grayscale.

##Usage
The application takes 3 arguments: 
1. a path to the first image;
2. a path to the second image; 
3. a desired name for the output image (name without an extension, the output is generated as ".png" file).

Execute with the following command:
```
 ./face_merger <first_image_path> <second_image_path> <output_file_name> 
```

#HOW I WENT ABOUT IT(wsl UBUNTU):
##Setup 
1. I made sure I had the build packages/openCV dependencies: cmake, ninja, g++:
```
sudo apt update
sudo apt-get install cmake g++
sudo apt-get install ninja-build
```

2. I made sure to get the OpenCV development package:
```
sudo apt install libopencv-dev
```

3. I wrote the cpp script, starting with a basic one that simply draws green grid lines on an image, I modified, debugged, and added more functionalities.
(the path of "haarcascade_frontalface_alt.xml" facial detector was found my manual inspection of my device)

4. I created the CMake script.
##Build
5. I built:
```
mkdir build && cd build
cmake -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja
```
##Running
6. I tested the executable like so: 
```
./face_merger ~/Josh_P.jpg ~/B_Keith.jpg our_faces 
```
 

 
