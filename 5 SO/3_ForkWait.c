#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
        // Imprime el PID del proceso hijo
        fprintf(stdout, "PID hijo: %ld\n", (long)getpid());
        // Duerme 2 segundos
        sleep(2);
    }
    else
    { /* Código PADRE */
        // Espera a que termine el proceso hijo
        if ((x = wait(&estado)) != pid_hijo)
        {
            // El proceso padre fue interrumpido por una señal
            fprintf(stdout, "PADRE: interrumpido por señal\n");
        }
        else
        {
            // Imprime el PID del proceso padre, el PID del proceso hijo y el estado del proceso hijo
            fprintf(stdout, "PID padre: %ld / PID hijo: %ld / estado hijo: %d\n", (long)getpid(), (long)pid_hijo, estado);
        }
    }

    // Sale con código de éxito
    exit(0);
}
