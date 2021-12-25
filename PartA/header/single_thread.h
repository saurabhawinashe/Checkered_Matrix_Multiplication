void singleThread(int N, int *matA, int *matB, int *output)
{
    // enforce N to be power of 2 and greater than 2
    assert(N>=4 and N == ( N &~ (N-1)));
    register int temp = N>>1;
    for(register int rowA = 0; rowA < N; rowA += 2){
        register int rowCe = (rowA>>1)*N;
        register int rowCo = ((rowA+1)>>1)*N;
        register int t5 = rowA*N;
        register int rowB = 0;
        register int t6 = rowB*N;
        register int t1 = matA[t5], t2 = matA[t5+1], t3 = matA[t5+N], t4 = matA[t5+N+1];

        //compute partial sums
        for(register int indexB = t6; indexB < t6+N; indexB+=2){
            register int colC = (indexB - t6)>>1;
            register int indexC = rowCe + colC;
            output[indexC] = t1 * matB[indexB+N+1] + t2 * matB[indexB];

            colC += temp;
            indexC = rowCo + colC;
            output[indexC] = t3 * matB[indexB+N] + t4 * matB[indexB+1];
        }
        for(register int indexA = t5+2; indexA < t5+N; indexA+=2){
            rowB = indexA - t5;
            t6 = rowB*N;
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
    }
}
