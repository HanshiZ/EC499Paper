#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstring>
#include "tests.h"
#define ENTIRE 0
using std::ifstream;
using std::string;

void getfield(const char *,double * bad, double * avg, double * good, int* n);
void getints(const char* str, int* array, int n = 5);
void gettds(const char* file, int* data, int lower, int upper);
void getpunt(const char* file, int* data, int lower, int upper);
void getall(const char* file, int* data, int lower, int upper);
void getdrive(const char* file, double* tddata, double* data, int* n);
void estgetdrive(const char* file, double* data, int* n);

int main()
{

	double big12avg[200] = {0};
	double big12bad[300] = {0};
	double big12good[200] = {0};
	int counts12[3] = { 0 };

	double big10avg[200] = { 0 };
	double big10bad[300] = { 0 };
	double big10good[200] = { 0 };
	int counts10[3] = { 0 };

	getfield("big12.csv", big12bad, big12avg, big12good, counts12);
	getfield("big10.csv", big10bad, big10avg, big10good, counts10);
	std::cout << welch_t(big12bad, big10bad, *(counts12), *(counts10)) << std::endl;
#if ENTIRE
	

	memset(counts12,0, 3*sizeof(int));
	memset(counts10, 0, 3 * sizeof(int));
	getall("big10.csv", counts10, 0, 100);
	getall("big12.csv", counts12, 0, 100);

	int* chi2[2] = { counts10,  counts12};
	std::cout << chi2test(chi2, 2, 3);
#endif
#if !ENTIRE	
	std::cout << welch_t(big12avg, big10avg, *(counts12 + 1), *(counts10 + 1)) << std::endl;
	std::cout << welch_t(big12good, big10good, *(counts12 + 2), *(counts10 + 2)) << std::endl;


	int b12tds[2] = {0};
	int b10tds[2] = {0};
	gettds("big12.csv",b12tds, 0, 20);
	gettds("big10.csv", b10tds, 0, 20);
	int* chi2[2] = { b12tds, b10tds };
	std::cout << chi2test(chi2, 2, 2);

	memset(b12tds,0,2*sizeof(int));
	memset(b10tds, 0, 2 * sizeof(int));
	
	gettds("big12.csv", b12tds, 20, 40);
	gettds("big10.csv", b10tds, 20, 40);
	chi2[0] = b12tds;
	chi2[1]	 = b10tds;
	std::cout << chi2test(chi2, 2, 2);
	
	memset(b12tds, 0, 2 * sizeof(int));
	memset(b10tds, 0, 2 * sizeof(int));

	gettds("big12.csv", b12tds, 40, 100);
	gettds("big10.csv", b10tds, 40, 100);
	chi2[0] = b12tds;
	chi2[1] = b10tds;
	std::cout << chi2test(chi2, 2, 2);

	int b12all[3] = { 0 };
	int b10all[3] = { 0 };
	getall("big12.csv", b12all, 0, 100);
	getall("big10.csv", b10all, 0, 100);
	chi2[0] = b12all;
	chi2[1] = b10all;
	std::cout << chi2test(chi2, 2, 3);
	
	double big12drive7[300];
	double big12drive[300];
	double big10drive7[300];
	double big10drive[300];
	memset(counts12, 0, 3 * sizeof(int));
	memset(counts10, 0, 3 * sizeof(int));

	getdrive("big12.csv", big12drive7, big12drive, counts12);
	getdrive("big10.csv", big10drive7, big10drive, counts10);

	std::cout << welch_t(big12drive, big10drive, *(counts12 + 1), *(counts10 + 1)) << std::endl;
	std::cout << welch_t(big12drive7, big10drive7, *(counts12), *(counts10)) << std::endl;
	memcpy(big12drive7 + *counts12, big12drive, *(counts12 + 1) * sizeof(double));
	memcpy(big10drive7 + *counts10, big10drive, *(counts10 + 1) * sizeof(double));
	std::cout << welch_t(big12drive7, big10drive7, *(counts12 + 1) + *(counts12), *(counts10 + 1) + *counts10) << std::endl;
    
	estgetdrive("big12.csv", big12drive, counts12 + 2);
	estgetdrive("big10.csv", big10drive, counts10 + 2);
	std::cout << welch_t(big12drive, big10drive, *(counts12 + 2), *(counts10 + 2 )) << std::endl;
#endif
	
	return 0;
}

