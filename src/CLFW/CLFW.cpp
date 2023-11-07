#include "../../include/CLFW/CLFW.hpp"

void CLFW::initialize(void)
{
    // Code
    logger = new Logger();
    logger->initialize();

    clfwTypes = new CLFWTypes();

    ocl_get_platforms();

    if (DEBUG)
    {
        cout << endl << "Select Preferred Platform : ";
        cin >> user_platform;
    }
    ocl_set_platform(user_platform);

    ocl_get_devices();
    if (DEBUG)
    {
        cout << endl << "Select Preferred Device : ";
        cin >> user_device;
    }
    ocl_set_device(user_device);

    ocl_dev_properties();

    ocl_create_context();

    ocl_create_command_queue();
}

void CLFW::ocl_exec_status(cl_int ocl_param, int line_no)
{
    // Code
    if (DEBUG)
    {
        if (ocl_param != CL_SUCCESS)
        {
            cerr << endl << "CLFW Error :  " << ocl_get_error_string(ocl_param) << " At Line No. " << line_no << endl;
            this->uninitialize();
            exit(CLFW_FAILURE);
        }   
    }
}

string CLFW::ocl_get_error_string(cl_int error)
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
        case -37: return "CL_INVALID_HOST_PTR";
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

void CLFW::ocl_get_platforms(void)
{
    // Code
    ocl_exec_status(clGetPlatformIDs(0, NULL, &ocl_num_platforms), __LINE__);
    
    ocl_platforms = (cl_platform_id*)malloc(ocl_num_platforms * sizeof(cl_platform_id));
    if (ocl_platforms == NULL)
    {
        if (DEBUG)
            cerr << endl << "CLFW Error :  Failed To Allocate Memory To ocl_platforms ... Exiting !!!" << endl;
        else
            logger->print_log("\nCLFW Error :  Failed To Allocate Memory To ocl_platforms ... Exiting !!!\n");

        this->uninitialize();
        exit(CLFW_FAILURE);
    }

    ocl_exec_status(clGetPlatformIDs(ocl_num_platforms, ocl_platforms, NULL), __LINE__);

    if (ocl_num_platforms == 0)
    {
        if (DEBUG)
            cerr << endl << "CLFW Error :  No OpenCL Supported Platforms Found ... Exiting !!!" << endl;
        else
            logger->print_log("\nCLFW Error :  No OpenCL Supported Platforms Found ... Exiting !!!\n");

        this->uninitialize();
        exit(CLFW_FAILURE);
    }
    else
    {
        if (DEBUG)
        {
            cout << endl << "OPENCL PLATFORMS" << endl;
            cout << "-------------------------------------------------------------";
            for (int i = 0; i < (int)ocl_num_platforms; i++)
            {
                ocl_exec_status(clGetPlatformInfo(ocl_platforms[i], CL_PLATFORM_NAME, 500, &ocl_platform_info, NULL), __LINE__);
                cout << endl << "Platform " << i + 1 << " : " << ocl_platform_info;
            }
            cout << endl << "-------------------------------------------------------------" << endl;
        }
    }
}

void CLFW::ocl_set_platform(int platform)
{
    if (platform <= (int)ocl_num_platforms)
        ocl_platform_id = ocl_platforms[platform - 1];
    else
    {
        if (DEBUG)
            cerr << endl << "CLFW Error : Please Select Valid OpenCL Platform ... Exiting !!!" << endl;
        else
            logger->print_log("CLFW Error : Please Select Valid OpenCL Platform ... Exiting !!!");

        this->uninitialize();
        exit(CLFW_FAILURE);
    }
}

void CLFW::ocl_get_devices(void)
{
    // Code
    if (ocl_num_platforms > 0)
    {
        ocl_exec_status(clGetDeviceIDs(
            ocl_platform_id,
            CL_DEVICE_TYPE_GPU,
            0,
            NULL,
            &ocl_num_devices
        ), __LINE__);

        ocl_devices = (cl_device_id*)malloc(ocl_num_devices * sizeof(cl_device_id));
        if (ocl_devices == NULL)
        {
            if (DEBUG)
                cerr << endl << "CLFW Error :  Failed To Allocate Memory To ocl_platforms ... Exiting !!!" << endl;
            else
                logger->print_log("\nCLFW Error :  Failed To Allocate Memory To ocl_platforms ... Exiting !!!\n");
            this->uninitialize();
            exit(CLFW_FAILURE);
        }

        ocl_exec_status(clGetDeviceIDs(
            ocl_platform_id,
            CL_DEVICE_TYPE_GPU,
            ocl_num_devices,
            ocl_devices,
            NULL
        ), __LINE__);

        if (ocl_num_devices == 0)
        {
            if (DEBUG)
                cerr << endl << "CLFW Error :  No OpenCL Supported Devices Found ... Exiting !!!" << endl;
            else
                logger->print_log("\nCLFW Error :  Failed To Allocate Memory To ocl_platforms ... Exiting !!!\n");
            this->uninitialize();
            exit(CLFW_FAILURE);
        }
        else
        {
            if (DEBUG)
            {
                cout << endl << "OPENCL DEVICES" << endl;
                cout << "-------------------------------------------------------------";
                for (int i = 0; i < (int)ocl_num_devices; i++)
                {
                    clGetDeviceInfo(ocl_devices[i], CL_DEVICE_NAME, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);
                    cout << endl << "Device " << i + 1 << " : " << ocl_dev_prop;
                }
                cout << endl << "-------------------------------------------------------------" << endl;
            }
        }
    }
}

