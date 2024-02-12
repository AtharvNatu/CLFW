#include "../include/CLFW/CLFW.hpp"

// Global Variable Declarations
const int elements = 11444777;
const int localSize = 256;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;
float* gold = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

float cpuTime = 0.0f;

const char* kernelSourceCode = 
	"__kernel void vecAddGPU(__global float *in1, __global float* in2, __global float *out, int len)" \
	"{" \
		"int i = get_global_id(0);" \
		"if (i < len)" \
		"{" \
			"out[i] = in1[i] + in2[i];" \
		"}" \
	"}";

void populateArray(float* arr, int len)
{
	// Code
	const float fscale = 1.0f / (float)RAND_MAX;

	for (int i = 0; i < len; i++)
	{
		arr[i] = fscale * rand();
	}
}

void vecAddCPU(const float* arr1, const float* arr2, float* out, int len)
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
	cpuTime = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;
}

void verifyOutput(float *hostArr, float *deviceArr)
{
	// Comparison
	const float epsilon = 0.000001f;
	int breakValue = -1;
	bool bAccuracy = true;

	for (int i = 0; i < elements; i++)
	{
		float val1 = hostArr[i];
		float val2 = deviceArr[i];

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

	size_t dataSize = elements * sizeof(float);

	clfw->hostMemAlloc((void**)&hostInput1, "float", dataSize);
	clfw->hostMemAlloc((void**)&hostInput2, "float", dataSize);
	clfw->hostMemAlloc((void**)&hostOutput, "float", dataSize);
	clfw->hostMemAlloc((void**)&gold, "float", dataSize);

	populateArray(hostInput1, elements);
	populateArray(hostInput2, elements);

	vecAddCPU(hostInput1, hostInput2, gold, elements);

	deviceInput1 = clfw->oclCreateBuffer(CL_MEM_READ_ONLY, dataSize);
	deviceInput2 = clfw->oclCreateBuffer(CL_MEM_READ_ONLY, dataSize);
	deviceOutput = clfw->oclCreateBuffer(CL_MEM_WRITE_ONLY, dataSize);

	clfw->oclCreateProgram(kernelSourceCode);

	clfw->oclCreateKernel("vecAddGPU", "bbbi", deviceInput1, deviceInput2, deviceOutput, elements);

	clfw->oclWriteBuffer(deviceInput1, dataSize, hostInput1);
	clfw->oclWriteBuffer(deviceInput2, dataSize, hostInput2);

	double gpuTime = clfw->oclExecuteKernel(clfw->getGlobalWorkSize(localSize, elements), localSize, 1);

	clfw->oclReadBuffer(deviceOutput, dataSize, hostOutput);

	verifyOutput(gold, hostOutput);

	std::cout << std::endl << "Time Required For CPU : " << cpuTime << " ms" << std::endl;
	std::cout << std::endl << "Time Required For GPU (OpenCL) : " << gpuTime << " ms" << std::endl << std::endl;

	clfw->oclReleaseBuffer(deviceOutput);
	clfw->oclReleaseBuffer(deviceInput2);
	clfw->oclReleaseBuffer(deviceInput1);

	clfw->hostMemFree(&gold);
	clfw->hostMemFree(&hostOutput);
	clfw->hostMemFree(&hostInput2);
	clfw->hostMemFree(&hostInput1);

    clfw->uninitialize();

	delete clfw;
	clfw = nullptr;

    return EXIT_SUCCESS;
}
