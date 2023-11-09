void *hola(void *arg)
{
    /* Esta función imprime el mensaje "Hola", carácter por carácter, con un retraso de 1 segundo entre cada carácter. */

    char *msg = "Hola";
    int i;
    for (i = 0; i < strlen(msg); i++)
    {
        printf("%c", msg[i]);
        fflush(stdout);
        usleep(1000000);
    }
    return NULL;
}

void *mundo(void *arg)
{
    /* Esta función imprime el mensaje "mundo", carácter por carácter, con un retraso de 1 segundo entre cada carácter. */

    char *msg = "mundo";

    int i;
    for (i = 0; i < strlen(msg); i++)
    {
        printf("%c", msg[i]);
        fflush(stdout);
        usleep(1000000);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    /* Esta función crea dos hilos, uno para ejecutar la función `hola()` y otro para ejecutar la función `mundo()`. Luego, espera a que ambos hilos terminen antes de continuar. */

    pthread_t h1;
    pthread_t h2;

    pthread_create(&h1, NULL, hola, NULL);
    pthread_create(&h2, NULL, mundo, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    printf(" Fin \n ");
}
