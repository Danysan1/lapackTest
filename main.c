#include "matrici.h"
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
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
