// Create other necessary functions here
#include <ctime>
#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()


__global__ void multiply(int N, int *matA, int *matB, int *output)

{
	//Compute the row chosen from matrix A
	int rowA = blockIdx.x;

	//Compute the column set from matrix B
	int colSetB = threadIdx.x*2;

	//Compute the index where the result has to be stored
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	
	register int sum = 0;

	register int indexB1, indexB2;
	
	//CMM algorithm that computes the product of one row and one solumn set
	indexB1 = (rowA & 1)?colSetB + N:colSetB+1+N;
	indexB2 = (rowA & 1)?colSetB + 1:colSetB; 
	register int temp = N<<1;
	for(register int indexA = rowA*N; indexA < (rowA+1)*N; indexA += 2)
	{
		sum += matA[indexA] * matB[indexB1] + matA[indexA + 1] * matB[indexB2];
		indexB1 += temp;
		indexB2 += temp;
	}
	
	//Write the result to the appropriate index
	output[index] = sum;
}



// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
	int *a, *b, *o;

	//Allocat memory in GPU for the input matrices
	cudaMalloc((void **)&a, N*N*sizeof(int));
	cudaMalloc((void **)&b, N*N*sizeof(int));
	cudaMalloc((void **)&o, N*(N>>1)*sizeof(int));
	
	auto begin = TIME_NOW;
	//Copy input matrices to GPU
	cudaMemcpy(a, matA, N*N*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(b, matB, N*N*sizeof(int), cudaMemcpyHostToDevice);
	auto end = TIME_NOW;

	cout<<"Time required for A & B transfer to GPU :"<<(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";

	begin = TIME_NOW;

	//Create N blocks for N rows in matA and N/2 threads in each block for the column sets in matB
	multiply<<<N, N>>1>>>(N, a, b, o);
	end = TIME_NOW;
	cout<<"Time required for mult in GPU :"<<(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";

	begin = TIME_NOW;
	//Copy result matrix back to CPU
	cudaMemcpy(output, o, N*(N>>1)*sizeof(int),cudaMemcpyDeviceToHost);
	end = TIME_NOW;

	cout<<"Time required for C transfer to CPU :"<<(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";

	cudaFree(a);
	cudaFree(b);
	cudaFree(o);
}