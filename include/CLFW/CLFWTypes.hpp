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
        cl_int intData;
        cl_float floatData;
        cl_double doubleData;
        cl_char charData;
        cl_uchar ucharData;
        cl_mem oclBuffer;
};
