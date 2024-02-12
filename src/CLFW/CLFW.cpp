#include "../../include/CLFW/CLFW.hpp"

CLFW::CLFW(void)
{
    // Code
    clfwTypes = new CLFWTypes();
}

void CLFW::initialize(void)
{
    // Code
    this->oclGetPlatforms();
    std::cout << std::endl << "Select Preferred Platform : ";
    std::cin >> user_platform;
    this->oclSetPlatform(user_platform);

    this->oclGetDevices();
    std::cout << std::endl << "Select Preferred Device : ";
    std::cin >> user_device;
    this->oclSetDevice(user_device);

    this->oclGetDeviceProperties();

    this->oclCreateContext();

    this->oclCreateCommandQueue();
}

void CLFW::oclExecStatus(cl_int ocl_param)
{
    // Code
    if (ocl_param != CL_SUCCESS)
    {
        std::cerr << std::endl << "CLFW Error :  " << oclGetErrorString(ocl_param) << std::endl;
        this->uninitialize();
        exit(CLFW_FAILURE);
    }
}

std::string CLFW::oclGetErrorString(cl_int error)
{
    // Code
    switch(error)
    {
        // Run-time and JIT Errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // Compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_hostPtr";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // Extension Errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";

        default: 
        return "Unknown OpenCL error !!!";
    }
}

void CLFW::oclGetPlatforms(void)
{
    // Code
    oclExecStatus(clGetPlatformIDs(0, NULL, &oclNumPlatforms));
    
    oclPlatforms = (cl_platform_id*)malloc(oclNumPlatforms * sizeof(cl_platform_id));
    if (oclPlatforms == NULL)
    {
        std::cerr << std::endl << "CLFW Error :  Failed To Allocate Memory To oclPlatforms ... Exiting !!!" << std::endl;
        this->uninitialize();
        exit(CLFW_FAILURE);
    }

    oclExecStatus(clGetPlatformIDs(oclNumPlatforms, oclPlatforms, NULL));

    if (oclNumPlatforms == 0)
    {
        std::cerr << std::endl << "CLFW Error :  No OpenCL Supported Platforms Found ... Exiting !!!" << std::endl;
        this->uninitialize();
        exit(CLFW_FAILURE);
    }
    else
    {
        if (DEBUG)
        {
            std::cout << std::endl << "OPENCL PLATFORMS" << std::endl;
            std::cout << "-------------------------------------------------------------";
            for (int i = 0; i < (int)oclNumPlatforms; i++)
            {
                oclExecStatus(clGetPlatformInfo(oclPlatforms[i], CL_PLATFORM_NAME, 500, &oclPlatformInfo, NULL));
                std::cout << std::endl << "Platform " << i + 1 << " : " << oclPlatformInfo;
            }
            std::cout << std::endl << "-------------------------------------------------------------" << std::endl;
        }
    }
}

void CLFW::oclSetPlatform(int platform)
{
    if (platform <= (int)oclNumPlatforms)
        oclPlatformId = oclPlatforms[platform - 1];
    else
    {
        std::cerr << std::endl << "CLFW Error : Please Select Valid OpenCL Platform ... Exiting !!!" << std::endl;
        this->uninitialize();
        exit(CLFW_FAILURE);
    }
}

void CLFW::oclGetDevices(void)
{
    // Code
    if (oclNumPlatforms > 0)
    {
        oclExecStatus(clGetDeviceIDs(
            oclPlatformId,
            CL_DEVICE_TYPE_GPU,
            0,
            NULL,
            &oclNumDevices
        ));

        oclDevices = (cl_device_id*)malloc(oclNumDevices * sizeof(cl_device_id));
        if (oclDevices == NULL)
        {
            std::cerr << std::endl << "CLFW Error :  Failed To Allocate Memory To oclPlatforms ... Exiting !!!" << std::endl;
            this->uninitialize();
            exit(CLFW_FAILURE);
        }

        oclExecStatus(clGetDeviceIDs(
            oclPlatformId,
            CL_DEVICE_TYPE_GPU,
            oclNumDevices,
            oclDevices,
            NULL
        ));

        if (oclNumDevices == 0)
        {
            std::cerr << std::endl << "CLFW Error :  No OpenCL Supported Devices Found ... Exiting !!!" << std::endl;
            this->uninitialize();
            exit(CLFW_FAILURE);
        }
        else
        {
            std::cout << std::endl << "OPENCL DEVICES" << std::endl;
            std::cout << "-------------------------------------------------------------";
            for (int i = 0; i < (int)oclNumDevices; i++)
            {
                clGetDeviceInfo(oclDevices[i], CL_DEVICE_NAME, sizeof(oclDevProp), &oclDevProp, NULL);
                std::cout << std::endl << "Device " << i + 1 << " : " << oclDevProp;
            }
            std::cout << std::endl << "-------------------------------------------------------------" << std::endl;
        }
    }
}

