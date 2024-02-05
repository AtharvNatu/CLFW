set -e

clear

cd ./bin/

clang++ -Wall -o VecAdd ../src/CLFW/*.cpp ../test/*.cpp -std=c++20 -framework OpenCL

cp VecAdd ../

cd ..

clear

./VecAdd
