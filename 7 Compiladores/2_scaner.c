#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ID 256
#define NUM 257
//* Simbolos
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'

#define MAYOR '>'
#define MAYORIGUAL 258
#define MENOR '<'
#define MENORIGUAL 259
#define ASIGNACION '='
#define IGUALDAD 261
#define DIFERENTE 262

#define COMA ','
#define PUNTOYCOMA ';'
#define PARI '('
#define PARD ')'
#define CORI '['
#define CORD ']'
#define LLAVEI '{'
#define LLAVED '}'
//* Palabras reservadas
#define WHILE 263
#define IF 264
#define FOR 265
#define DO 266
#define INT 267
#define FLOAT 268
#define CHAR 269
#define RETURN 270
#define BREAK 271
#define CONTINUE 272
#define ELSE 273
#define SWITCH 274
#define CASE 275
#define DEFAULT 276
#define VOID 277
#define STATIC 278
#define CONST 279
#define SIZEOF 280
#define STRUCT 281
#define TYPEDEF 282

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
        memset(lexema, 0, sizeof(lexema));
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

    //* regla de comentario
    if (c == '/')
    {
        c = fgetc(f);
        if (c == '/') // single-line comment
        {
            while (c != '\n' && c != EOF)
                c = fgetc(f);
            if (c == EOF)
                return EOF;
            return scaner(); // continue scanning
        }
        else if (c == '*') // multi-line comment
        {
            while (1)
            {
                c = fgetc(f);
                if (c == EOF)
                    return EOF;
                if (c == '*')
                {
                    if ((c = fgetc(f)) == '/')
                        break;
                    else
                        ungetc(c, f);
                }
            }
            return scaner(); // continue scanning
        }
        else
        {
            ungetc(c, f);
            return DIV; // return '/' if not a comment
        }
    }

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
    case '>':
        if ((c = fgetc(f)) == '=')
        {
            lexema[0] = '>';
            lexema[1] = c;
            lexema[2] = '\0';
            return MAYORIGUAL;
        }
        else
            ungetc(c, f);
        return MAYOR;
    case '<':
        if ((c = fgetc(f)) == '=')
        {
            lexema[0] = '<';
            lexema[1] = c;
            lexema[2] = '\0';

            return MENORIGUAL;
        }
        else
            ungetc(c, f);
        return MENOR;
    case '=':
        if ((c = fgetc(f)) == '=')
        {
            lexema[0] = '=';
            lexema[1] = c;
            lexema[2] = '\0';
            return IGUALDAD;
        }
        else
            ungetc(c, f);
        return ASIGNACION;
    case '!':
        if ((c = fgetc(f)) == '=')
        {
            lexema[0] = '!';
            lexema[1] = c;
            lexema[2] = '\0';
            return DIFERENTE;
        }
        else
            ungetc(c, f);
        return '!';
    case ',':
        return COMA;
    case ';':
        return PUNTOYCOMA;
    case '(':
        return PARI;
    case ')':
        return PARD;
    case '[':
        return CORI;
    case ']':
        return CORD;
    case '{':
        return LLAVEI;
    case '}':
        return LLAVED;
    default:
        return c;
    }
}

int espalres()
{
    if (strcmp(lexema, "while") == 0)
        return WHILE;
    if (strcmp(lexema, "if") == 0)
        return IF;
    if (strcmp(lexema, "for") == 0)
        return FOR;
    if (strcmp(lexema, "do") == 0)
        return DO;
    if (strcmp(lexema, "int") == 0)
        return INT;
    if (strcmp(lexema, "float") == 0)
        return FLOAT;
    if (strcmp(lexema, "char") == 0)
        return CHAR;
    if (strcmp(lexema, "return") == 0)
        return RETURN;
    if (strcmp(lexema, "break") == 0)
        return BREAK;
    if (strcmp(lexema, "continue") == 0)
        return CONTINUE;
    if (strcmp(lexema, "else") == 0)
        return ELSE;
    if (strcmp(lexema, "switch") == 0)
        return SWITCH;
    if (strcmp(lexema, "case") == 0)
        return CASE;
    if (strcmp(lexema, "default") == 0)
        return DEFAULT;
    if (strcmp(lexema, "void") == 0)
        return VOID;
    if (strcmp(lexema, "static") == 0)
        return STATIC;
    if (strcmp(lexema, "const") == 0)
        return CONST;
    if (strcmp(lexema, "sizeof") == 0)
        return SIZEOF;
    if (strcmp(lexema, "struct") == 0)
        return STRUCT;
    if (strcmp(lexema, "typedef") == 0)
        return TYPEDEF;

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
    case MAYOR:
        printf("token = MAYOR [%c] \n", token);
        break;
    case MAYORIGUAL:
        printf("token = MAYORIGUAL [%s] \n", lexema);
        break;
    case MENOR:
        printf("token = MENOR [%c] \n", token);
        break;
    case MENORIGUAL:
        printf("token = MENORIGUAL [%s] \n", lexema);
        break;
    case ASIGNACION:
        printf("token = ASIGNACION [%c] \n", token);
        break;
    case IGUALDAD:
        printf("token = IGUALDAD [%s] \n", lexema);
        break;
    case DIFERENTE:
        printf("token = DIFERENTE [%s] \n", lexema);
    case COMA:
        printf("token = COMA [%c] \n", token);
    case PUNTOYCOMA:
        printf("token = PUNTOYCOMA [%c] \n", token);
        break;
    case PARI:
        printf("token = PARI [%c] \n", token);
        break;
    case PARD:
        printf("token = PARD [%c] \n", token);
        break;
    case CORI:
        printf("token = CORI [%c] \n", token);
        break;
    case CORD:
        printf("token = CORD [%c] \n", token);
        break;
    case LLAVEI:
        printf("token = LLAVEI [%c] \n", token);
        break;
    case LLAVED:
        printf("token = LLAVED [%c] \n", token);
        break;
    case WHILE:
        printf("token = WHILE [%s] \n", lexema);
        break;
    case IF:
        printf("token = IF [%s] \n", lexema);
        break;
    case FOR:
        printf("token = FOR [%s] \n", lexema);
        break;
    case DO:
        printf("token = DO [%s] \n", lexema);
        break;
    case INT:
        printf("token = INT [%s] \n", lexema);
        break;
    case FLOAT:
        printf("token = FLOAT [%s] \n", lexema);
        break;
    case CHAR:
        printf("token = CHAR [%s] \n", lexema);
        break;
    case RETURN:
        printf("token = RETURN [%s] \n", lexema);
        break;
    case BREAK:
        printf("token = BREAK [%s] \n", lexema);
        break;
    case CONTINUE:
        printf("token = CONTINUE [%s] \n", lexema);
        break;
    case ELSE:
        printf("token = ELSE [%s] \n", lexema);
        break;
    case SWITCH:
        printf("token = SWITCH [%s] \n", lexema);
        break;
    case CASE:
        printf("token = CASE [%s] \n", lexema);
        break;
    case DEFAULT:
        printf("token = DEFAULT [%s] \n", lexema);
        break;
    case VOID:
        printf("token = VOID [%s] \n", lexema);
        break;
    case STATIC:
        printf("token = STATIC [%s] \n", lexema);
        break;
    case CONST:
        printf("token = CONST [%s] \n", lexema);
        break;
    case SIZEOF:
        printf("token = SIZEOF [%s] \n", lexema);
        break;
    case STRUCT:
        printf("token = STRUCT [%s] \n", lexema);
        break;
    case TYPEDEF:
        printf("token = TYPEDEF [%s] \n", lexema);
        break;
    }
}
