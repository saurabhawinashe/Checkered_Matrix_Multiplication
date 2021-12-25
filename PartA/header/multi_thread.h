#include <pthread.h>

struct thread_data {
	int rowA, N, line;
	int *matA;
	int *matB;
	int *output;
};
// Create other necessary functions here
void* thread_func(void* td){
	struct thread_data *t;
	t = (struct thread_data *) td;
	register int N = t->N, rowA = t->rowA;
	register int *matA = t->matA, *matB = t->matB, *output = t->output;

	register int temp = N>>1;
	register int rowCe = (rowA>>1)*N;
    register int rowCo = ((rowA+1)>>1)*N;
    register int t5 = rowA*N;
    register int t1 = matA[t5], t2 = matA[t5+1], t3 = matA[t5+N], t4 = matA[t5+N+1];
    for(register int indexB = 0; indexB < N; indexB+=2){
        register int colC = indexB>>1;
        register int indexC = rowCe + colC;
        output[indexC] = t1 * matB[indexB+N+1] + t2 * matB[indexB];

        colC += temp;
        indexC = rowCo + colC;
        output[indexC] = t3 * matB[indexB+N] + t4 * matB[indexB+1];
    }
    for(register int indexA = t5+2; indexA < t5+N; indexA+=2){
        register int rowB = indexA - t5;
        register int t6 = rowB*N;
        t1 = matA[indexA], t2 = matA[indexA+1], t3 = matA[indexA+N], t4 = matA[indexA+N+1];
        for(register int indexB = t6; indexB < t6+N; indexB+=2){
            register int colC = (indexB - t6)>>1;
            register int indexC = rowCe + colC;
            output[indexC] += t1 * matB[indexB+N+1] + t2 * matB[indexB];

            colC += temp;
            indexC = rowCo + colC;
            output[indexC] += t3 * matB[indexB+N] + t4 * matB[indexB+1];
        }
    }
    pthread_exit(NULL);
}

// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
	assert(N>=4 and N == ( N &~ (N-1)));
    struct thread_data td[4];
    for(register int rowA = 0; rowA < N; rowA += 8){
    	pthread_t thread[4];
    	for(int i = 0; i < 4; i++){
    		td[i].rowA = rowA + i*2;
    		td[i].N = N;
    		td[i].matA = matA;
    		td[i].matB = matB;
    		td[i].output = output;
    		int rc = pthread_create(&thread[i], NULL, &thread_func, (void *)&td[i]);
    	}
    	for(int i = 0; i < 4; i++)
    		pthread_join(thread[i], NULL);
    }
}
