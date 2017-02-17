#include <stdio.h>
#include <string.h>
#include <lapacke/lapacke.h> // https://michaellindon.github.io/lindonslog/programming/atlas-blas-lapack-linear-algebra-libraries/
#include <cblas.h>

void stampaMatrice(float *matrice, unsigned righe, unsigned colonne);
float *creaMatrice(unsigned righe, unsigned colonne);

void sistemaLineare();
void sistemaLineareInterattivo();
void inversa();
void inversaInterattivo();

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    //sistemaLineare();
    sistemaLineareInterattivo();

    //inversa();
    //inversaInterattivo();

    return 0;
}

void stampaMatrice(float matrice[], unsigned righe, unsigned colonne){
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
    printf("Creazione matrice %d*%d\n", righe, colonne);
    float *matrice = (float*)malloc(righe * colonne * sizeof(float));

    for(riga=0; riga<righe; riga++){
        printf("Riga %d: ",riga);
        for(colonna=0; colonna<colonne; colonna++){
            scanf("%f", &matrice[riga*colonne + colonna]);
        }
    }

    return matrice;
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
    float a[SIS_LDA*SIS_N] = {1,1, 1,
                              1,2,-1,
                              3,3,-1};

    float b[SIS_LDB*SIS_N] = {4,
                              1,
                              4};

    puts("\nMatrice A:");
    stampaMatrice(a, SIS_N, SIS_LDA);
    puts("\nMatrice B:");
    stampaMatrice(b, SIS_N, SIS_LDB);

    lapack_int indiciPivot[SIS_N];
    lapack_int info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, SIS_N, SIS_NRHS, a, SIS_LDA, indiciPivot, b, SIS_LDB);
    if(info)
        printf("\nErrore %d",info);
    else {
        puts("\nRisultato:");
        stampaMatrice(b, SIS_N, SIS_LDB);
    }
}

void sistemaLineareInterattivo(){
    lapack_int n;
    printf("Numero incognite? ");
    scanf("%d", &n);

    float *a = creaMatrice(n,n),
            *b = creaMatrice(n,1);

    puts("\nMatrice A:");
    stampaMatrice(a, n, n);
    puts("\nMatrice B:");
    stampaMatrice(b, n, 1);

    const lapack_int lda=n, nrhs=1, ldb=nrhs;
    lapack_int *indiciPivot = (int*)malloc(n*sizeof(int));
    lapack_int info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, n, nrhs, a, lda, indiciPivot, b, ldb);
    if(info)
        printf("\nErrore %d",info);
    else {
        puts("\nRisultato:");
        stampaMatrice(b, n, 1);
    }

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
#define INV_M 3 // righe di A
#define INV_N 3 // colonne di A
#define INV_LDA INV_M // dimensione effettiva di A
    float a[INV_LDA * INV_N] = {1, 1, 0,
                                0, 1, 0,
                                2, 1, 1};

    puts("\nMatrice:");
    stampaMatrice(a, INV_M, INV_N);

    lapack_int indiciPivot[INV_N];
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, INV_M,INV_N, a, INV_LDA, indiciPivot);
    if(info){
        printf("\nErrore di fattorizzazione %d, matrice non invertibile\n",info);
    } else {
        info = LAPACKE_sgetri(LAPACK_ROW_MAJOR, INV_N, a, INV_LDA, indiciPivot);
        if(info)
            printf("\nErrore %d",info);
        else {
            puts("\nInversa:");
            stampaMatrice(a, 3, 3);
        }
    }
}

void inversaInterattivo(){
    lapack_int n;
    printf("Ordine della matrice d invertire? ");
    scanf("%d", &n);

    float *a = creaMatrice(n,n);

    puts("\nMatrice A:");
    stampaMatrice(a, n, n);

    const lapack_int lda=n;
    lapack_int *indiciPivot = (int*)malloc(n*sizeof(int));
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n, n, a, lda, indiciPivot);
    if(info){
        printf("\nErrore di fattorizzazione %d, matrice non invertibile\n",info);
    } else {
        info = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n, a, lda, indiciPivot);
        if(info)
            printf("\nErrore %d",info);
        else {
            puts("\nInversa:");
            stampaMatrice(a, 3, 3);
        }
    }

    free(a);
    free(indiciPivot);
}
