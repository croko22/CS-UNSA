#include <stdio.h>

void main()
{
    // initlialize the variable name
    int i, NOP, sum = 0, count = 0, y, quant, wt = 0, tat = 0, at[10], bt[10], temp[10];
    float avg_wt, avg_tat;
    printf(" Número total de procesos en el sistema: ");
    scanf("%d", &NOP);
    y = NOP; // Asignar el número de procesos a la variable y

    // Usar un bucle for para ingresar los detalles del proceso como el tiempo de llegada y el tiempo de servicio
    for (i = 0; i < NOP; i++)
    {
        printf("\n Ingrese el tiempo de llegada y el tiempo de servicio del proceso [%d]\n", i + 1);
        printf(" El tiempo de llegada es: \t"); // Aceptar tiempo de llegada
        scanf("%d", &at[i]);
        printf(" \nEl tiempo de servicio es: \t"); // Aceptar el tiempo de servicio
        scanf("%d", &bt[i]);
        temp[i] = bt[i]; // almacenar el tiempo de servicio en la matriz temporal
    }
    // Aceptar el tiempo cuántico
    printf("Ingrese el quantum de tiempo para el proceso: \t");
    scanf("%d", &quant);
    // Mostrar el número de proceso, el tiempo de servicio, el tiempo de respuesta y el tiempo de espera
    printf("\n No de proceso \t\t Tiempo de servicio \t\t TAT \t\t Tiempo de espera ");
    for (sum = 0, i = 0; y != 0;)
    {
        if (temp[i] <= quant && temp[i] > 0) // definir las condiciones
        {
            sum = sum + temp[i];
            temp[i] = 0;
            count = 1;
        }
        else if (temp[i] > 0)
        {
            temp[i] = temp[i] - quant;
            sum = sum + quant;
        }
        if (temp[i] == 0 && count == 1)
        {
            y--; // decrementar el número de proceso
            printf("\nProceso No [%d] \t\t %d\t\t\t\t %d\t\t\t %d", i + 1, bt[i], sum - at[i], sum - at[i] - bt[i]);
            wt = wt + sum - at[i] - bt[i];
            tat = tat + sum - at[i];
            count = 0;
        }
        if (i == NOP - 1)
        {
            i = 0;
        }
        else if (at[i + 1] <= sum)
        {
            i++;
        }
        else
        {
            i = 0;
        }
    }
    // representa el tiempo de espera promedio y el tiempo de respuesta
    avg_wt = wt * 1.0 / NOP;
    avg_tat = tat * 1.0 / NOP;
    printf("\n Tiempo de respuesta promedio: \t%f", avg_wt);
    printf("\n Tiempo de espera promedio: \t%f", avg_tat);
}