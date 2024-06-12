#include <stdio.h>

int preanalisis;

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

void Resto();

void Term()
{
    if (preanalisis >= '0' && preanalisis <= '9')
    {
        printf("%c", preanalisis);
        parea(preanalisis);
    }
    else if (preanalisis == '+' || preanalisis == '-')
    {
        error();
    }
    else
    {
        error();
    }
}

void Exp()
{
    Term();
    Resto();
}

void Resto()
{
    if (preanalisis == '+')
    {
        parea('+');
        Term();
        printf("+");
        Resto();
    }
    else if (preanalisis == '-')
    {
        parea('-');
        Term();
        printf("-");
        Resto();
    }
    else if (preanalisis >= '0' && preanalisis <= '9')
    {
        error();
    }
    else if (preanalisis != '\n' && preanalisis != EOF)
    {
        error();
    }
}

int main()
{
    preanalisis = getchar();
    Exp();
    return 0;
}