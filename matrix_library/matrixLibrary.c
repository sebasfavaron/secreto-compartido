#include <stdlib.h>
#include <stdio.h>
#include "matrixLibrary.h"
#include "libraryMod251.h"
#include <math.h>


/*          Creation of Matrix        */
uint8_t ** new_matrix(int n, int m)
{
    uint8_t ** matrix = calloc(1, n * sizeof(uint8_t *));
    if (matrix == NULL)
    {
        printf("No se pudo inicializar la matriz \n");
        exit(0);
    }
    for (int i = 0; i  < n; i++)
    {
        matrix[i] = calloc(1, m * sizeof(uint8_t));
        if (matrix[i] == NULL)
        {
            printf("No se pudo inicializar la fila %d \n", i);
            exit(0);
        }
    }
    return matrix;
}

/*          Free Matriz                 */


void freeMatrix(uint8_t ** matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}


/*          Operaciones con matrices        */

void add_matrix (uint8_t ** matrix1, int n1, int m1, uint8_t ** matrix2, uint8_t ** answer)
{
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
        {
            answer[i][j] = suma_mod251(matrix1[i][j],matrix2[i][j]);
        }
    }
}

void substract_matrix (uint8_t ** matrix1, int n1, int m1, uint8_t ** matrix2, uint8_t ** answer)
{
    for (int i=0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
        {
            answer[i][j] = resta_mod251(matrix1[i][j], matrix2[i][j]);
        }
    }
}

void productoEscalar(uint8_t ** matrix, int n, int m, uint8_t num, uint8_t ** answer)
{
    for (int i = 0; i < n ; i++)
    {
        for (int j = 0; j < m; j++)
        {
            answer[i][j] = producto_mod251(matrix[i][j], num);
        }
    }
    return;
}

void copyMatrix (uint8_t ** matrixToCopy, int n, int m, uint8_t ** dest)
{
    for (int i=0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            dest[i][j] = matrixToCopy[i][j];
        }
    }
    return;
}

void multiply_matrix(uint8_t ** matrix1, int n1, int m1, uint8_t ** matrix2, int n2, int m2, uint8_t ** answer)
{
    for (int l = 0; l < n1; l++)
    {
        for (int p = 0; p < m2; p++ )
        {
            answer[l][p] = 0;
        }
    }

    for (int i = 0; i < n1; i ++)
    {
        for (int j = 0; j < m2; j++)
        {
            for (int k = 0; k < n2; k++)
            {
                answer[i][j] = suma_mod251(answer[i][j], producto_mod251(matrix1[i][k], matrix2[k][j]));
            }
        }
    }
}

void transpose_matrix ( uint8_t ** matrix, int n, int m, uint8_t ** answer)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j ++) {
            answer[i][j] = matrix[j][i];
        }
    }
}

void power_matrix (uint8_t ** matrix, int n, int m, int power, uint8_t ** answer)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            answer[i][j] = pow(matrix[i][j], power);
        }
    }
}

/* Operaciones de Filas */

void multiplyRow(uint8_t ** matrix, int n1, int m1, int rowIndex, uint8_t num)
{
    for (int j = 0; j < m1; j++)
    {
        matrix[rowIndex][j] = producto_mod251(matrix[rowIndex][j], num);
    }
}

void divideRow(uint8_t ** matrix, int n, int m,int rowIndex, uint8_t num)
{
    for (int j = 0; j < m; j++)
    {
        matrix[rowIndex][j] = dividir_mod251(matrix[rowIndex][j], num);
    }
}

void matrixSolver (uint8_t ** matrix, int n, int m, uint8_t * answer)
{
    uint8_t ** auxMatrix = new_matrix(n, m);
    copyMatrix(matrix, n, m, auxMatrix);
    solve_matrix(matrix, n, m);
    getResult(matrix, n, m, answer);
    freeMatrix(auxMatrix, n, m);
    return;
}

