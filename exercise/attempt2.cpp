/** 
 * @brief Calculate the sum of an array
 * @param pd_input - input pointer to array of data to be summed
 * @param i_N - input size of array to be summed
 * @return sum of array
 * Compile(Mac): g++-7 -fopenmp -o attemp2 attempt2.cpp
 */
#include <iostream>
#include <omp.h>
#include <math.h>
using namespace std;

const int maxn = 10000000;
int a[maxn];
int b[maxn];

void parallel_s(int);
void sequential_s(int);

int main()
{
	int n;
	
	while(true)
	{
	cout << "enter n: " <<endl;
	cin >>n;
	
	sequential_s(n);
	parallel_s(n);
	int i ;
	}
	
	system("pause");
	return 0;
}

void parallel_s(int n)
{
	double start = omp_get_wtime();
	int i,j,h;
	int temp1 = log(n)/log(2);
	#pragma omp parallel for shared(a,b) private(i)	
	for ( i = 1;  i <= n; i++)
	{
		a[i] = i;
		b[i] = i;
	}
	
	#pragma omp parallel for shared(a,b)private(i,j,h)
	for ( i = 1; i<=temp1+1; i=i+1)
	{
		int tid = omp_get_thread_num();
		int temp =pow(2,i-1);
		for ( j = 1; j+temp<=n; j++)
		{
			b[j+temp] = a[j]+a[j+temp];
		}
		for (  h = 1; h<=n; h++ )
		{
			a[h] = b[h];
		}
	}
	double end = omp_get_wtime();

	cout << " the time of parallel: " << end-start<<endl;
}

void sequential_s(int n)
{
	double start = omp_get_wtime();
	int i;

	for ( i = 1;  i <= n; i++)
	{
		a[i] = i;
		b[i] = i;
	}
	

	
	for ( i = 1; pow(2,i-1)<n; i++)
	{
		int temp =pow(2,i-1);
		
		for (int j = 1; j+temp<=n; j++)
		{
			b[j+temp] = a[j]+a[j+temp];
		}
		
		for ( int h = 1; h<=n; h++ )
			a[h] = b[h];
	}

	double end = omp_get_wtime();

	cout << " the time of serials: " << end-start<<endl;
}
