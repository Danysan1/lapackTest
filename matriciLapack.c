#include "matrici.h"
#include <stdio.h>
#include <stdlib.h>

// https://michaellindon.github.io/lindonslog/programming/atlas-blas-lapack-linear-algebra-libraries/
#include <lapacke/lapacke.h>
// https://software.intel.com/en-us/node/520875
// https://software.intel.com/en-us/node/520972
// https://software.intel.com/en-us/node/521079#CEE0C0FF-AC9E-4A8E-B6DD-F685EC92C021


/**
 * http://www.netlib.org/lapack/lapack-3.1.1/html/sgetrf.f.html
 * http://www.netlib.org/lapack/lapack-3.1.1/html/sgetri.f.html
 * http://www.netlib.org/lapack/explore-html/d8/ddc/group__real_g_ecomputational_ga8d99c11b94db3d5eac75cac46a0f2e17.html#ga8d99c11b94db3d5eac75cac46a0f2e17
 * http://www.netlib.org/lapack/explore-html/d8/ddc/group__real_g_ecomputational_ga1af62182327d0be67b1717db399d7d83.html#ga1af62182327d0be67b1717db399d7d83
 */
void inversa(){
    //       [ 1 1 0 ]         -1   [  1 -1 0 ]
    //   A = [ 0 1 0 ]  ==>   A   = [  0  1 0 ]
    //       [ 2 1 1 ]              [ -2  1 1 ]
#define INV_N 3 // colonne di A
#define INV_M INV_N // righe di A (=INV_N, matrice quadrata)
#define INV_LDA INV_M // dimensione effettiva di A
    float a[INV_M * INV_N] = { 1, 1, 0,
                               0, 1, 0,
                               2, 1, 1};

    stampaMatrice("Matrice:", a, INV_M, INV_N);

    lapack_int indiciPivot[INV_N];
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, INV_M, INV_N, a, INV_LDA, indiciPivot); // https://software.intel.com/en-us/node/520877

    if(info){
        printf("\nErrore di fattorizzazione %d, matrice non invertibile\n",info);
    } else {
        info = LAPACKE_sgetri(LAPACK_ROW_MAJOR, INV_N, a, INV_LDA, indiciPivot); // https://software.intel.com/en-us/node/520946
        if(info)
            printf("\nErrore %d",info);
        else {
            stampaMatrice("Inversa:", a, 3, 3);
        }
    }
}

void inversaInterattivo(){
    lapack_int n = chiediInt("Ordine della matrice? ");
    float *a = creaMatrice(n,n);

    stampaMatrice("Matrice A:", a, n, n);

    lapack_int *indiciPivot = (int*)malloc(n*sizeof(lapack_int));
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n, n, a, n, indiciPivot); // https://software.intel.com/en-us/node/520877

    if(info){
        printf("\nErrore di fattorizzazione %d, matrice non invertibile\n",info);
    } else {
        info = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n, a, n, indiciPivot); // https://software.intel.com/en-us/node/520946
        if(info)
            printf("\nErrore %d",info);
        else {
            stampaMatrice("Inversa:", a, n, n);
        }
    }

    free(a);
    free(indiciPivot);
}

/**
 * https://software.intel.com/en-us/node/520972
 * https://software.intel.com/en-us/node/520973
 * http://www.netlib.org/lapack/explore-html/d0/db8/group__real_g_esolve_ga3b05fb3999b3d7351cb3101a1fd28e78.html#ga3b05fb3999b3d7351cb3101a1fd28e78
 * http://www.netlib.org/lapack/explore-3.1.1-html/sgesv.f.html
 * http://www.netlib.org/lapack/lug/node38.html
 * http://www.netlib.org/lapack/lapacke.html
 * https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_sgesv_col.c.htm
 * http://www.netlib.org/clapack/readme
 */