void CLFW::ocl_set_device(int device)
{
    // Code
    if (device <= (int)ocl_num_devices)
        ocl_device_id = ocl_devices[device - 1];
    else
    {
        if (DEBUG)
            cerr << endl << "CLFW Error : Please Select Valid OpenCL Device ... Exiting !!!" << endl;
        else
            logger->print_log("CLFW Error : Please Select Valid OpenCL Device ... Exiting !!!");

        this->uninitialize();
        exit(CLFW_FAILURE);
    }
}

void CLFW::ocl_dev_properties(void)
{   
    // Code
    if (DEBUG)
    {
        cout << endl << "DEVICE PROPERTIES" << endl;
        cout << "-------------------------------------------------------------";
        ocl_exec_status(clGetDeviceInfo(
            ocl_device_id,
            CL_DEVICE_NAME,
            sizeof(ocl_dev_prop),
            &ocl_dev_prop,
            NULL
        ), __LINE__);
        cout << endl << "GPU Device Name : " << ocl_dev_prop << endl;

        ocl_exec_status(clGetDeviceInfo(
            ocl_device_id,
            CL_DEVICE_VENDOR,
            sizeof(ocl_dev_prop),
            &ocl_dev_prop,
            NULL
        ), __LINE__);
        cout << endl << "GPU Device Vendor : " << ocl_dev_prop << endl;

        ocl_exec_status(clGetDeviceInfo(
            ocl_device_id,
            CL_DEVICE_VERSION,
            sizeof(ocl_dev_prop),
            &ocl_dev_prop,
            NULL
        ), __LINE__);
        cout << endl << "GPU Device Version : " << ocl_dev_prop << endl;

        ocl_exec_status(clGetDeviceInfo(
            ocl_device_id,
            CL_DEVICE_GLOBAL_MEM_SIZE,
            sizeof(ocl_mem_size),
            &ocl_mem_size,
            NULL
        ), __LINE__);
        cout << endl << "GPU Memory : " << (unsigned long long) ocl_mem_size / 1000000000 << " GB" << endl;

        ocl_exec_status(clGetDeviceInfo(
            ocl_device_id,
            CL_DEVICE_MAX_COMPUTE_UNITS,
            sizeof(ocl_compute_units),
            &ocl_compute_units,
            NULL
        ), __LINE__);
        cout << endl << "GPU Compute Units : " << ocl_compute_units << endl;

        cout << "-------------------------------------------------------------" << endl;
    }
}

void CLFW::ocl_create_context(void)
{
    // Code
    ocl_context = clCreateContext(
        NULL,
        1,
        &ocl_device_id,
        NULL,
        NULL,
        &ocl_result
    );
    ocl_exec_status(ocl_result, __LINE__);
}

void CLFW::ocl_create_command_queue(void)
{
    // Code
    ocl_command_queue = clCreateCommandQueue(
        ocl_context,
        ocl_device_id,
        0,
        &ocl_result     
    );
    ocl_exec_status(ocl_result, __LINE__);
}

