#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    int i, n = 4; // 2^4 = 16 procesos

    // Crea un ciclo para crear 4 procesos hijos usando fork()
    for (i = 0; i < n; i++)
    {
        // Crea un nuevo proceso usando fork()
        if ((pid = fork()) < 0)
        {
            // Error al hacer fork()
            break;
        }
    }

    // Imprime el PID del proceso y el PID del padre
    printf("Proceso: %d / Padre: %d\n", (int)getpid(), (int)getppid());

    return 0;
}