#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    // Declaracion de variables
    pid_t pid_hijo;
    int estado, x;
    long i, j;

    // Crea un nuevo proceso usando fork()
    if ((pid_hijo = fork()) == -1)
    { /* Código PADRE: Error */
        // Imprime un mensaje de error y sale con código de error
        perror("Fallo al hacer fork()");
        exit(-1);
    }
    else if (pid_hijo == 0)
    { /* Código HIJO */
        // Ejecuta el comando "ls -l" en el proceso hijo
        if (execl("/bin/ls", "ls", "-l", NULL) < 0)
        {
            // Imprime un mensaje de error y sale con código de error
            perror("Fallo al ejecutar: ls");
            exit(-1);
        }
    }
    else /* Código PADRE */
        if ((x = wait(&estado)) != pid_hijo)
        {
            // El proceso padre fue interrumpido por una señal
            fprintf(stdout, "PADRE: interrumpido por señal\n");
            fflush(stdout);
        }
    exit(0); /* Código PADRE e HIJO, aunque el hijo nunca pasará por aquí */
}