void sistemaLineare(){
    //   [ 1 1  1 ] [ x ]   [ 4 ]         [ x ]   [ 1 ]
    //   [ 1 2 -1 ] [ y ] = [ 1 ]   ==>   [ y ] = [ 1 ]
    //   [ 3 3 -1 ] [ z ]   [ 4 ]         [ z ]   [ 2 ]
#define SIS_N 3 // numero di equazioni/incognite
#define SIS_LDA SIS_N // dimensione effettiva di A
#define SIS_NRHS 1 // numero di colonne della matrice B
#define SIS_LDB SIS_NRHS // dimensione effettiva di B
    float a[SIS_N*SIS_N] = { 1, 1, 1,
                             1, 2,-1,
                             3, 3,-1};

    float b[SIS_NRHS*SIS_N] = { 4,
                                1,
                                4};

    stampaMatrice("Matrice A:", a, SIS_N, SIS_LDA);
    stampaMatrice("Matrice B:", b, SIS_N, SIS_LDB);

    lapack_int indiciPivot[SIS_N];
    lapack_int info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, SIS_N, SIS_NRHS, a, SIS_LDA, indiciPivot, b, SIS_LDB); // https://software.intel.com/en-us/node/520973

    if(info)
        printf("\nErrore %d",info);
    else {
        stampaMatrice("Risultato:", b, SIS_N, SIS_LDB);
    }
}

void sistemaLineareInterattivo(){
    const lapack_int nrhs = 1;
    lapack_int n = chiediInt("Numero equazioni? ");
    float *a = creaMatrice(n,n);
    float *b = creaMatrice(n,nrhs);

    stampaMatrice("Matrice A:", a, n, n);
    stampaMatrice("Matrice B:", b, n, nrhs);

    lapack_int *indiciPivot = (int*)malloc(n*sizeof(lapack_int));
    lapack_int info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, n, nrhs, a, n, indiciPivot, b, nrhs); // https://software.intel.com/en-us/node/520973

    if(info)
        printf("\nErrore %d",info);
    else
        stampaMatrice("Risultato:", b, n, 1);

    free(a);
    free(b);
    free(indiciPivot);
}

/**
 * https://software.intel.com/en-us/node/521144
 * https://software.intel.com/en-us/node/521147
 * https://software.intel.com/en-us/node/521079
 */
void autovettori(){
    //       [ 2 1 0 ]                  [ 4 ]                  [ 1 -1  1 ]
    //   A = [ 1 3 1 ] ==> autovalori = [ 2 ]    autovettori = [ 2  0 -1 ]
    //       [ 0 1 2 ]                  [ 1 ]                  [ 1  1  1 ]
#define AUT_N 3
#define AUT_LDA AUT_N
#define AUT_LDVL AUT_N
#define AUT_LDVR AUT_N
    float A[AUT_N * AUT_N] = { 2, 1, 0,
                               1, 3, 1,
                               0, 1, 2};
    float wr[AUT_N];
    float wi[AUT_N];
    float vr[AUT_N * AUT_N];

    lapack_int info = LAPACKE_sgeev(LAPACK_ROW_MAJOR, 'N', 'V', AUT_N, A, AUT_LDA, wr, wi, NULL, AUT_LDVL, vr, AUT_LDVR);

    if(info)
        printf("\nErrore %d",info);
    else {
        stampaMatrice("Autovalori (parte reale):", wr, AUT_N, 1);
        stampaMatrice("Autovalori (parte immaginaria):", wi, AUT_N, 1);
        stampaMatrice("Autovettori:", vr, AUT_N, AUT_N);
    }
}

void autovettoriInterattivo(){
    lapack_int n = chiediInt("Ordine della matrice? ");
    float *A = creaMatrice(n,n);
    float *wr = creaMatriceVuota(n, 1);
    float *wi = creaMatriceVuota(n, 1);
    float *vr = creaMatriceVuota(n, n);

    lapack_int info = LAPACKE_sgeev(LAPACK_ROW_MAJOR, 'N', 'V', n, A, n, wr, wi, NULL, n, vr, n);

    if(info)
        printf("\nErrore %d",info);
    else {
        stampaMatrice("Autovalori (parte reale):", wr, n, 1);
        stampaMatrice("Autovalori (parte immaginaria):", wi, n, 1);
        stampaMatrice("Autovettori:", vr, n, n);
    }

    free(A);
    free(wr);
    free(wi);
    free(vr);
}
