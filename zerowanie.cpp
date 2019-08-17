#include "zerowanie.h"

void zeruj(int *A, int N)
{
    int i;

    for(i = 0; i < N; i++)
	A[i] = 0;

}

void zeruj(char *A, int N)
{
    int i;

    for(i = 0; i < N; i++)
	A[i] = 0;

}

void zeruj(int **A, int N, int M)
{
    int i, j;

    for(i = 0; i < N; i++)
    {
	for(j = 0; j < M; j++)
	{
	    A[i][j] = 0;
	}
    }

}

void zeruj(char **A, int N, int M)
{
    int i, j;

    for(i = 0; i < N; i++)
    {
	for(j = 0; j < M; j++)
	{
	    A[i][j] = 0;
	}
    }

}

void zeruj(float *A, int N)
{
    int i;

    for(i = 0; i < N; i++)
	A[i] = 0.0;

}
