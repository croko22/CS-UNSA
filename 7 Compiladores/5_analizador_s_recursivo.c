#include <stdio.h>

int preanalisis;

// rutinas auxiliares
void error()
{
    printf("Error de sintaxis\n");
}

void parea(int t)
{
    if (preanalisis == t)
        preanalisis = getchar();
    else
        error();
}

// rutinas de reglas
void A()
{
    if (preanalisis == 'a')
        parea('a');
    else
        error();
}

void B()
{
    if (preanalisis == 'b')
        parea('b');
    else
        error();
}

void S()
{
    if (preanalisis == 'x')
    {
        parea('x');
        S();
    }
    else if (preanalisis == 'a')
    {
        A();
        B();
        parea('c');
    }
    else
        error();
}

int main()
{
    preanalisis = getchar();
    // La funci�n getchar se puede utilizar
    // para leer cadenas de varios
    // caracteres, leyendo en un bucle
    // la cadena car�cter a car�cter
    S();
}