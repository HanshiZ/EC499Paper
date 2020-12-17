#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "systemsolver.h"

using std::cout;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::string;

template<class T>
void printarray(T *, int n);
void getints(const char* str, int* array, int n = 5);
void totals(int * aa[], int n = 11);

int * process(const char * file, int *** data, int intervals = 10);



#if 1
int main()
{	
	cout.precision(3);
	const int interval = 10;
	int** data[2];
	//const char* csvname = "big10.csv";
	
	int* point = process("big12.csv",data, interval);
	double * perstart  = (double *) calloc(interval,sizeof(double));
	double * solution = (double*)calloc(interval, sizeof(double));
	double ** system = doubleptrs(9, 10);
	cout << "Start,Opp. Start,Prob,Avg. Points\n";
	for (int i = 1; i < interval + 1; ++i)
	{
		for (int j = 1; j < interval + 1; ++j)
		{
			cout << i << ',' << j << ',';
			
			if (data[0][i][0])
			{
				double prob = 1.0 * data[0][i][j] / data[0][i][0];
				double avgp = prob ? 1.0 * data[1][i][j] / data[0][i][j] : 0;
				cout << prob << ',' << avgp << '\n';
				if (i < 10 && j < 10) system[i - 1][j - 1] = ((j == i) ? 1 + prob : prob);
			}
			else
			{
				cout << "no data" << ',' << "no data\n";
			}
		}
	}
	cout << "Start & Avg. Points \\\\ \n";
	for (int i = 0; i < interval; ++i)
	{
		if (perstart && data[0][i + 1][0])
		{
			perstart[i] = 1.0 * point[i] / data[0][i + 1][0];
			cout << i + 1 << " & " << perstart[i] << "\\\\ \n";
			if (i < 9) system[i][9] = perstart[i];
		}
		else
			cout << i + 1 << ',' << "no data" << '\n';
	}
	cout << "v_1 & v_2 & v_3 & v_4 & v_5 & v_6 & v_7 & v_8 & v_9 & Avg. Points \\\\ \n";
	for (int i = 0; i < 9; ++i)
		printa(system[i],10);
	solve(system, solution, 9, 10);
	cout << "System Solutions\n";
	printa(solution, interval);
	free(perstart);
	freeintptrs(data[0], interval + 1);
	freeintptrs(data[1], interval + 1);
	freeptrs(system, 9);
	free(point);
	
	return 0;
}

#endif
int * process(const char * file, int *** data, int intervals)
{

	int ** locations = intptrs(intervals+1,intervals+1);
	int ** points = intptrs(intervals+1,intervals+1);
	int * point = (int*) calloc(intervals,sizeof(int));
	ifstream csv(file);
	string line;
	int prev[9] = { 0 };
	int current[9] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, current);
		if (prev[0] && prev[0] == current[0] && prev[4] != current[4])
		{
			int start = prev[1] / (100 / intervals);
			point[start] = point[start] + prev[3];
			++start;
			int end = current[1] / (100 / intervals) + 1;
			++locations[start][end];
			points[start][end] = points[start][end] + prev[3];
		}
		memcpy(prev, current, sizeof(int) * 5);
	}
	csv.close();
	totals(locations, intervals+ 1);
	totals(points, intervals + 1);
	cout << "points\n";
	for (int i = 0; i < intervals + 1; ++i)
		printarray(points[i], intervals + 1);
	cout << "occurences\n";
	for (int i = 0; i < intervals + 1; ++i)
		printarray(locations[i], intervals + 1);
	data[0] = locations;
	data[1] = points;
	return point;
}

void totals(int * aa[], int n)
{
	for (int i = 1; i < n; ++i)
	{
		int vert = 0;
		int hor = 0;
		for (int j = 1; j < n; ++j)
		{
			hor = hor + aa[i][j];
			vert = vert + aa[j][i];
		}
		aa[0][i] = vert;
		aa[i][0] = hor;
	}
}

void getints(const char* str, int* array, int n)
{
	const char* ptr = str;
	for (int i = 0; i < n && ptr; ++i)
	{
		array[i] = atoi(ptr);
		ptr = strchr(ptr, ',');
		ptr = ptr + 1;
	}
}

template <class T>
void printarray(T* a, int n)
{
	for (int i = 0; i < n; i++)
        {
             cout << a[i];
	         i == n - 1 ? cout << "\\\\ \n" : cout << " & ";
        }
}