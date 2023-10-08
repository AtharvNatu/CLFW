#include "../include/CLFW/CLFW.hpp"

// Global Variable Declarations
const int elements = 11444777;
const int local_size = 256;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;
float* gold = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

float cpu_time = 0.0f;

void populate_array(float* arr, int len)
{
	// Code
	const float fscale = 1.0f / (float)RAND_MAX;

	for (int i = 0; i < len; i++)
	{
		arr[i] = fscale * rand();
	}
}

void vec_add_cpu(const float* arr1, const float* arr2, float* out, int len)
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
	cpu_time = sdkGetTimerValue(&timer);
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

void verify_output(float *host_arr, float *device_arr)
{
	// Comparison
	const float epsilon = 0.000001f;
	int breakValue = -1;
	bool bAccuracy = true;

	for (int i = 0; i < elements; i++)
	{
		float val1 = host_arr[i];
		float val2 = device_arr[i];

		if (fabs(val1 - val2) > epsilon)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}

	if (bAccuracy)
		cout << endl << "Vector Addition Successful !!!" << endl;
	else
		cerr << endl << "Vector Addition Failed !!!" << endl;
}

int main(void)
{
    CLFW *clfw = nullptr;
	clfw = new CLFW();

    clfw->initialize();

	cout << endl << "Testing VecAdd ..." << endl;

	size_t data_size = elements * sizeof(float);

	clfw->host_alloc_mem((void**)&hostInput1, "float", data_size);
	clfw->host_alloc_mem((void**)&hostInput2, "float", data_size);
	clfw->host_alloc_mem((void**)&hostOutput, "float", data_size);
	clfw->host_alloc_mem((void**)&gold, "float", data_size);

	populate_array(hostInput1, elements);
	populate_array(hostInput2, elements);

	vec_add_cpu(hostInput1, hostInput2, gold, elements);

	deviceInput1 = clfw->ocl_create_buffer(OCL_READ, data_size);
	deviceInput2 = clfw->ocl_create_buffer(OCL_READ, data_size);
	deviceOutput = clfw->ocl_create_buffer(OCL_WRITE, data_size);

	clfw->ocl_create_program("./include/OpenCL-Kernels/VecAdd.cl");

	clfw->ocl_create_kernel("vec_add_gpu", "bbbi", deviceInput1, deviceInput2, deviceOutput, elements);

	clfw->ocl_write_buffer(deviceInput1, data_size, hostInput1);
	clfw->ocl_write_buffer(deviceInput2, data_size, hostInput2);

	clfw->ocl_execute_kernel(round_global_size(local_size, elements), local_size);

	clfw->ocl_read_buffer(deviceOutput, data_size, hostOutput);

	verify_output(gold, hostOutput);

	cout << endl << "Time Required For CPU : " << cpu_time << " ms" << endl;
	cout << endl << "Time Required For GPU (OpenCL) : " << clfw->ocl_gpu_time << " ms" << endl << endl;

	clfw->ocl_release_buffer(deviceOutput);
	clfw->ocl_release_buffer(deviceInput2);
	clfw->ocl_release_buffer(deviceInput1);

	clfw->host_release_mem(gold);
	clfw->host_release_mem(hostOutput);
	clfw->host_release_mem(hostInput2);
	clfw->host_release_mem(hostInput1);

    clfw->uninitialize();

	delete clfw;
	clfw = nullptr;

    return EXIT_SUCCESS;
}
