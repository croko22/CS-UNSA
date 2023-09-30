#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;   // Identificador de proceso
    int var = 0; // Variable compartida entre el padre y el hijo

    // PID del padre antes de hacer fork()
    printf("PID antes de fork(): %d\n", (int)getpid());

    // Creación de un proceso hijo
    if ((pid = fork()) > 0)
    {
        // PID del padre
        printf("PID del padre: %d\n", (int)getpid());
        var++; // Incrementa la variable compartida
    }
    else
    {
        // PID del hijo
        if (pid == 0)
        {
            printf("PID del hijo: %d\n", (int)getpid());
        }
        else
        {
            printf("Error al hacer fork()\n");
        }
    }

    // Impresión del PID del proceso actual y el valor de la variable compartida
    printf("Proceso [%d] -> var = %d\n", (int)getpid(), var);

    return 0;
}
