#define WIN32_LEAN_AND_MEAN
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "slowcode.h"
#include <windows.h>
#include <stdint.h>
#include <iostream>

typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}

void get_walltime(double* wcTime) { 
	struct timeval tp; 
	gettimeofday(&tp, NULL); 
	*wcTime = (double)(tp.tv_sec + tp.tv_usec / 1000000.0); 
}
//   complex algorithm for evaluation
void myfunc(std::vector<std::vector<double> >  &v_s,
			std::vector<std::vector<double> >  &v_mat, std::vector<int> &i_v){
	double d_val;
	for (int j =  0; j  <  v_s.size(); j  ++)
	{
		for (int i  =  0;   i  <  v_s[0].size(); i++)
		{
			d_val = fmod(i_v[i],256);
			v_mat[i][j] = v_s[i][j]*(sin(d_val)*sin(d_val)-cos(d_val)*cos(d_val));
		}
	}
}

int main(int argc, char *argv[])
{
	//this should be called as > ./slow_code<i_R><i_N>
	int i_R = 1000;
	int i_N = 100;

	double d_S, d_E;

	//parse input parameters
	if (argc >= 2)
	{
		i_R = atoi(argv[1]);
	}
	if (argc >= 3)
	{
		i_N = atoi(argv[2]);
	}

	//declarations
	std::vector<std::vector<double>> vd_s(i_N, std::vector<double>(i_N));
	std::vector<std::vector<double>> vd_mat(i_N, std::vector<double>(i_N));
	std::vector<int> vi_v(i_N);

	//populate memory with some random data
	for (int i = 0; i < i_N; i++)
	{
		vi_v[i] = i*i;
		for (int j = 0; j < i_N; j++)
		{
			vd_s[i][j] = j + i;
		}

		//start benchmark
		get_walltime(&d_S);

		//iterative test loop
		for (int i = 0; i < i_R; i++)
		{
			myfunc(vd_s, vd_mat, vi_v);
		}
		//end benchmark
		get_walltime(&d_E);

		//report results
		std::cout<<"Elapsed time: %f\n"<<d_E - d_S;
		return 0;
	}
}
