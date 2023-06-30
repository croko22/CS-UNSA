#include <iostream>
#include <fstream>

using namespace std;
//* IOS::CUR MOLA
int main()
{
    fstream file("file.txt");
    // Set the file pointer to the current position
    char c;
    for (int i = 0; i < 10; i++)
    {
        file.seekg(0, ios::cur);
        file.get(c);
        cout << c;
    }

    return 0;
}