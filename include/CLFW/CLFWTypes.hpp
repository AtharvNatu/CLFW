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
    cl_int int_data;
    cl_float float_data;
    cl_char char_data;
    cl_uchar uchar_data;
    cl_mem ocl_buffer;
};
