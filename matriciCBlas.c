#include "matrici.h"
#include <stdlib.h>

#include <cblas.h>
// https://software.intel.com/en-us/node/520730
// https://software.intel.com/en-us/node/520748
// https://software.intel.com/en-us/node/520774


void moltiplicaMatriceVettore(){
    //      [ 1 2 1,5 ]        [ 1 ]        [ 0 ]                                        [ 11 ]
    //  A = [ 0 3  2  ]    x = [ 2 ]    y = [ 0 ]    a = 1    b = 1    y = a*A*x + b*y = [ 14 ]
    //                         [ 4 ]
#define MV_M 2 // Righe di A, righe di y
#define MV_N 3 // Colonne di A, righe di x
#define MV_LDA MV_N // Dimensione effettiva di A
#define MV_INC_X 1 // Dimensione elementi in x
#define MV_INC_Y 1 // Dimensione elementi in y
    float A[MV_M * MV_N] = { 1, 2, 1.5,
                             0, 3, 2  };
    float x[MV_N] = {1,2,4};
    float y[MV_M] = {0,0};
    float a = 1, b = 1;

    stampaMatrice("Matrice:", A, MV_M, MV_N);
    stampaMatrice("Vettore:", x, MV_N, 1);

    cblas_sgemv(CblasRowMajor, CblasNoTrans, MV_M, MV_N, a, A, MV_LDA, x, MV_INC_X, b, y, MV_INC_Y); // https://software.intel.com/en-us/node/520750

    stampaMatrice("Risultato:", y, MV_M, 1);
}

void moltiplicaMatriceVettoreInterattivo(){
    int m = chiediInt("Numero righe della matrice? ");
    int n = chiediInt("Numero colonne della matrice? ");
    float *A = creaMatrice(m,n);
    float *x = creaMatrice(n, 1);
    float *y = creaMatriceVuota(m, 1);
    float a = 1, b = 1;

    stampaMatrice("Matrice:", A, m, n);
    stampaMatrice("Vettore:", x, n, 1);

    cblas_sgemv(CblasRowMajor, CblasNoTrans, m, n, a, A, n, x, 1, b, y, 1); // https://software.intel.com/en-us/node/520750

    stampaMatrice("Risultato:", y, m, 1);

    free(A);
    free(x);
    free(y);
}

void moltiplicaMatrici(){
    //      [ 2 1 10 ]        [  2 0 ]        [ 0 0 ]                                        [ -5 61 ]
    //  A = [ 1 0 -1 ]    B = [  1 1 ]    C = [ 0 0 ]    a = 1    b = 1    C = a*A*B + b*C = [  3 -6 ]
    //      [ 2 1  0 ]        [ -1 6 ]        [ 0 0 ]                                        [  5  1 ]
#define MM_M 3 // Righe di A, righe di C
#define MM_N 2 // Colonne di B, colonne di C
#define MM_K 3 // Colonne di A, righe di B
#define MM_LDA MM_K // Dimensione effettiva di A
#define MM_LDB MM_N // Dimensione effettiva di B
#define MM_LDC MM_N // Dimensione effettiva di C
    float A[MM_M * MM_K] = { 2, 1,10,
                             1, 0,-1,
                             2, 1, 0};

    float B[MM_K * MM_N] = { 2, 0,
                             1, 1,
                             -1, 6};

    float C[MM_M * MM_N] = { 0, 0,
                             0, 0,
                             0, 0};
    float a = 1, b = 1;

    stampaMatrice("Matrice A:", A, MM_M, MM_K);
    stampaMatrice("Matrice B:", B, MM_K, MM_N);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, MM_M, MM_N, MM_K, a, A, MM_LDA, B, MM_LDB, b, C, MM_LDC); // https://software.intel.com/en-us/node/520775

    stampaMatrice("Risultato:", C, MM_M, MM_N);
}

void moltiplicaMatriciInterattivo(){
    int m  = chiediInt("Numero righe della prima matrice? ");
    int n = chiediInt("Numero colonne della seconda matrice? ");
    int k = chiediInt("Dimensione comune? ");
    float *A = creaMatrice(m, k);
    float *B = creaMatrice(k, n);
    float *C = creaMatriceVuota(m, n);
    float a = 1, b = 1;

    stampaMatrice("Matrice A:", A, m, k);
    stampaMatrice("Matrice B:", B, k, n);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, a, A, k, B, n, b, C, n); // https://software.intel.com/en-us/node/520775

    stampaMatrice("Risultato:", C, m, n);

    free(A);
    free(B);
    free(C);
}
