#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <exception>

using namespace std;

// Define the states
const int START = 0;
const int ERROR = 1;
const int IDENTIFIER = 2;
const int ACCEPT = 4;

// Define the inputs
const int LETTER = 0;
const int DIGIT = 1;
const int FDC = 2;

//* TABLA
int state_transition_table[3][3] = {
    {IDENTIFIER, ERROR, ERROR},       // START
    {ERROR, ERROR, ERROR},            // ERROR
    {IDENTIFIER, IDENTIFIER, ACCEPT}, // IDENTIFIER
};
bool break_flag = false;

int main()
{
    ifstream file("test2.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line))
    {
        int state = START;
        string lexeme;

        for (char c : line)
        {
            cout << "Estado actual: " << state + 1 << endl;
            int input;
            if (isalpha(c))
            {
                input = LETTER;
            }
            else if (isdigit(c))
            {
                input = DIGIT;
            }
            else
            {
                input = FDC;
            }

            int next_state = state_transition_table[state][input];
            if (next_state == ERROR)
            {
                cout << "Error" << lexeme << endl;
                lexeme = "";
                // state = START;
                // break_flag = true;
                break;
            }
            else
            {
                lexeme += c;
                state = next_state;
            }
        }

        if (state == IDENTIFIER)
        {
            cout << "Indentificador: " << lexeme << endl;
        }
        else if (state == ERROR)
        {
            cout << "Error" << lexeme << endl;
        }
    }

    // Close the file
    file.close();

    return 0;
}
