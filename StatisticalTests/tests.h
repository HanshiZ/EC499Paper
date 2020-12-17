#ifndef TESTS_H
#define TESTS_H

double t_prob(double val, int deg, bool two_tail = 0, int iter = 1000000);

double welch_t(const double* d1, const double* d2, int n1, int n2, bool tails = 0);

double varx(const double* data, double mean, int n);

double chi2test(int** data, int row, int col);

double chi2_prob(double chistat, int deg, int iter = 1000000);

template <class numerical>
numerical total(numerical* ary, int n);

#endif
