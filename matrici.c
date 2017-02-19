#include "matrici.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
