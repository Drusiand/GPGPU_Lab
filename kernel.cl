__kernel void MatMul(__global double* A, __global double* B, __global double* C, const int colsA, const int colsB) {
    const int row = get_global_id(0);
    const int col = get_global_id(1);

    double tmpSum = 0;
    for(int i = 0; i < colsA; i++){
        tmpSum += A[row * colsA + i] * B[i * colsB + col];
        // printf("%d ", row * colsB + col);
        // printf("%lf", tmpSum);
    }
    C[row * colsB + col] = tmpSum;
}