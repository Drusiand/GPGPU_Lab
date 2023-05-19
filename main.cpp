#include "Worker.h"


int main() {
	Worker worker;
	cl_int errCode = worker.Init("kernel.cl");
	if (errCode == CL_SUCCESS)
		errCode = worker.Run("inputA.txt", "inputB.txt", "outputC.txt");
	return errCode;
}