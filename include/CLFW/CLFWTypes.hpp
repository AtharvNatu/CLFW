#pragma once

#include "CLFWMacros.hpp"

// Platform Headers
#if (CLFW_OS == 1)
    #include <CL/opencl.h>
    #pragma comment(lib, "OpenCL.lib")
#elif (CLFW_OS == 2)
    #include <CL/opencl.h>
#elif (CLFW_OS == 3)
    #include <OpenCL/opencl.h>
#endif

class CLFWTypes
{
    public:

    // Member Variables
    int int_data;
    float float_data;
    char char_data;
    unsigned char uchar_data;
    cl_mem ocl_buffer;
};