void CLFW::oclSetDevice(int device)
{
    // Code
    if (device <= (int)oclNumDevices)
        oclDeviceId = oclDevices[device - 1];
    else
    {
        std::cerr << std::endl << "CLFW Error : Please Select Valid OpenCL Device ... Exiting !!!" << std::endl;
        this->uninitialize();
        exit(CLFW_FAILURE);
    }
}

void CLFW::oclGetDeviceProperties(void)
{   
    // Code
    if (DEBUG)
    {
        std::cout << std::endl << "DEVICE PROPERTIES" << std::endl;
        std::cout << "-------------------------------------------------------------";
        oclExecStatus(clGetDeviceInfo(
            oclDeviceId,
            CL_DEVICE_NAME,
            sizeof(oclDevProp),
            &oclDevProp,
            NULL
        ));
        std::cout << std::endl << "GPU Device Name : " << oclDevProp << std::endl;

        oclExecStatus(clGetDeviceInfo(
            oclDeviceId,
            CL_DEVICE_VENDOR,
            sizeof(oclDevProp),
            &oclDevProp,
            NULL
        ));
        std::cout << std::endl << "GPU Device Vendor : " << oclDevProp << std::endl;

        oclExecStatus(clGetDeviceInfo(
            oclDeviceId,
            CL_DEVICE_VERSION,
            sizeof(oclDevProp),
            &oclDevProp,
            NULL
        ));
        std::cout << std::endl << "GPU Device Version : " << oclDevProp << std::endl;

        oclExecStatus(clGetDeviceInfo(
            oclDeviceId,
            CL_DEVICE_GLOBAL_MEM_SIZE,
            sizeof(oclMemSize),
            &oclMemSize,
            NULL
        ));
        std::cout << std::endl << "GPU Memory : " << (unsigned long long) oclMemSize / 1000000000 << " GB" << std::endl;

        oclExecStatus(clGetDeviceInfo(
            oclDeviceId,
            CL_DEVICE_MAX_COMPUTE_UNITS,
            sizeof(oclComputeUnits),
            &oclComputeUnits,
            NULL
        ));
        std::cout << std::endl << "GPU Compute Units : " << oclComputeUnits << std::endl;

        std::cout << "-------------------------------------------------------------" << std::endl;
    }
}

void CLFW::oclCreateContext(void)
{
    // Code
    oclContext = clCreateContext(
        NULL,
        1,
        &oclDeviceId,
        NULL,
        NULL,
        &oclResult
    );
    oclExecStatus(oclResult);
}

void CLFW::oclCreateCommandQueue(void)
{
    // Code
    oclCommandQueue = clCreateCommandQueue(
        oclContext,
        oclDeviceId,
        CL_QUEUE_PROFILING_ENABLE,
        &oclResult     
    );
    oclExecStatus(oclResult);
}

const char *CLFW::oclReadKernelFile(const char *oclKernelFile)
{
    // Variable Declarations
    char ch;
    int i = 0;

    // Code
    if (oclKernelFile == NULL)
        return NULL;

    FILE *oclKernel_fp = NULL;

    #if (CLFW_OS == 1)
        fopen_s(&oclKernel_fp, oclKernelFile, "r");
    #else
        oclKernel_fp = fopen(oclKernelFile, "r");
    #endif

    if (oclKernel_fp == NULL)
    {
        if (DEBUG)
        {
            std::cerr << std::endl << "CLFW Error :  Failed To Open OpenCL Kernel File " << oclKernelFile << " ... Exiting !!!" << std::endl;
            return NULL;
        }      
    }

    int file_length = std::filesystem::file_size(oclKernelFile);

    char *source_code = (char *)calloc(1, file_length + 1);
    if (source_code == NULL)
    {
        if (DEBUG)
        {
            std::cerr << std::endl << "CLFW Error :  Failed To Allocate Memory To OpenCL Kernel Source Code ... Exiting !!!" << std::endl;
            return NULL;
        }
    }

    while ((ch = fgetc(oclKernel_fp)) != EOF)
        source_code[i++] = ch;
    *(source_code + i) = '\0';

    fclose(oclKernel_fp);
    oclKernel_fp = NULL;

    return (const char *)source_code;
}

