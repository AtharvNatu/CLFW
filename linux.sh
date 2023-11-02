#!/bin/bash
set -e

clear

cd ./bin/

# Uses Rocm 5.7.0
g++ -Wall -c ../src/CLFW/*.cpp ../test/*.cpp -I "/opt/rocm-5.7.1/include/"

g++ -o VecAdd *.o -L "/opt/rocm-5.7.1/lib/" -lOpenCL -lm

# cp VecAdd ../

# cd ..

# clear

# ./VecAdd

