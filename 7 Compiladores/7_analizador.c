#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAS '+'
#define MENOS '-'
#define NUM 256
#define FIN -1
char lexema[80];
int tok;
void parea(int);
void error();
void Resto();
void Term();
int scanner();

//* Ejercicio A)
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
    if (tok == NUM)
    {
        Term();
        Resto();
    }
    else
        error();
}
void Resto()
{
    if (tok == MAS)
    {
        parea(MAS);
        Term();
        printf("+");
        Resto();
    }
    else if (tok == MENOS)
    {
        parea(MENOS);
        Term();
        printf("-");
        Resto();
    }
    else

        ;
}
void Term()
{
    if (tok == NUM)
    {
        printf("%s ", lexema);
        parea(NUM);
    }
    else
        error();
}
void error()
{
    printf("Error de sintaxis");
}
void parea(int t)
{
    if (tok == t)
        tok = scanner();
    else
        error();
}
int scanner()
{
    int c, i;
    do
        c = getchar();
    while (c == ' ');
    if (c == '\n')
        return FIN;
    if (c == MAS || c == MENOS)
        return c;
    if (isdigit(c))
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = getchar();
        } while (isdigit(c));
        lexema[i] = 0;
        ungetc(c, stdin);
        return NUM;
    }
    return FIN;
}

int main()
{
    // tok = scanner();
    // //*lexema print
    // printf("Lexema: %s\n", lexema);
    // //* token print
    // printf("Token: %d\n", tok);

    // Exp();
    // printf("\n");

    char expr[100];
    printf("Enter postfix expression: ");
    fgets(expr, 100, stdin);
    expr[strcspn(expr, "\n")] = 0; // Remove newline character
    printf("Result: %d\n", evaluatePostfix(expr));
    return 0;
}