/** 
 * @brief Calculate the sum of an array
 * @param pd_input - input pointer to array of data to be summed
 * @param i_N - input size of array to be summed
 * @return sum of array
 * Compile(Mac): gcc-7 -fopenmp -o Parallel Parallel_PrefixS.cpp
 * For some reason, the code in main function keeps throwing error when compiling 
 * The other part of the code just works fine
 */

#include <stdio.h>
#include <omp.h>
#include <iostream>

void Sequential_PrefixSum(double *pInput, double * pOutput, int i_N)
{
    int i;
 
    pOutput[0] = pInput[0];

    for ( i = 1; i < i_N; i++ )
    {
        pOutput[i] = pInput[i] + pOutput[i-1];
    }

    return;

}

void Parallel_PrefixSum(double * pInput, double *pOutput, int i_N)
{
    int i;
 
    int nCore = omp_get_num_procs();
 
    int nStep = i_N / nCore;

    if ( nStep * nCore < i_N )
    {
        nStep += 1;
    }
 
#pragma omp parallel for num_threads(nCore)
    for ( i = 0; i < nCore; i++ )
    {
        int k;
        int nStart = i * nStep;
        int nEnd = (i+1) * nStep;
        if ( nEnd > i_N )
        {
            nEnd = i_N;
        }
        pOutput[nStart] = pInput[nStart];
        for ( k = nStart+1; k < nEnd; k++ )
        {
            pOutput[k] = pInput[k] + pOutput[k-1];
        }
    }
 
    for ( i = 2; i < nCore; i++ )
    {
        pOutput[i * nStep - 1] += pOutput[(i-1) * nStep - 1];
    }
 
    pOutput[i_N-1] += pOutput[(nCore-1)*nStep - 1];
 
#pragma omp parallel for num_threads(nCore-1)
    for ( i = 1; i < nCore; i++ )
    {
        int k;
        int nStart = i * nStep;
        int nEnd = (i+1) * nStep - 1;
        if ( nEnd >= i_N )
        {
            nEnd = i_N - 1;
        }
        for ( k = nStart; k < nEnd; k++ )
        {
            pOutput[k] += pOutput[nStart-1];
        }
    }
    return;
}

int main()
{
	int N = 10;
	double *pIn = new double[N];
	double *oOut = new double[N];

	int i=0;

	for(i=0; i<N ;i++)
	{
    	pIn[i]=i;
	}

	Sequential_PrefixSum(pIn,oOut,N);
	Parallel_PrefixSum(pIn,oOut,N);

	return 0;

}
