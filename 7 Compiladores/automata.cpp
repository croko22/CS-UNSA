#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

enum TokenType
{
    WORD,
    NUMBER,
    SYMBOL,
    UNKNOWN
};

struct Token
{
    TokenType type;
    string value;
};

// Function to determine if a character is a separator
bool isSeparator(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

// Function to determine if a character is a special symbol
bool isSpecialSymbol(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';');
}

Token getTokenType(string token)
{
    Token result;
    if (isalpha(token[0]))
    {
        result.type = WORD;
    }
    else if (isdigit(token[0]))
    {
        result.type = NUMBER;
    }
    else if (isSpecialSymbol(token[0]))
    {
        result.type = SYMBOL;
    }
    else
    {
        result.type = UNKNOWN;
    }
    result.value = token;
    return result;
}

void printToken(Token token)
{
    switch (token.type)
    {
    case WORD:
        cout << "palabra = " << token.value << endl;
        break;
    case NUMBER:
        cout << "numero = " << token.value << endl;
        break;
    case SYMBOL:
        cout << "simbolo = " << token.value << endl;
        break;
    default:
        break;
    }
}

int main()
{
    ifstream file("test.txt");
    if (!file)
    {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string token;
    char c;
    while (file.get(c))
    {
        if (isSeparator(c) || isSpecialSymbol(c))
        {
            if (!token.empty())
            {
                printToken(getTokenType(token));
                token.clear();
            }
            if (isSpecialSymbol(c))
            {
                token = c;
                printToken(getTokenType(token));
                token.clear();
            }
        }
        else
        {
            token += c;
        }
    }

    if (!token.empty())
    {
        printToken(getTokenType(token));
    }

    file.close();
    return 0;
}
