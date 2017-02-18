#include <stdio.h>
#include <string.h>
#include <ctype.h>

// https://michaellindon.github.io/lindonslog/programming/atlas-blas-lapack-linear-algebra-libraries/
#include <lapacke/lapacke.h>
// https://software.intel.com/en-us/node/520875
// https://software.intel.com/en-us/node/520972
// https://software.intel.com/en-us/node/521079#CEE0C0FF-AC9E-4A8E-B6DD-F685EC92C021
#include <cblas.h>
// https://software.intel.com/en-us/node/520730
// https://software.intel.com/en-us/node/520748
// https://software.intel.com/en-us/node/520774

void stampaMatrice(char *titolo, float *matrice, unsigned righe, unsigned colonne);
float *creaMatrice(unsigned righe, unsigned colonne);
float *creaMatriceVuota(unsigned righe, unsigned colonne);
int chiediInt(char *richiesta);

void moltiplicaMatriceVettore();
void moltiplicaMatriceVettoreInterattivo();
void moltiplicaMatrici();
void moltiplicaMatriciInterattivo();
void sistemaLineare();
void sistemaLineareInterattivo();
void inversa();
void inversaInterattivo();

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    char buffer = 0;
    while(1){
        printf("Azione? < v:vett*matr | m:matr*matr | s:sistema | i:inversa | EOF:esci > ");

        do {
            buffer = getchar();
        } while (isspace(buffer));

        switch(buffer){
        case 'v':
            //moltiplicaMatriceVettore();
            moltiplicaMatriceVettoreInterattivo();
            break;

        case 'm':
            //moltiplicaMatrici();
            moltiplicaMatriciInterattivo();
            break;

        case 's':
            //sistemaLineare();
            sistemaLineareInterattivo();
            break;

        case 'i':
            //inversa();
            inversaInterattivo();
            break;

        case EOF:
            putchar('\n');
            return 0;

        default:
            puts("Comando sconosciuto");
            break;
        }

        putchar('\n');
    }
}


void stampaMatrice(char *titolo, float *matrice, unsigned righe, unsigned colonne){
    puts(titolo);
    unsigned riga, colonna;
    for(riga=0; riga<righe; riga++){
        for(colonna=0; colonna<colonne; colonna++){
            printf("%1.1f\t", matrice[riga*colonne + colonna]);
        }
        putchar('\n');
    }
}


// Alloca la matrice dinamicamente, ricordarsi la free()
float *creaMatrice(unsigned righe, unsigned colonne){
    unsigned riga, colonna;

    if(righe == 1)
        printf("Creazione vettore di dimensione %d\n", colonne);
    else
        printf("Creazione matrice %d*%d\n", righe, colonne);

    float *matrice = (float*)malloc(righe * colonne * sizeof(float));

    for(riga=0; riga<righe; riga++){
        for(colonna=0; colonna<colonne; colonna++){
            printf("M[%d,%d] = ", riga+1, colonna+1);
            scanf("%f", &matrice[riga*colonne + colonna]);
        }
    }

    return matrice;
}


// Alloca la matrice dinamicamente, ricordarsi la free()
float *creaMatriceVuota(unsigned righe, unsigned colonne){
    unsigned dim = righe * colonne * sizeof(float);
    float *matrice = (float*)malloc(dim);
    memset(matrice, 0, dim);
    return matrice;
}

int chiediInt(char *richiesta){
    int n;
    printf(richiesta);
    scanf("%d", &n);
    return n;
}


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
    stampaMatrice("Vettore:", x, 1, MV_N);

    cblas_sgemv(CblasRowMajor, CblasNoTrans, MV_M, MV_N, a, A, MV_LDA, x, MV_INC_X, b, y, MV_INC_Y); // https://software.intel.com/en-us/node/520750

    stampaMatrice("Risultato:", y, 1, MV_M);
}


void moltiplicaMatriceVettoreInterattivo(){
    lapack_int m = chiediInt("Numero righe della matrice? ");
    lapack_int n = chiediInt("Numero colonne della matrice? ");
    float *A = creaMatrice(m,n);
    float *x = creaMatrice(1, n);
    float *y = creaMatriceVuota(1, m);
    float a = 1, b = 1;

    stampaMatrice("Matrice:", A, m, n);
    stampaMatrice("Vettore:", x, 1, n);

    cblas_sgemv(CblasRowMajor, CblasNoTrans, m, n, a, A, n, x, 1, b, y, 1); // https://software.intel.com/en-us/node/520750

    stampaMatrice("Risultato:", y, 1, m);

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
    lapack_int m  = chiediInt("Numero righe della prima matrice? ");
    lapack_int n = chiediInt("Numero colonne della seconda matrice? ");
    lapack_int k = chiediInt("Dimensione comune? ");
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


/**
 * http://www.netlib.org/lapack/explore-html/d0/db8/group__real_g_esolve_ga3b05fb3999b3d7351cb3101a1fd28e78.html#ga3b05fb3999b3d7351cb3101a1fd28e78
 * http://www.netlib.org/lapack/explore-3.1.1-html/sgesv.f.html
 * http://www.netlib.org/lapack/lug/node38.html
 * http://www.netlib.org/lapack/lapacke.html
 * https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_sgesv_col.c.htm
 * https://software.intel.com/en-us/node/520973
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
