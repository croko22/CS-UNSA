#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <exception>

using namespace std;

// Function to determine if a character is a separator
bool isSeparator(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

// Function to determine if a character is a special symbol
bool isSpecialSymbol(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')');
}

bool isFloat(const std::string &s)
{
    try
    {
        std::stof(s);
        return true;
    }
    catch (std::exception &e)
    {
        return false;
    }
}

int main()
{
    // Open the source code file
    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    // Read the file line by line
    while (getline(file, line))
    {
        stringstream ss(line);
        string word;
        // Process each word in the line
        while (ss >> word)
        {
            // If the word is a number or a float, print it accordingly
            if (isdigit(word[0]) || (word[0] == '-' && isdigit(word[1])))
            {
                if (isFloat(word))
                {
                    cout << "float = " << word << endl;
                }
                else
                {
                    cout << "number = " << word << endl;
                }
            }
            // If the word is a special symbol, print it as a symbol
            else if (isSpecialSymbol(word[0]))
            {
                cout << "symbol = " << word << endl;
            }
            // Otherwise, it's a word
            else
            {
                cout << "word = " << word << endl;
            }
        }
    }

    // Close the file
    file.close();

    return 0;
}
