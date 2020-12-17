#include <cstdio>
#include <random>
#include <cmath>
#include <ctime>
#include "tests.h"

template <class numerical>
numerical total(numerical* ary, int n)
{
	numerical sum = 0;
	for (; n; --n)
		sum += *(ary + n - 1);
	return sum;
}

double varx(const double* data, double mean, int n)
{
	double sum = 0;
	for (int i = 0; i < n; ++i)
		sum = sum + (data[i] - mean) * (data[i]-mean);
	return sum / (n - 1.0);
}

double welch_t(const double * d1,const double * d2, int n1, int n2, bool tails)
{
	printf("\nRunning a %s Welch's t-test. n1 = %d, n2 = %d\n", tails ? "two tail" : "one tail",n1,n2);
	double mean1 = 0;
	double mean2 = 0;
	for (int i1 = 0; i1 < n1; ++i1)
		mean1 = mean1 + d1[i1];
	for (int i2 = 0; i2 < n2; ++i2)
	    mean2 = mean2 + d2[i2];
	mean1 = mean1 / n1;
	mean2 = mean2 / n2;
	double var1 = varx(d1, mean1, n1);
	double var2 = varx(d2, mean2, n2);
	printf("mean1, s^2: %.3f, %3f\n", mean1, var1);
	printf("mean2, s^2: %.3f, %3f\n", mean2, var2);

	double tstat = (mean1 - mean2) / sqrt(var1 / n1 + var2 / n2);
	printf("t-stat: %.3f ",tstat);
	int deg_free = floor((var1 / n1 + var2 / n2) * (var1 / n1 + var2 / n2) / (var1 * var1 / ((n1 - 1.0)*n1*n1) + var2 * var2 / ((n2 - 1.0)*n2*n2)));
	printf("degrees of freedom: %d\n", deg_free);

	return t_prob(tstat, deg_free, tails);
}

double t_prob(double val, int deg, bool two_tail, int iter)
{
	int count = 0;
	val = fabs(val);
	time_t seed = time(0);
	std::default_random_engine gen((unsigned)seed);
	std::student_t_distribution<double> dis(deg);
	for (int i = 0; i < iter; ++i)
		dis(gen) > val ? ++count : count;
	return two_tail ? (2.0 * count) / iter : (1.0 * count) / iter;

}


double chi2_prob(double chistat, int deg, int iter)
{
	int count = 0;
	time_t seed = time(0);
	std::default_random_engine gen((unsigned)seed);
	std::chi_squared_distribution<double> dis(deg);
	for (int i = 0; i < iter; ++i)
		dis(gen) > chistat ? ++count : count;
	return  (1.0 * count) / iter;
}

double chi2test(int** data, int row, int col)
{
	double chi = 0;
	int* rtot = (int*)calloc(row, sizeof(int));
	
	int* ctot = (int*)calloc(col, sizeof(int));
	if (!(rtot && ctot))
	{
		printf("\nChi^2 test memory allocation failed.\n");
		return 0;
	}
	for (int i = 0; i < row; ++i)
	{
		rtot[i] = total(data[i], col);
		for (int j = 0; j < col; ++j)
			ctot[j] += data[i][j];
	}
	
	int tot = total(ctot, col);
	for (int j = 0; j < row; ++j)
		for (int k = 0; k < col; ++k)
		{
			double expect = 1.0 * rtot[j] * ctot[k] / tot;
			chi += ((1.0 * data[j][k] - expect) * (1.0* data[j][k] - expect) / expect);
		}
	free(rtot);
	free(ctot);
	printf("\nChi^2 test with %d degree%s of freedom and statistic = %.3f\n", (col - 1)*(row - 1), (col - 1)*(row - 1)==1 ?"":"s",chi);
	return chi2_prob(chi, (col - 1) * (row - 1));
}