void gettds(const char* file, int* data, int lower, int upper)
{
	ifstream csv(file, std::ios::in);
	string line;
	int prev[5] = { 0 };
	int current[5] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, prev);
		if (prev[0])
		{
			if (prev[1] >= lower && prev[1] < upper)
				prev[3] > 5 ? ++data[1] : ++data[0];
		}
		memcpy(prev, current, sizeof(int) * 5);
	}
	printf("\n%s data bewtween %d and %d: TDs = %d, other = %d\n", file, lower, upper, data[1], data[0]);
	csv.close();
}

void getall(const char* file, int* data, int lower, int upper)
{
	ifstream csv(file, std::ios::in);
	string line;
	int prev[5] = { 0 };
	int current[5] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, prev);
		if (prev[0])
		{
			if (prev[1] >= lower && prev[1] < upper)
			{
				if (prev[3] > 5)  ++data[2];
				else if (prev[3]) ++data[1];

				else ++data[0];
			}
		}
		memcpy(prev, current, sizeof(int) * 5);
	}
	printf("\n%s data bewtween %d and %d: TDs = %d, FGS = %d, other = %d\n", file, lower, upper, data[2],data[1] ,data[0]);
	csv.close();
}

void getpunt(const char* file, int* data, int lower, int upper)
{
	ifstream csv(file, std::ios::in);
	string line;
	int prev[9] = { 0 };
	int current[9] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, current,9);
		if (prev[0])
		{
			if (prev[1] >= lower && prev[1] < upper)
				prev[6] ? ++data[1] : ++data[0];
		}
		memcpy(prev, current, sizeof(int) * 9);
	}
	printf("\n%s data bewtween %d and %d: punts = %d, other = %d\n", file, lower, upper, data[1], data[0]);
	csv.close();
}


void getfield(const char* file, double* bad, double* avg, double* good, int* n) 
{
	ifstream csv(file, std::ios::in);
    string line;
	int prev[9] = { 0 };
	int current[9] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, prev);
		if (prev[0])
		{
			int start = prev[1];
#if ENTIRE
			if (start < 100)
#else
			if (start < 20)
#endif		
			{
				bad[n[0]] = prev[3];
				++n[0];
			}
			else if (start < 40)
			{
				avg[n[1]] = prev[3];
				++n[1];
			}
			else
			{
				good[n[2]] = prev[3];
				++n[2];
			}
		}
		memcpy(prev, current, sizeof(int) * 5);
	}
	csv.close();
}

void getdrive(const char* file, double * tddata, double * data, int* n)
{
	ifstream csv(file, std::ios::in);
	string line;
	int prev[9] = { 0 };
	int current[9] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, prev);
		if (prev[0])
		{
			if (prev[3] > 5)
			{
				tddata[n[0]] = 100.0 - prev[1];
				++n[0];
			}
			else if (prev[3] == 0)
			{
				data[n[1]] = prev[2] - prev[1];
				++n[1];
			}

		}
	}
	csv.close();
}

void estgetdrive(const char* file, double* data, int* n)
{
	ifstream csv(file, std::ios::in);
	string line;
	int prev[9] = { 0 };
	int current[9] = { 0 };
	while (std::getline(csv, line))
	{
		const char* cs = line.c_str();
		getints(cs, prev);
		if (prev[0])
		{
			if (prev[3] > 5)
			{
				data[n[0]] = 100.0 - prev[1];
				++n[0];
			}
			else if (prev[3] == 0)
			{
				data[n[0]] = prev[2] - prev[1];
				++n[0];
			}
			else if (prev[3] == 3)
			{
				data[n[0]] = 65.0 - prev[1];
				++n[0];
			}

		}
	}
	csv.close();
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