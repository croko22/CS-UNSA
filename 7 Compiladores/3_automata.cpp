#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <exception>

using namespace std;

enum State
{
    START,
    IDENTIFIER,
    NUMBER,
    ERROR
};

int main()
{
    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line))
    {
        State state = START;
        cout << "Estado inicial: " << state + 1 << endl;
        string lexeme;

        for (char c : line)
        {
            cout << "Estado actual: " << state + 1 << endl;
            switch (state)
            {
            case START:
                if (isalpha(c))
                {
                    state = IDENTIFIER;
                    lexeme += c;
                }
                else if (isdigit(c))
                {
                    state = NUMBER;
                    lexeme += c;
                }
                break;
            case IDENTIFIER:
                if (isalnum(c))
                {
                    lexeme += c;
                }
                else
                {
                    cout << "Indentificador: " << lexeme << endl;
                    lexeme = "";
                    state = START;
                }
                break;
            case NUMBER:
                if (isdigit(c))
                {
                    lexeme += c;
                }
                else if (isalpha(c))
                {
                    lexeme += c;
                    state = ERROR;
                }
                break;
            }
        }

        if (state == IDENTIFIER)
        {
            cout << "Indentificador: " << lexeme << endl;
        }
        else if (state == NUMBER)
        {
            cout << "Numero: " << lexeme << endl;
        }
        else if (state == ERROR)
        {
            cout << "Error: " << lexeme << endl;
        }
    }

    // Close the file
    file.close();

    return 0;
}
