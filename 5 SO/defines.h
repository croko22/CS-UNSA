/*  defines . H   */
#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

/* Valores enteros asignados a verdadero, falso, ejcucion */
/* correcta y ejecucion con error.                        */

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0

/* Longitud maxima de la linea de ordenes.             */
#define MAXLINE 200
/* Numero maximo de argumentos para cada orden simple. */
#define MAXARG 20
/* LOGO que aparece al empezar e ejecutar el minishell */
#define LOGO "MINI SHELL  (c)2004 Tu nombre\n"
/* Cadena que aparece como PROMPT en la linea de comandos */
#define PROMPT "msh_$ "

#endif