const char *CLFW::ocl_read_kernel_from_file(const char *ocl_kernel_file)
{
    // Variable Declarations
    char ch;
    int i = 0;

    // Code
    if (ocl_kernel_file == NULL)
        return NULL;

    FILE *ocl_kernel_fp = NULL;

    #if (CLFW_OS == 1)
        fopen_s(&ocl_kernel_fp, ocl_kernel_file, "r");
    #else
        ocl_kernel_fp = fopen(ocl_kernel_file, "r");
    #endif

    if (ocl_kernel_fp == NULL)
    {
        if (DEBUG)
        {
            cerr << endl << "CLFW Error :  Failed To Open OpenCL Kernel File " << ocl_kernel_file << " ... Exiting !!!" << endl;
            return NULL;
        }
        else
        {
            logger->print_log("\nCLFW Error :  Failed To Open OpenCL Kernel File ", ocl_kernel_file, " ... Exiting !!!\n");
            return NULL;
        }          
    }

    int file_length = filesystem::file_size(ocl_kernel_file);

    char *source_code = (char *)calloc(1, file_length + 1);
    if (source_code == NULL)
    {
        if (DEBUG)
        {
            cerr << endl << "CLFW Error :  Failed To Allocate Memory To OpenCL Kernel Source Code ... Exiting !!!" << endl;
            return NULL;
        }
        else
        {
            logger->print_log("\nCLFW Error :  Failed To Allocate Memory To OpenCL Kernel Source Code ... Exiting !!!\n");
            return NULL;
        }
    }

    while ((ch = fgetc(ocl_kernel_fp)) != EOF)
        source_code[i++] = ch;
    *(source_code + i) = '\0';

    fclose(ocl_kernel_fp);
    ocl_kernel_fp = NULL;

    return (const char *)source_code;
}

