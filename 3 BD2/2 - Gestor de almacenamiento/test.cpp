#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    fstream file("myfile.txt");

    if (!file.is_open())
    {
        cout << "Could not open file";
        return 1;
    }

    char buffer[1024];
    // size_t bytes_written;

    // vector<char> v(data.begin(), data.end());
    vector<char> v(100, '7');
    file.write(v.data(), v.size());
    // if (bytes_written != data.size())
    // {
    //     cout << "Could not write all data to file";
    //     return 1;
    // }

    file.close();

    return 0;
}
