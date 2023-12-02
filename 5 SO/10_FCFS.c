#include <stdio.h>

// Función para encontrar el tiempo de espera de todos los procesos
void encontrarTiempoDeEspera(int procesos[], int n, int bt[], int wt[])
{
    // El tiempo de espera para el primer proceso es 0
    wt[0] = 0;

    // Calculando el tiempo de espera
    for (int i = 1; i < n; i++)
    {
        wt[i] = bt[i - 1] + wt[i - 1];
    }
}

// Función para calcular el tiempo de respuesta
void encontrarTiempoDeRespuesta(int procesos[], int n, int bt[], int wt[], int tat[])
{
    for (int i = 0; i < n; i++)
    {
        tat[i] = bt[i] + wt[i];
    }
}

// Función para calcular el tiempo promedio
void encontrarTiempoPromedio(int procesos[], int n, int bt[])
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    // Función para encontrar el tiempo de espera de todos los procesos
    encontrarTiempoDeEspera(procesos, n, bt, wt);

    // Función para encontrar el tiempo de respuesta para todos los procesos
    encontrarTiempoDeRespuesta(procesos, n, bt, wt, tat);

    // Muestra los procesos junto con todos los detalles
    printf("Proceso | Tiempo de ráfaga | Tiempo de espera | Tiempo de respuesta\n");

    // Calcula el tiempo de espera total y el tiempo de respuesta total
    for (int i = 0; i < n; i++)
    {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf(" P%d", (i + 1));
        printf("\t\t%d\t", bt[i]);
        printf("\t%d", wt[i]);
        printf("\t\t\t%d\n", tat[i]);
    }

    float s = (float)total_wt / (float)n;
    float t = (float)total_tat / (float)n;
    printf("Tiempo de espera promedio = %f", s);
    printf("\n");
    printf("Tiempo de respuesta promedio = %f ", t);
}

int main()
{
    int procesos[] = {1, 2, 3, 4, 5};
    int n = sizeof procesos / sizeof procesos[0];

    // Tabla 1
    int tiempoDeRáfaga[] = {10, 1, 2, 1, 5};
    // Tabla 2
    // int tiempoDeRáfaga[] = {4, 6, 8, 4, 4};

    encontrarTiempoPromedio(procesos, n, tiempoDeRáfaga);
    return 0;
}
