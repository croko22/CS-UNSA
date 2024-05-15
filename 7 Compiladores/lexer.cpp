#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main()
{
    // Input source code string
    string source_code = "valor = valor + inc;";

    // Define a single regex pattern for both tokens
    regex combined_pattern("([a-zA-Z_][a-zA-Z0-9_]*)|([-=+;])");

    // Define a regex iterator
    sregex_iterator next(source_code.begin(), source_code.end(), combined_pattern);
    sregex_iterator end;

    // Loop through the source code string
    while (next != end)
    {
        smatch match = *next;
        string lexeme = match.str();

        // Match the lexeme with respective token pattern
        if (regex_match(lexeme, regex("[a-zA-Z_][a-zA-Z0-9_]*")))
        {
            cout << "Id \"" << lexeme << "\"" << endl;
        }
        else if (regex_match(lexeme, regex("[-=+;]")))
        {
            cout << "Special char \"" << lexeme << "\"" << endl;
        }

        next++;
    }

    return 0;
}