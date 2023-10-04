#include "../include/CLFW/CLFW.hpp"

void populate_array(float* arr, int len)
{
	// Code
	const float fscale = 1.0f / (float)RAND_MAX;

	for (int i = 0; i < len; i++)
	{
		arr[i] = fscale * rand();
	}
}

void vec_add_cpu(float* arr1, float* arr2, float* out, int len)
{
	// Code
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0; i < len; i++)
	{
		out[i] = arr1[i] + arr2[i];
	}

	sdkStopTimer(&timer);
	cout << "Time Required For CPU : " << sdkGetTimerValue(&timer) << " ms" << endl;
	sdkDeleteTimer(&timer);
	timer = NULL;
}

size_t round_global_size(int local_size, unsigned int global_size)
{
    // Round Global Size To Nearest Multiple Of Local Size

	// Code
	unsigned int remainder = global_size % local_size;

	if (remainder == 0)
		return global_size;
	else
		return global_size + local_size - remainder;
}

int main(void)
{
    CLFW *clfw = nullptr;

    const int elements = 11444777;
    const int local_size = 256;
    cl::Buffer bufferA, bufferB, bufferC;
    vector<cl::Buffer> kernel_args;
    float *A = nullptr, *B = nullptr, *C = nullptr;
    float *D = nullptr, *E = nullptr, *F = nullptr;

    clfw = new CLFW();

    clfw->ocl_initialize();

    size_t data_size = elements * sizeof(float);

    clfw->host_alloc_mem((void**)&A, "float", data_size);
    clfw->host_alloc_mem((void**)&B, "float", data_size);
    clfw->host_alloc_mem((void**)&C, "float", data_size);

    clfw->host_alloc_mem((void**)&D, "float", data_size);
    clfw->host_alloc_mem((void**)&E, "float", data_size);
    clfw->host_alloc_mem((void**)&F, "float", data_size);

    populate_array(A, elements);
    populate_array(B, elements);

    cout << endl << "\nVector Addition" << endl;
    cout << "-------------------------------------------------------------" << endl;

    vec_add_cpu(A, B, C, elements);

    cout << endl << "CPU : A" << endl;
    for (auto i = 0; i < 10; i++)
        cout << endl << A[i];
    cout << endl;

    // clfw->host_alloc_mem((void**)&C, "float", data_size);

    // bufferA = clfw->ocl_create_buffer(OCL_READ, data_size);
    // bufferB = clfw->ocl_create_buffer(OCL_READ, data_size);
    // bufferC = clfw->ocl_create_buffer(OCL_WRITE, data_size);

    bufferA = cl::Buffer(clfw->ocl_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data_size);
    bufferB = cl::Buffer(clfw->ocl_context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data_size);
    bufferC = cl::Buffer(clfw->ocl_context, CL_MEM_WRITE_ONLY, data_size);

    clfw->ocl_command_queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, data_size, A);
    clfw->ocl_command_queue.enqueueReadBuffer(bufferA, CL_TRUE, 0, data_size, D);

    cout << endl << "GPU : D" << endl;
    for (auto i = 0; i < 10; i++)
        cout << endl << D[i];
    cout << endl;

    clfw->ocl_command_queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, data_size, B);

    // clfw->ocl_write_buffer(bufferA, data_size, (void**)&A);
    // clfw->ocl_write_buffer(bufferB, data_size, (void**)&B);

    // clfw->ocl_command_queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, data_size, A);
    // clfw->ocl_command_queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, data_size, B);

    clfw->ocl_create_program("./include/OpenCL-Kernels/VecAdd.cl");

    clfw->ocl_create_kernel("vecAdd", "bbbi", &bufferA, &bufferB, &bufferC, elements);

    clfw->ocl_kernel.setArg(0, bufferA);
    clfw->ocl_kernel.setArg(1, bufferB);
    clfw->ocl_kernel.setArg(2, bufferC);
    clfw->ocl_kernel.setArg(3, elements);

    clfw->ocl_execute_kernel(round_global_size(local_size, elements), local_size);

    clfw->ocl_command_queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, data_size, C);

    // clfw->ocl_read_buffer(bufferC, data_size, C);

    bool success = true;
    for (int i = 0; i < elements; i++) 
    {
        if (C[i] != A[i] + B[i]) 
        {
            success = false;
            break;
        }
    }

    if (success) 
    {
        cout << endl << "Vector addition successful ! " << endl;
    } 
    else 
    {
        cout << endl << "Vector addition failed !" << endl;
    }

    cout << endl << "-------------------------------------------------------------" << endl;

    clfw->host_release_mem(C);
    clfw->host_release_mem(B);
    clfw->host_release_mem(A);

    clfw->ocl_uninitialize();   

    delete clfw;
    clfw = nullptr;

    return EXIT_SUCCESS;
}
