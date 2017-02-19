#include "matrici.h"
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    char buffer = 0;
    while(1){
        printf("< V:vett*matr | M:matr*matr | S:sistema | I:inversa | A:autovettori | EOF:esci > ");

        do {
            buffer = getchar();
        } while (isspace(buffer));

        switch(buffer){
        case 'V':
        case 'v':
            //moltiplicaMatriceVettore();
            moltiplicaMatriceVettoreInterattivo();
            break;

        case 'M':
        case 'm':
            //moltiplicaMatrici();
            moltiplicaMatriciInterattivo();
            break;

        case 'S':
        case 's':
            //sistemaLineare();
            sistemaLineareInterattivo();
            break;

        case 'I':
        case 'i':
            //inversa();
            inversaInterattivo();
            break;

        case 'A':
        case'a':
            //autovettori();
            autovettoriInterattivo();
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
