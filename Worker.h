#pragma once
#include <CL/cl.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <fstream>

#include "Matrix.h"

#define BUILD_STR "-cl-std=CL2.0" /*Verify OpenCL version!*/
#define MAT_MUL_FUNC_NAME "MatMul" /*Matrix multiplication function name as it is described in kernel.cl*/

using std::vector;
using std::array;
using std::string;
using std::ifstream;

class Worker
{
public:
	Worker();
	~Worker();
	cl_int Init(const char* kernelFileName);
	cl_int Run(const char* aSrc, const char* bSrc, const char* cDst); // A * B = C

private:
	cl_int CreatePlatform();
	cl_int CreateProgram(const char* kernelFileName);
	
	cl_int errCode;

	cl::Platform platform;
	cl::Device device;
	cl::Context context;
	cl::Program program;
};

