#include <stdio.h>

struct Process
{
    int arrival_time;
    int burst_time;
    int waiting_time;
};

int compare(const void *a, const void *b)
{
    struct Process *p1 = (struct Process *)a;
    struct Process *p2 = (struct Process *)b;
    return p1->burst_time - p2->burst_time;
}

int main()
{
    int n, i, j;
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    printf("Ingrese el número de procesos: ");
    scanf("%d", &n);
    struct Process processes[n];
    for (i = 0; i < n; i++)
    {
        printf("Ingrese el tiempo de llegada y el tiempo de servicio del proceso %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
    }
    qsort(processes, n, sizeof(struct Process), compare);
    processes[0].waiting_time = 0;
    for (i = 1; i < n; i++)
    {
        processes[i].waiting_time = 0;
        for (j = 0; j < i; j++)
        {

            processes[i].waiting_time += processes[j].burst_time;
        }

        avg_waiting_time += processes[i].waiting_time;
    }

    avg_waiting_time /= n;

    for (i = 0; i < n; i++)
    {

        avg_turnaround_time += processes[i].burst_time + processes[i].waiting_time;
    }

    avg_turnaround_time /= n;

    printf("\nProceso\tTiempo de llegada\tTiempo de servicio\tTiempo de espera\tTiempo de respuesta\n");

    for (i = 0; i < n; i++)
    {

        printf("%d\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\n", i + 1, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].burst_time + processes[i].waiting_time);
    }

    printf("\nTiempo de espera promedio: %f\n", avg_waiting_time);

    printf("Tiempo de respuesta promedio: %f\n", avg_turnaround_time);

    return 0;
}