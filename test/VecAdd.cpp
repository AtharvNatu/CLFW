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
		std::cout << std::endl << "Vector Addition Successful ..." << std::endl;
	else
		std::cerr << std::endl << "Vector Addition Failed At Index : " << breakValue << " !!!" << std::endl;
}

int main(void)
{
    CLFW *clfw = nullptr;
	clfw = new CLFW();

    clfw->initialize();

	std::cout << std::endl << "Testing VecAdd ..." << std::endl;

	size_t data_size = elements * sizeof(float);

	clfw->hostMemAlloc((void**)&hostInput1, "float", data_size);
	clfw->hostMemAlloc((void**)&hostInput2, "float", data_size);
	clfw->hostMemAlloc((void**)&hostOutput, "float", data_size);
	clfw->hostMemAlloc((void**)&gold, "float", data_size);

	populate_array(hostInput1, elements);
	populate_array(hostInput2, elements);

	vec_add_cpu(hostInput1, hostInput2, gold, elements);

	deviceInput1 = clfw->oclCreateBuffer(CL_MEM_READ_ONLY, data_size);
	deviceInput2 = clfw->oclCreateBuffer(CL_MEM_READ_ONLY, data_size);
	deviceOutput = clfw->oclCreateBuffer(CL_MEM_WRITE_ONLY, data_size);

	clfw->oclCreateProgram("./include/OpenCL-Kernels/VecAdd.cl");

	clfw->oclCreateKernel("vec_add_gpu", "bbbi", deviceInput1, deviceInput2, deviceOutput, elements);

	clfw->oclWriteBuffer(deviceInput1, data_size, hostInput1);
	clfw->oclWriteBuffer(deviceInput2, data_size, hostInput2);

	double gpuTime = clfw->oclExecuteKernel(round_global_size(local_size, elements), local_size);

	clfw->oclReadBuffer(deviceOutput, data_size, hostOutput);

	verify_output(gold, hostOutput);

	std::cout << std::endl << "Time Required For CPU : " << cpu_time << " ms" << std::endl;
	std::cout << std::endl << "Time Required For GPU (OpenCL) : " << gpuTime << " ms" << std::endl << std::endl;

	clfw->oclReleaseBuffer(deviceOutput);
	clfw->oclReleaseBuffer(deviceInput2);
	clfw->oclReleaseBuffer(deviceInput1);

	clfw->hostMemFree((void**)&gold);
	clfw->hostMemFree((void**)&hostOutput);
	clfw->hostMemFree((void**)&hostInput2);
	clfw->hostMemFree((void**)&hostInput1);

    clfw->uninitialize();

	delete clfw;
	clfw = nullptr;

    return EXIT_SUCCESS;
}
