/** 
 * @brief Calculate the sum of an array
 * @param pd_input - input pointer to array of data to be summed
 * @param i_N - input size of array to be summed
 * @return sum of array
 * Compile(Mac): g++-7 -fopenmp -o attemp1 attempt1.cpp
 */
#include <iostream> 
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <math.h>
using namespace std;

const int maxn = 10000000;
const int maxn1 = 23;
const int maxn2 = 5000000;
int a[maxn];
int b[maxn1][maxn2];
int c[maxn1][maxn2];
int temp;
int n=10;

void seq_PrefixSum();
void para_PrefixSum();

int main()
{
	while(true){
	cout << "enter n(-1 to quit) : " << endl;
	cin >>n;
	if(n==-1) return 0;
	temp = pow(2,n);
	para_PrefixSum();
	seq_PrefixSum();
	}
	system("pause");
	return 0;
}


void para_PrefixSum()
{
	double start = omp_get_wtime();
	int i,j,h;
		
	#pragma omp parallel for shared(a) private(i)
	for ( i= 1; i <=temp; i=i+1)
	{
		a[i] = i;
	}
	#pragma omp parallel for shared(a,b) private(j)
	
	for (  j = 1; j <= temp; j=j+1 )
	{
		b[0][j] = a[j];
	}
	
	int temp1 = n;
	
	#pragma omp parallel for shared(b) private(j,h)
	for (  h = 1; h <= temp1; h++)
	{
		int temp2 = temp/pow(2,h);
		
		for (  j = 1; j <= temp2; j++)
		{
			b[h][j]=b[h-1][2*j-1]+b[h-1][2*j];
		}
	}

	#pragma omp parallel for shared(c,b) private(j,h)
	for ( h = temp1; h>=0; h--)
	{
		for (j = 1; j <= temp/pow(2,h); j++)
		{
			if(j%2==0) c[h][j]=c[h+1][j/2];
			if(j==1) c[h][1]=b[h][1];
			if(j%2==1&&j>1) c[h][j]=c[h+1][(j-1)/2]+b[h][j];
		}
	}
	
	double end = omp_get_wtime();
	
	cout << " the time for parallel: " << end-start<<endl;
	
}

void seq_PrefixSum()
{
	double start = omp_get_wtime();

	for ( int i= 1; i <=temp; i++)
	{
		a[i] = i;
	}

	for ( int j = 1; j <= temp; j++ )
	{
		b[0][j] = a[j];
	}
	int temp1 = n;
	
	for ( int h = 1; h <= temp1; h++)
	{
		int temp2 = temp/pow(2,h);
		
		for ( int j = 1; j <= temp2; j++)
		{
			b[h][j]=b[h-1][2*j-1]+b[h-1][2*j];
		}
	}

	
	for (int h = temp1; h>=0; h--)
	{
		for (int j = 1; j <= temp/pow(2,h); j++)
		{
			if(j%2==0) c[h][j]=c[h+1][j/2];
			if(j==1) c[h][1]=b[h][1];
			if(j%2==1&&j>1) c[h][j]=c[h+1][(j-1)/2]+b[h][j];
		}
	}
	double end = omp_get_wtime();

	cout << " the time for series: " << end-start<<endl;
	
}