void CLFW::oclCreateProgram(const char* oclKernelFile)
{
    // Code
    oclKernelSourceCode = oclReadKernelFile(oclKernelFile);
    if (oclKernelSourceCode == NULL)
    {
        if (DEBUG)
            std::cerr << std::endl << "CLFW Error :  Failed To Read OpenCL Kernel From File : " << oclKernelFile << std::endl;
        this->uninitialize();
        exit(CLFW_FAILURE); 
    }

    size_t ocl_source_code_size = strlen(oclKernelSourceCode) + 1;

    oclProgram = clCreateProgramWithSource(
        oclContext,
        1,
        (const char**)&oclKernelSourceCode,
        &ocl_source_code_size,
        &oclResult
    );
    oclExecStatus(oclResult);

    // Release
    free((void*)oclKernelSourceCode);
    oclKernelSourceCode = NULL;

    oclResult = clBuildProgram(
        oclProgram,
        0,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (oclResult != CL_SUCCESS)
    {
        size_t length;
        char *buffer = NULL;

        clGetProgramBuildInfo(
            oclProgram,
            oclDeviceId,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &length
        );

        buffer = (char*)malloc(length);
        if (buffer)
        {
            clGetProgramBuildInfo(
                oclProgram,
                oclDeviceId,
                CL_PROGRAM_BUILD_LOG,
                length,
                buffer,
                NULL
            );

            if (DEBUG)
                std::cerr << std::endl << "CLFW Error :  OpenCL Program Build Log : " << buffer << std::endl;

            free(buffer);
            buffer = NULL;
            this->uninitialize();
            exit(CLFW_FAILURE);
        }
    }
}

void CLFW::oclCreateKernel(const char* oclKernelName, const char* oclKernelArgTypes, ...)
{
    // Variable Declarations
    va_list kernel_args_list;

    // Code
    oclKernel = clCreateKernel(oclProgram, oclKernelName, &oclResult);
    oclExecStatus(oclResult);

    va_start(kernel_args_list, oclKernelArgTypes);

    for (auto i = 0; oclKernelArgTypes[i] != '\0'; i++)
    {
        switch(oclKernelArgTypes[i])
        {
            case 'b':
                clfwTypes->oclBuffer = va_arg(kernel_args_list, cl_mem);
                oclExecStatus(clSetKernelArg(oclKernel, i, sizeof(cl_mem), (void*)&clfwTypes->oclBuffer));
            break;

            case 'i':
                clfwTypes->intData = va_arg(kernel_args_list, cl_int);
                oclExecStatus(clSetKernelArg(oclKernel, i, sizeof(cl_int), (void*)&clfwTypes->intData));
            break;

            case 'f':
                clfwTypes->floatData = va_arg(kernel_args_list, cl_double);
                oclExecStatus(clSetKernelArg(oclKernel, i, sizeof(cl_float), (void*)&clfwTypes->floatData));
            break;

            case 'd':
                clfwTypes->floatData = va_arg(kernel_args_list, cl_double);
                oclExecStatus(clSetKernelArg(oclKernel, i, sizeof(cl_double), (void*)&clfwTypes->doubleData));
            break;

            case 'c':
                clfwTypes->charData = va_arg(kernel_args_list, cl_int);
                oclExecStatus(clSetKernelArg(oclKernel, i, sizeof(cl_char), (void*)&clfwTypes->charData));
            break;

            case 'u':
                clfwTypes->ucharData = va_arg(kernel_args_list, cl_uint);
                oclExecStatus(clSetKernelArg(oclKernel, i, sizeof(cl_uchar), (void*)&clfwTypes->ucharData));
            break;
        }
    }

    va_end(kernel_args_list);
}

double CLFW::oclExecuteKernel(size_t oclGlobalWorkSize, size_t oclLocalWorkSize)
{
    // Code
    oclExecStatus(clEnqueueNDRangeKernel(
        oclCommandQueue,
        oclKernel,
        1,
        NULL,
        &oclGlobalWorkSize,
        &oclLocalWorkSize,
        0,
        NULL,
        &timeEvent
    ));

    oclExecStatus(clFinish(oclCommandQueue));

    oclExecStatus(clGetEventProfilingInfo(
        timeEvent,
        CL_PROFILING_COMMAND_START,
        sizeof(startTime),
        &startTime,
        NULL
    ));

    oclExecStatus(clGetEventProfilingInfo(
        timeEvent,
        CL_PROFILING_COMMAND_END,
        sizeof(endTime),
        &endTime,
        NULL
    ));

    double gpuTime = (double) (endTime - startTime) * 1.0e-6;

    return gpuTime;

}

cl_mem CLFW::oclCreateBuffer(int flag, size_t oclDataSize)
{
    oclBuffer = clCreateBuffer(
        oclContext,
        flag,
        oclDataSize,
        NULL,
        &oclResult
    );
    
    oclExecStatus(oclResult);

    return oclBuffer;
}

void CLFW::oclWriteBuffer(cl_mem oclDataBuffer, size_t oclDataSize, void* hostPtr)
{
    // Code
    oclExecStatus(clEnqueueWriteBuffer(
        oclCommandQueue,
        oclDataBuffer,
        CL_FALSE,
        0,
        oclDataSize,
        hostPtr,
        0,
        NULL,
        NULL
    ));
}

void CLFW::oclReadBuffer(cl_mem oclDataBuffer, size_t oclDataSize, void* hostPtr)
{
    // Code
    oclExecStatus(clEnqueueReadBuffer(
        oclCommandQueue,
        oclDataBuffer,
        CL_TRUE,
        0,
        oclDataSize,
        hostPtr,
        0,
        NULL,
        NULL
    ));
}

void CLFW::oclReleaseBuffer(cl_mem oclDataBuffer)
{
    if (oclDataBuffer)
    {
        clReleaseMemObject(oclDataBuffer);
        oclDataBuffer = NULL;
    }
}

void CLFW::hostMemAlloc(void** hostPtr, std::string hostType, size_t hostSize)
{
    if (hostType == "int")
        *hostPtr = new int[hostSize];
    else if (hostType == "float")
        *hostPtr = new float[hostSize];
    else if (hostType == "double")
        *hostPtr = new double[hostSize];
    else if (hostType == "char")
        *hostPtr = new char[hostSize];
    else if (hostType == "uchar")
        *hostPtr = new unsigned char[hostSize];

    if (hostPtr == NULL)
    {
        if (DEBUG)
            std::cerr << std::endl << "Failed To Allocate Memory To : " << *hostPtr << std::endl;

        exit(EXIT_FAILURE);
    }

    memset(*hostPtr, 0, hostSize);
}

void CLFW::hostMemFree(float** hostPtr)
{
    if (*hostPtr)
    {
        delete[] *hostPtr;
        *hostPtr = nullptr;
    }
}

void CLFW::uninitialize(void)
{
    // Code
    if (oclKernel)
    {
        clReleaseKernel(oclKernel);
        oclKernel = nullptr;
    }

    if (oclProgram)
    {
        clReleaseProgram(oclProgram);
        oclProgram = nullptr;
    }

    if (oclCommandQueue)
    {
        clFlush(oclCommandQueue);
        clReleaseCommandQueue(oclCommandQueue);
        oclCommandQueue = nullptr;
    }

    if (oclContext)
    {
        clReleaseContext(oclContext);
        oclContext = nullptr;
    }

    if (oclDevices)
    {
        free(oclDevices);
        oclDevices = NULL;
    }

    if (oclPlatforms)
    {
        free(oclPlatforms);
        oclPlatforms = NULL;
    }
}

CLFW::~CLFW()
{
    // Code
    delete clfwTypes;
    clfwTypes = nullptr;
}
