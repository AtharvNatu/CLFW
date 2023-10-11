#!/bin/bash
set -e

clear

cd ./bin/

# Uses Rocm 5.7.0
g++ -w -c ../src/CLFW/*.cpp ../test/*.cpp -I "/opt/rocm-5.7.0/include/"

g++ -o VecAdd *.o -L "/opt/rocm-5.7.0/lib/" -lOpenCL -lm

cp VecAdd ../

cd ..

clear

./VecAdd

