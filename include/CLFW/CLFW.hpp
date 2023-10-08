#ifndef _CLFW_HEADER_HPP_
    
    #define _CLFW_HEADER_HPP_

    // Standard Headers
    #include <iostream>
    #include <cstdlib>
    #include <cstdarg>
    #include <filesystem>
    #include <cstring>
    #include <cmath>

    // Timer
    #ifndef _HELPER_TIMER_H
        #define _HELPER_TIMER_H
        #include "helper_timer.h"
    #endif

    // CLFW Headers
    #include "CLFWMacros.hpp"
    #include "Logger.hpp"
    #include "CLFWTypes.hpp"

    // Platform Headers
    #if (CLFW_OS == 1)
        #include <CL/opencl.h>
        #pragma comment(lib, "OpenCL.lib")
    #elif (CLFW_OS == 2)
        #include <CL/opencl.h>
    #elif (CLFW_OS == 3)
        #include <OpenCL/cl.h>
    #endif

    class CLFW
    {
        // Member Variable Declarations
        private:

        // ClfwKernelDataTypes
        CLFWTypes *clfwTypes = nullptr;

        // Logger
        Logger *logger = nullptr;

        // Timer
        StopWatchInterface *ocl_timer = nullptr;

        // Device Enumeration
        cl_platform_id *ocl_platforms = nullptr;
        cl_device_id *ocl_devices = nullptr;
        cl_uint ocl_num_platforms = 0, ocl_num_devices = 0;
        char ocl_platform_info[512], ocl_dev_prop[1024];

        // Device Selection
        cl_platform_id ocl_platform_id;
        cl_device_id ocl_device_id;
        int user_platform, user_device;

        // Device Properties
        cl_ulong ocl_mem_size;
        cl_uint ocl_compute_units;

        cl_context ocl_context;
        cl_command_queue ocl_command_queue;
        const char* ocl_kernel_source_code = NULL;
        cl_program ocl_program;
        cl_kernel ocl_kernel;
        cl_int ocl_result;
        cl_mem ocl_buffer;

        public:

        float ocl_gpu_time = 0.0f;

        // Member Function Prototypes
        void initialize(void);
        void uninitialize(void);

        void ocl_exec_status(cl_int ocl_result, int line_no);
        string ocl_get_error_string(cl_int ocl_result);

        void ocl_get_platforms(void);
        void ocl_set_platform(int ocl_platform);

        void ocl_get_devices(void);
        void ocl_set_device(int ocl_device);

        void ocl_dev_properties(void);
        const char *ocl_read_kernel_from_file(const char *ocl_kernel_file);

        void ocl_create_context(void);
        void ocl_create_command_queue(void);
        void ocl_create_program(const char* ocl_kernel_file);
        void ocl_create_kernel(const char* ocl_kernel_name, const char* ocl_kernel_arg_types,...);
        void ocl_execute_kernel(size_t ocl_global_work_size, size_t ocl_local_work_size);

        void host_alloc_mem(void** host_ptr, string host_type, size_t host_size);
        void host_release_mem(void* host_ptr);

        cl_mem ocl_create_buffer(int flag, size_t ocl_data_size);
        void ocl_write_buffer(cl_mem ocl_data_buffer, size_t ocl_data_size, void* host_ptr);
        void ocl_read_buffer(cl_mem ocl_data_buffer, size_t ocl_data_size, void* host_ptr);
        void ocl_release_buffer(cl_mem ocl_data_buffer);
    };

#endif // _CLFW_HEADER_HPP_