void CLFW::ocl_create_program(const char* ocl_kernel_file)
{
    // Code
    ocl_kernel_source_code = ocl_read_kernel_from_file(ocl_kernel_file);
    if (ocl_kernel_source_code == NULL)
    {
        if (DEBUG)
            cerr << endl << "CLFW Error :  Failed To Read OpenCL Kernel From File : " << ocl_kernel_file << endl;
        else
            logger->print_log("\nCLFW Error :  Failed To Read OpenCL Kernel From File : ", ocl_kernel_file);

        this->uninitialize();
        exit(CLFW_FAILURE); 
    }

    size_t ocl_source_code_size = strlen(ocl_kernel_source_code) + 1;

    ocl_program = clCreateProgramWithSource(
        ocl_context,
        1,
        (const char**)&ocl_kernel_source_code,
        &ocl_source_code_size,
        &ocl_result
    );
    ocl_exec_status(ocl_result, __LINE__);

    // Release
    free((void*)ocl_kernel_source_code);
    ocl_kernel_source_code = NULL;

    ocl_result = clBuildProgram(
        ocl_program,
        0,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (ocl_result != CL_SUCCESS)
    {
        size_t length;
        char *buffer = NULL;

        clGetProgramBuildInfo(
            ocl_program,
            ocl_device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &length
        );

        buffer = (char*)malloc(length);
        if (buffer)
        {
            clGetProgramBuildInfo(
                ocl_program,
                ocl_device_id,
                CL_PROGRAM_BUILD_LOG,
                length,
                buffer,
                NULL
            );

            if (DEBUG)
                cerr << endl << "CLFW Error :  OpenCL Program Build Log : " << buffer << endl;
            else
                logger->print_log("\nCLFW Error :  OpenCL Program Build Log : ", buffer);

            free(buffer);
            buffer = NULL;
            this->uninitialize();
            exit(CLFW_FAILURE);
        }
    }
}

void CLFW::ocl_create_kernel(const char* ocl_kernel_name, const char* ocl_kernel_arg_types, ...)
{
    // Variable Declarations
    va_list kernel_args_list;

    // Code
    ocl_kernel = clCreateKernel(ocl_program, ocl_kernel_name, &ocl_result);
    ocl_exec_status(ocl_result, __LINE__);

    va_start(kernel_args_list, ocl_kernel_arg_types);

    for (auto i = 0; ocl_kernel_arg_types[i] != '\0'; i++)
    {
        switch(ocl_kernel_arg_types[i])
        {
            case 'b':
                clfwTypes->ocl_buffer = va_arg(kernel_args_list, cl_mem);
                ocl_exec_status(clSetKernelArg(ocl_kernel, i, sizeof(cl_mem), (void*)&clfwTypes->ocl_buffer), __LINE__);
            break;

            case 'i':
                clfwTypes->int_data = va_arg(kernel_args_list, cl_int);
                ocl_exec_status(clSetKernelArg(ocl_kernel, i, sizeof(cl_int), (void*)&clfwTypes->int_data), __LINE__);
            break;

            case 'f':
                clfwTypes->float_data = va_arg(kernel_args_list, cl_float);
                ocl_exec_status(clSetKernelArg(ocl_kernel, i, sizeof(cl_float), (void*)&clfwTypes->float_data), __LINE__);
            break;

            case 'c':
                clfwTypes->char_data = va_arg(kernel_args_list, cl_char);
                ocl_exec_status(clSetKernelArg(ocl_kernel, i, sizeof(cl_char), (void*)&clfwTypes->char_data), __LINE__);
            break;

            case 'u':
                clfwTypes->uchar_data = va_arg(kernel_args_list, cl_uchar);
                ocl_exec_status(clSetKernelArg(ocl_kernel, i, sizeof(cl_uchar), (void*)&clfwTypes->uchar_data), __LINE__);
            break;
        }
    }

    va_end(kernel_args_list);
}

void CLFW::ocl_execute_kernel(size_t ocl_global_work_size, size_t ocl_local_work_size)
{
    // Code

    // Kernel Configuration
    sdkCreateTimer(&ocl_timer);
    sdkStartTimer(&ocl_timer);
    {
        ocl_exec_status(clEnqueueNDRangeKernel(
            ocl_command_queue,
            ocl_kernel,
            1,
            NULL,
            &ocl_global_work_size,
            &ocl_local_work_size,
            0,
            NULL,
            NULL
        ), __LINE__);
    }
    sdkStopTimer(&ocl_timer);

    // Finish Command Queue
    clFinish(ocl_command_queue);

    ocl_gpu_time = sdkGetTimerValue(&ocl_timer);  
    sdkDeleteTimer(&ocl_timer);
    ocl_timer = NULL;
}

/// @brief Create an OpenCL buffer
/// @param flag OpenCL standard compliant memory flag eg. `CL_MEM_READ_ONLY`, `CL_MEM_WRITE_ONLY`, `CL_MEM_READ_WRITE`, etc...
/// @param ocl_data_size size of the buffer to be created in bytes
/// @return cl_mem buffer object
cl_mem CLFW::ocl_create_buffer(int flag, size_t ocl_data_size)
{	
	ocl_buffer = clCreateBuffer(
		ocl_context,
		flag,
		ocl_data_size,
		NULL,
		&ocl_result
	);
    
    ocl_exec_status(ocl_result, __LINE__);

    return ocl_buffer;
}

void CLFW::ocl_write_buffer(cl_mem ocl_data_buffer, size_t ocl_data_size, void* host_ptr)
{
    // Code
    ocl_exec_status(clEnqueueWriteBuffer(
        ocl_command_queue,
        ocl_data_buffer,
        CL_FALSE,
        0,
        ocl_data_size,
        host_ptr,
        0,
        NULL,
        NULL
    ), __LINE__);
}

void CLFW::ocl_read_buffer(cl_mem ocl_data_buffer, size_t ocl_data_size, void* host_ptr)
{
    // Code
    ocl_exec_status(clEnqueueReadBuffer(
        ocl_command_queue,
        ocl_data_buffer,
        CL_TRUE,
        0,
        ocl_data_size,
        host_ptr,
        0,
        NULL,
        NULL
    ), __LINE__);
}

void CLFW::ocl_release_buffer(cl_mem ocl_data_buffer)
{
    if (ocl_data_buffer)
    {
        clReleaseMemObject(ocl_data_buffer);
        ocl_data_buffer = NULL;
    }
}

void CLFW::host_alloc_mem(void** host_ptr, string host_type, size_t host_size)
{
    if (host_type == "int")
        *host_ptr = new int[host_size];
    else if (host_type == "float")
        *host_ptr = new float[host_size];
    else if (host_type == "double")
        *host_ptr = new double[host_size];
    else if (host_type == "char")
        *host_ptr = new char[host_size];
    else if (host_type == "uchar")
        *host_ptr = new unsigned char[host_size];

    if (host_ptr == NULL)
    {
        if (DEBUG)
            cerr << endl << "Failed To Allocate Memory To : " << *host_ptr << endl;
        else
            logger->print_log("Failed To Allocate Memory");

        exit(EXIT_FAILURE);
    }

    memset(*host_ptr, 0, host_size);
}

void CLFW::host_release_mem(void** host_ptr)
{
    if (host_ptr)
    {
        delete host_ptr;
        *host_ptr = nullptr;
    }
}

void CLFW::uninitialize(void)
{
    // Code
    if (ocl_kernel)
    {
        clReleaseKernel(ocl_kernel);
        ocl_kernel = NULL;
    }

    if (ocl_program)
    {
        clReleaseProgram(ocl_program);
        ocl_program = NULL;
    }

    if (ocl_command_queue)
    {
        clReleaseCommandQueue(ocl_command_queue);
        ocl_command_queue = NULL;
    }

    if (ocl_context)
    {
        clReleaseContext(ocl_context);
        ocl_context = NULL;
    }

    if (ocl_devices)
    {
        free(ocl_devices);
        ocl_devices = NULL;
    }

    if (ocl_platforms)
    {
        free(ocl_platforms);
        ocl_platforms = NULL;
    }

    if (logger)
    {
        logger->print_log("Log File Closed");
        logger->uninitialize();
        logger = NULL;
    }
}