void solve_matrix (uint8_t ** matrix, int n, int m)
{
    int j;
    for (int i = 0; i < n; i++)
    {
        if (i >= m)
        {
            break;
        }

        swapInNonZeroRow(matrix, n, m, i);

        if (matrix[i][i] == 0)
        {
            return;
        }

        if (matrix[i][i] != 1)
        {
            uint8_t factor=matrix[i][i];
            divideRow(matrix, n, m, i, factor);
        }

        for (int j = 0; j < n ; j++)
        {
            if (matrix[j][i] != 0 && j != i)
            {
                uint8_t factor = matrix[j][i];
                divideRow(matrix, n, m, j, factor);
                subtractRow(matrix, n, m, j, i);
            }
        }
    }
    for (int x = 0; x < n; x++)
    {
        if (matrix[x][x] != 1)
        {
            uint8_t factor = matrix[x][x];
            divideRow(matrix, n, m, x, matrix[x][x]);
        }
    }
}

void getResult(uint8_t ** matrix, int n1, int m1, uint8_t *answer)
{
    for (int i = 0; i < n1; i++)
    {
        answer[i] = matrix[i][m1 - 1];
    }
}

void subtractRow(uint8_t ** matrix, int n1, int m1, int rowIndexChanged, int rowIndex)
{
    for (int j = 0; j < m1; j++)
    {
        matrix[rowIndexChanged][j] = mod251(resta_mod251(matrix[rowIndexChanged][j], matrix[rowIndex][j]));
    }

}


void swapInNonZeroRow(uint8_t ** matrix, int n1,int m1, int rowIndex)
{
    if (matrix[rowIndex][rowIndex] != 0)
    {
        return;
    }
    else
    {
        for (int i = rowIndex; i < n1; i++)
        {
            if (matrix[i][rowIndex] != 0)
            {
                swapRows(matrix, n1, m1, i, rowIndex);
                return;
            }
        }
    }
}

void swapRows(uint8_t ** matrix, int n1, int m1, int rowIndex1, int rowIndex2)
{
    for (int j = 0; j < m1; j++)
    {
        uint8_t aux = matrix[rowIndex1][j];
        matrix[rowIndex1][j] = matrix[rowIndex2][j];
        matrix[rowIndex2][j] = aux;
    }
}

void printMatrixWithResults(uint8_t ** matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m - 1; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("= %d \n", matrix[i][m - 1]);
    }
    printf("\n");
}

void printMatrix(uint8_t ** matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m ; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void getInvertibleMatrix (uint8_t ** matrix, int n, int m, uint8_t ** answer)
{
    uint8_t ** auxMatrix = new_matrix(n, n * 2);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n*2; j++)
        {
            if (j < n)
            {
                auxMatrix[i][j] = matrix[i][j];
            }
            else if (j - n == i)
            {
                auxMatrix[i][j] = 1;
            } else {
                auxMatrix[i][j] = 0;
            }
        }
    }
    solve_matrix(auxMatrix, n, n*2);
    for (int i = 0; i < n; i++)
    {
        for (int j = n; j < n * 2 ;j++)
        {
            answer[i][j - n] = auxMatrix[i][j];
        }
    }
    freeMatrix(auxMatrix, n, n*2);
}

void getIdentityMatrix(uint8_t ** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        matrix[i][i] = 1;
    }
    return;
}

bool areEqualMatrix(uint8_t ** matrix1, int n, int m, uint8_t ** matrix2)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (matrix1[i][j] != matrix2[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool areResultOfMatrix(uint8_t ** matrix, int n, int m, uint8_t * answer)
{
    for (int i = 0; i < n; i++)
    {
        uint8_t sum = 0;
        for (int j = 0; j < m - 1; j++)
        {
            sum =  suma_mod251(sum, producto_mod251(matrix[i][j], answer[j]));
        }
        sum = mod251(sum);
        uint8_t verify = mod251(matrix[i][m - 1]);
        if (sum != verify)
        {
            return false;
        }
    }
    return true;
}

void printVector(uint8_t * vector, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" X%d = %d ", i, vector[i]);
    }
    printf("\n");
}

uint8_t * newVector (int n)
{
    return malloc(n * sizeof(uint8_t));
}


