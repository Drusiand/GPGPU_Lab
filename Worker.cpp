#include "Worker.h"

Worker::Worker()
{
	errCode = 0;
}

Worker::~Worker()
{
}

cl_int Worker::Init(const char* kernelFileName)
{
	errCode = CreatePlatform();
	errCode = CreateProgram(kernelFileName);
	return errCode;
}

cl_int Worker::CreatePlatform()
{
	vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	assert(platforms.size() > 0);
	platform = platforms.front();
	vector<cl::Device> devices;

	errCode = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	assert(devices.size() > 0);

	if (!errCode)
	{
		device = devices.front();
		cl::STRING_CLASS vendor = device.getInfo<CL_DEVICE_VENDOR>(); //No need to keep as field
		cl::STRING_CLASS version = device.getInfo<CL_DEVICE_VERSION>(); //No need to keep as field

		std::cout << "Current vendor:" << vendor << std::endl;
		std::cout << "Current OpenCL version:" << version << std::endl;
	}
    return errCode;
}

cl_int Worker::CreateProgram(const char* kernelFileName)
{
	ifstream kernelFile(kernelFileName);
	string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.size() + 1));

	context = cl::Context(device);
	program = cl::Program(context, sources);

	return program.build(BUILD_STR);
}

cl_int Worker::Run(const char* aSrc, const char* bSrc, const char* cDst)
{
	Matrix matrixA;
	matrixA.ReadFromFile(aSrc);
	assert(matrixA.Data() != nullptr);
	const int rowsA = matrixA.Rows();
	const int colsA = matrixA.Cols();
	const int countA = matrixA.Count();

	Matrix matrixB;
	matrixB.ReadFromFile(bSrc);
	assert(matrixB.Data() != nullptr);
	const int rowsB = matrixB.Rows();
	const int colsB = matrixB.Cols();
	const int countB = matrixB.Count();

	assert(rowsA == colsB);

	Matrix matrixC;
	matrixC.CreateFromDims(rowsA, colsB);
	const int rowsC = matrixC.Rows();
	const int colsC = matrixC.Cols();
	const int countC = matrixC.Count();

	cl::Buffer memBufA(context, CL_MEM_READ_WRITE, sizeof(double) * countA);
	cl::Buffer memBufB(context, CL_MEM_READ_WRITE, sizeof(double) * countB);
	cl::Buffer memBufC(context, CL_MEM_READ_WRITE, sizeof(double) * countC);

	cl::CommandQueue queue(context, device);
	errCode = queue.enqueueWriteBuffer(memBufA, CL_TRUE, 0, sizeof(double) * countA, matrixA.Data());

	if (errCode == CL_SUCCESS)
		errCode = queue.enqueueWriteBuffer(memBufB, CL_TRUE, 0, sizeof(double) * countB, matrixB.Data());

	cl::Kernel kernel(program, MAT_MUL_FUNC_NAME, &errCode);

	if (errCode == CL_SUCCESS){
		errCode += kernel.setArg(0, memBufA);
		errCode += kernel.setArg(1, memBufB);
		errCode += kernel.setArg(2, memBufC);
		errCode += kernel.setArg(3, colsA);
		errCode += kernel.setArg(4, colsB);
	}
	
	if (errCode == CL_SUCCESS) {
		errCode = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(colsC, rowsC));
	}

	if (errCode == CL_SUCCESS) {
		queue.enqueueReadBuffer(memBufC, CL_TRUE, 0, sizeof(double) * countC, matrixC.Data());
	}

	// matrixC.Print();
	std::cout << "Result written to " << cDst;
	matrixC.WriteToFile(cDst);

	return errCode;
}
