#ifndef MATRICI_H
#define MATRICI_H

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

#endif // MATRICI_H
