#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Declare the function prototype for get_processes
int get_processes(int *pids, int size);

int main()
{
    // Obtiene la lista de procesos.
    int pids[1024];
    int n = get_processes(pids, sizeof(pids));

    // Imprime la lista de procesos.
    for (int i = 0; i < n; i++)
    {
        printf("PID: %d\n", pids[i]);
    }

    return 0;
}

int get_processes(int *pids, int size)
{
    // Abre el archivo /proc/stat.
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
    {
        return -1;
    }

    // Lee la primera línea del archivo.
    char line[1024];
    fgets(line, sizeof(line), fp);

    // Extrae los PIDs de los procesos en ejecución.
    for (int i = 0; i < size; i++)
    {
        // Busca el siguiente PID.
        char *p = strstr(line, " ");
        if (!p)
        {
            break;
        }

        // Extrae el PID.
        pids[i] = atoi(p + 1);

        // Avanza al siguiente PID.
        line = p + 1;
    }

    // Cierra el archivo.
    fclose(fp);

    int i = 0;
    // Rest of your code...
    return i;
}