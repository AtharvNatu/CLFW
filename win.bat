@echo off

cls

@REM OpenCL Windows

cd "./bin/"

cl.exe /c /EHsc /std:c++20 /I "C:\KhronosOpenCL\include" "../src/CLFW/CLFW.cpp" "../src/CLFW/Logger.cpp" "../test/VecAdd.cpp"

link.exe VecAdd.obj CLFW.obj Logger.obj /LIBPATH:"C:\KhronosOpenCL\lib"

@copy VecAdd.exe "../" > nul

cd ../

cls

VecAdd.exe

