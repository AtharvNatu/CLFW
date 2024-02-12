#!/bin/bash
set -e

clear

cd ./bin/

# Uses AMD ROCm
g++ -Wall -c ../src/CLFW/*.cpp ../test/*.cpp -I "/opt/rocm/include/"

g++ -o VecAdd *.o -L "/opt/rocm/lib/" -lOpenCL -lm

cp VecAdd ../

cd ..

clear

./VecAdd

#? Testing Memory Leaks - -fsanitize=address
# g++ -fsanitize=address -g -Wall -c ../src/CLFW/*.cpp ../test/*.cpp -I "/opt/rocm/include/"
# g++ -fsanitize=address -g -o VecAdd *.o -L "/opt/rocm/lib/" -lOpenCL -lm
# # valgrind --leak-check=full ./VecAdd




