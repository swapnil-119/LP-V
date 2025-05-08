#include <iostream>
using namespace std;

__global__ void add(int* A, int* B, int* C, int size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < size) {
        C[tid] = A[tid] + B[tid];
    }

}

void addCPU(int* A, int*B, int*C, int N)
{
    for(int i=0; i<N; i++)
    {
        C[i]=A[i]+B[i];
    }
}

void initialize(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 10;
    }
}

void print(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        cout << vector[i] << " ";
    }
    cout << endl;
}

int main() {
    int N = 10;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];
    int* D = new int[N];

    size_t vectorBytes = N * sizeof(int);

    initialize(A, N);
    initialize(B, N);

    cout << "Vector A: ";
    print(A, N);
    cout << "Vector B: ";
    print(B, N);

    int* X, * Y, * Z;
    cudaMalloc(&X, vectorBytes);
    cudaMalloc(&Y, vectorBytes);
    cudaMalloc(&Z, vectorBytes);

    cudaMemcpy(X, A, vectorBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(Y, B, vectorBytes, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;

    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    float gpu_elapsed_time;
    cudaEvent_t gpu_start,gpu_stop;
    cudaEventCreate(&gpu_start);
    cudaEventCreate(&gpu_stop);
    cudaEventRecord(gpu_start);

    add<<<blocksPerGrid, threadsPerBlock>>>(X, Y, Z, N);

    cudaEventRecord(gpu_stop);
    cudaEventSynchronize(gpu_stop);
    cudaEventElapsedTime(&gpu_elapsed_time, gpu_start, gpu_stop);
    cudaEventDestroy(gpu_start);
    cudaEventDestroy(gpu_stop);


    cudaMemcpy(C, Z, vectorBytes, cudaMemcpyDeviceToHost);

    cout<<"GPU Elapsed time is: "<<gpu_elapsed_time<<" milliseconds"<<endl;

    cout << "Addition: ";
    print(C, N);

    float cpu_elapsed_time;
    cudaEvent_t cpu_start,cpu_stop;
    cudaEventCreate(&cpu_start);
    cudaEventCreate(&cpu_stop);
    cudaEventRecord(cpu_start);

    addCPU(A,B,D,N);

    cudaEventRecord(cpu_stop);
    cudaEventSynchronize(cpu_stop);
    cudaEventElapsedTime(&cpu_elapsed_time, cpu_start, cpu_stop);
    cudaEventDestroy(cpu_start);
    cudaEventDestroy(cpu_stop);

    cout<<"CPU Elapsed time is: "<<cpu_elapsed_time<<" milliseconds"<<endl;

    cout << "Addition: ";
    print(D, N);

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] D;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);

    return 0;
}