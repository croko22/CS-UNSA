#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "defines.h"

char *orden;
char *argumentos[MAXARG];
int narg;
int es_background;

void construye_orden(char *argv[]);

int main(int argc, char *argv[])
{
    int i;
    int status;

    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s fich_a_visualizar\n", argv[0]);
        exit(1);
    }

    construye_orden(argv);

    if (fork() == 0)
    { /* CODIGO DEL HIJO */
        execvp(orden, argumentos);
        fprintf(stderr, "%s no encontrado o no ejecutable\n", orden);
        exit(1);
    }

    /* CODIGO DEL PADRE*/
    wait(&status);
    exit(1);
}

void construye_orden(char *argv[])
{
    int i, j;

    narg = 1;
    es_background = FALSE;
    for (j = 0; j < MAXARG; j++)
        argumentos[j] = NULL;

    /* Atencion: La asignación de la cadena "cat" a 'orden' que está declarado como
    un 'char *' se trata como estática, es decir, en tiempo de compilación, el compilador
    reserva espacio de memoria suficiente para almacenar la cadena "cat" (4 bytes). */

    orden = "cat";

    /* Atención: Por contra, el array de cadenas 'argumentos' no se trata como una
    variable estática, y por eso, es responsabilidad del programador reservar memoria
    para las posiciones que se vayan a utilizar (en el ejemplo, 0 y 1) */
    argumentos[0] = (char *)malloc(strlen(orden) + 1);
    strcpy(argumentos[0], orden);
    argumentos[1] = (char *)malloc(strlen(argv[1]) + 1);
    strcpy(argumentos[1], argv[1]);
}