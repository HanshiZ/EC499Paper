#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "systemsolver.h"
const double TOL = .0000000001;


void printa(double * a , int n)
{
    for(int i = 0; i < n; ++i)
        printf("%.3f%s", a[i], i < n - 1 ? " & " : "\\\\ \n");
}

double ** doubleptrs(int r, int c)
{
    double ** ret = (double **) calloc(r, sizeof(double*));
    if (ret)
        for(int i = 0; i < r; ++i)
            ret[i] = (double*) calloc(c, sizeof(double)); 
    return ret;
}

int** intptrs(int r, int c)
{
    int** ret = (int**)calloc(r, sizeof(int*));
    if (ret)
        for (int i = 0; i < r; ++i)
            ret[i] = (int*)calloc(c, sizeof(int));
    return ret;
}

void freeintptrs(int** ptrs, int n)
{
    for (int i = 0; i < n; ++i)
        free(ptrs[i]);
    free(ptrs);
}

void freeptrs(double ** ptrs, int n)
{
    for(int i = 0; i < n; ++i)
        free(ptrs[i]);
    free(ptrs);
}

bool solve(double ** matrix, double * solutions, int rows, int cols)
{
    bool check = reduce(matrix, rows, cols);
    if (check)
    {
        *(solutions + rows - 1) = matrix[rows-1][cols-1] / matrix[rows-1][cols-2];
        for(int i = rows - 2; i > -1; --i)
        {
            double value = 0;
            for(int k = i + 1; k < cols - 1; ++k)
            {
                value += solutions[k] * matrix[i][k]; 
            }
            *(solutions + i) = (matrix[i][cols - 1] - value) /  matrix[i][i];
        }
    }
    else
        printf("No unique solution set.\n");
    return check;
}

bool reduce(double ** matrix, int rows, int cols)
{
    for(int i = 0; i < rows; ++i)
    {
        int maxloc = i;
        double max = fabs(matrix[i][i]);
        for(int j = i; j < rows ;++j)
        {
            if (fabs(matrix[j][i]) > max)
            {
                maxloc = j;
                max = matrix[j][i];
            }
        }
        if (max < TOL)
            return 0;
        if(i != maxloc)
        {
            double * temp = *(matrix + maxloc);
            *(matrix + maxloc) = *(matrix + i);
            *(matrix + i) = temp;
        }
        double pivot = matrix[i][i];
        for(int j2 = i + 1; j2 < rows ;++j2)
        {
            double scalar = matrix[j2][i] / pivot;
            combine(*(matrix + i) + i,*(matrix + j2) + i,scalar,cols - i);
        }
    }
    return 1;
}

void combine(double * r1, double * r2, double scalar, int n)
{
    for(int i = 0; i < n ; ++i)
       *(r2 + i) = *(r2 + i) - *(r1 + i) * scalar;
}



