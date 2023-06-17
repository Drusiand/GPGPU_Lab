__kernel void MatMul(__global double* A, __global double* B, __global double* C, const int colsA, const int colsB,
                            __local double* localA, __local double* localB, const int groupSize) {

    const int row = get_global_id(0);
    const int col = get_global_id(1);

    for (int i = 0; i < colsA * groupSize; i++){
       localB[i] = 0;
       localA[i] = 0;
            
    }
    barrier( CLK_LOCAL_MEM_FENCE );

    __private double sum = 0;

    const int localRow = get_local_id(0);
    const int localCol = get_local_id(1);

    int numTiles = colsB / groupSize + ((colsB % groupSize == 0) ? 0 : 1);  

    for (int t = 0; t < numTiles; t++) {
        if ((row < colsB) && (localCol + t * groupSize < colsA)) {
            localA[localRow * groupSize + localCol] = A[row * colsA + t * groupSize + localCol];
        }
        barrier( CLK_LOCAL_MEM_FENCE );

        if ((col < colsB) && (localRow + t * groupSize < colsA)) {
            localB[localRow * groupSize + localCol] = B[(t * groupSize + localRow) * colsB + col];
        }
        barrier( CLK_LOCAL_MEM_FENCE );

        for (int k = 0; k < groupSize; k++) {
            sum += localA[localRow * groupSize + k] * localB[k * groupSize + localCol];
        }
        barrier( CLK_LOCAL_MEM_FENCE );

    }
    C[row * colsB + col] = sum;
}