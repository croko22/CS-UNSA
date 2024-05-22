#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ID 256
#define NUM 257
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'
#define MAYOR '>'
#define MENOR '<'
#define MAYORIGUAL 258
#define PUNTOYCOMA ';'
#define PARI '('
#define WHILE 259
#define IF 260

int scaner();
void mostrar(int);
int espalres();

FILE *f;
char lexema[80];

void main(int n, char *pal[])
{
    int token;
    f = stdin;  // entrada estandar del teclado
    if (n == 2) // si se especifico un archivo de lectura
    {
        f = fopen(pal[1], "rt"); // lectura modo texto
        if (f == NULL)
            f = stdin;
    }
    if (f == stdin) // la lectura sera desde la entrada estandar
        printf("Ingrese texto ........ termine con Ctrl z \n");

    while (1)
    {
        token = scaner();
        if (token == EOF)
            break;
        mostrar(token);
    }

    if (f != stdin)
        fclose(f);
}

int scaner()
{
    int c;
    int i;
    do
        c = fgetc(f);
    while (isspace(c)); // ignora blancos

    if (c == EOF)
        return EOF;

    if (isalpha(c)) // regla del ID
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = fgetc(f);
        } while (isalnum(c) || c == '_');

        lexema[i] = 0;
        ungetc(c, f);   // se devuelve c al flujo de entrada
        i = espalres(); // verifica si es palabra reservada
                        //  WHILE , IF
        if (i >= 0)
            return i;
        return ID; // se trata de un ID
    }

    if (isdigit(c)) // regla del NUM
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = fgetc(f);
        } while (isdigit(c));

        lexema[i] = 0;
        ungetc(c, f);
        return NUM;
    }

    switch (c)
    {
    case '+':
        return PLUS;
    case '-':
        return MINUS;
    case '*':
        return MULT;
    case '/':
        return DIV;
    default:
        return c;
    }

    // regla de PUNTOYCOMA y PARI
    if ((c == ';') || (c == '('))
        return c; // regla del ";" y "("

    // regla de ">" o ">="
    if (c == '>')
    {
        c = fgetc(f);
        if (c == '=') // return MAYORIGUAL
        {
            lexema[0] = '>';
            lexema[1] = '=';
            lexema[2] = 0;
            return MAYORIGUAL;
        }
        ungetc(c, f);
        return MAYOR; // return MAYOR
    }
}

int espalres()
{
    if (strcmp(lexema, "while") == 0)
        return WHILE;
    if (strcmp(lexema, "if") == 0)
        return IF;

    return -1;
}

void mostrar(int token)
{

    switch (token)
    {
    case ID:
        printf("token = ID [%s] \n", lexema);
        break;
    case NUM:
        printf("token = NUM [%s] \n", lexema);
        break;
    case PLUS:
        printf("token = PLUS [%c] \n", token);
        break;
    case MINUS:
        printf("token = MINUS [%c] \n", token);
        break;
    case MULT:
        printf("token = MULT [%c] \n", token);
        break;
    case DIV:
        printf("token = DIV [%c] \n", token);
        break;
    case MAYORIGUAL:
        printf("token = MAYORIGUAL [%s] \n", lexema);
        break;
    case WHILE:
        printf("token = WHILE [%s] \n", lexema);
        break;
    case IF:
        printf("token = IF [%s] \n", lexema);
        break;
    case PARI:
        printf("token = PARI [%c] \n", token);
        break;
    case MAYOR:
        printf("token = MAYOR [%c] \n", token);
        break;
    case PUNTOYCOMA:
        printf("token = PUNTOYCOMA [%c] \n", token);
        break;
    }
}
