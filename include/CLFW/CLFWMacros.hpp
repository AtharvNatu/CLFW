// CLFW Macros

// OpenCL Version 1.2 For API Backward Compatibility
#define CL_MINIMUM_OPENCL_VERSION   120
#define CL_TARGET_OPENCL_VERSION    120

#define CLFW_SUCCESS                EXIT_SUCCESS
#define CLFW_FAILURE                EXIT_FAILURE

#define DEBUG                       true

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    #define CLFW_OS 1
#elif defined(__linux__)
    #define CLFW_OS 2
#elif defined(__APPLE__)
    #define CLFW_OS 3
#endif
