#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAS '+'
#define MENOS '-'
#define MULT '*'
#define DIV '/'
#define NUM 256
#define FIN -1

char lexema[80];
int tok;
FILE *archivo;
int lineIndex;
char linea[1024];
void parea(int);
void error();
void Resto();
void Term();
void Exp();
void ListaExp();
int scanner();

#define MAX 100

typedef struct
{
    int items[MAX];
    int top;
} Stack;

void initStack(Stack *s)
{
    s->top = -1;
}

int isEmpty(Stack *s)
{
    return s->top == -1;
}

void push(Stack *s, int value)
{
    if (s->top < MAX - 1)
    {
        s->items[++s->top] = value;
    }
    else
    {
        printf("Stack full\n");
        exit(1);
    }
}

int pop(Stack *s)
{
    if (!isEmpty(s))
    {
        return s->items[s->top--];
    }
    else
    {
        printf("Stack empty\n");
        exit(1);
    }
}

int evaluatePostfix(char *expr)
{
    Stack s;
    initStack(&s);
    char *token = strtok(expr, " ");

    while (token != NULL)
    {
        if (isdigit(token[0]))
        {
            push(&s, atoi(token));
        }
        else
        {
            int val1 = pop(&s);
            int val2 = pop(&s);
            switch (token[0])
            {
            case '+':
                push(&s, val2 + val1);
                break;
            case '-':
                push(&s, val2 - val1);
                break;
            case '*':
                push(&s, val2 * val1);
                break;
            case '/':
                push(&s, val2 / val1);
                break;
            }
        }
        token = strtok(NULL, " ");
    }
    return pop(&s);
}

void Exp()
{
    Term();
    Resto();
    printf("\n");
}

void Resto()
{
    if (tok == MAS)
    {
        parea(MAS);
        Term();
        printf("+ ");
        Resto();
    }
    else if (tok == MENOS)
    {
        parea(MENOS);
        Term();
        printf("- ");
        Resto();
    }
}

void Term()
{
    if (tok == NUM)
    {
        printf("%s ", lexema);
        parea(NUM);
    }
    else if (tok == '(')
    {
        parea('(');
        Exp();
        parea(')');
    }
    else
    {
        error();
    }
}

void error()
{
    printf("Error de sintaxis\n");
    exit(1);
}

void parea(int t)
{
    if (tok == t)
    {
        tok = scanner();
    }
    else
    {
        error();
    }
}

int scanner()
{
    int c, i;
    do
    {
        c = linea[lineIndex++];
    } while (c == ' ');

    if (c == '\0')
    {
        return FIN;
    }
    if (c == MAS || c == MENOS || c == '*' || c == '/')
    {
        return c;
    }
    if (isdigit(c))
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = linea[lineIndex++];
        } while (isdigit(c));
        lexema[i] = 0;
        lineIndex--;
        return NUM;
    }
    if (c == '(' || c == ')')
    {
        return c;
    }
    return FIN;
}

void ListaExp()
{
    if (tok != FIN)
    {
        Exp();
        if (tok == ';')
        {
            parea(';');
            ListaExp();
        }
    }
}

int main()
{
    archivo = fopen("programa1.txt", "r");
    if (!archivo)
    {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    while (fgets(linea, sizeof(linea), archivo))
    {
        // Eliminar el salto de línea al final
        linea[strcspn(linea, "\n")] = 0;
        lineIndex = 0;

        // Iniciar el proceso de análisis sintáctico
        tok = scanner();
        ListaExp();
    }

    fclose(archivo);
    return EXIT_SUCCESS;
}
