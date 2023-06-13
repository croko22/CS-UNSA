#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const int BUFFER_SIZE = 1024;
std::vector<char> buffer(BUFFER_SIZE + 1, 0);

int main(int argc, char const *argv[])
{
    streampos begin, end;
    // ofstream myfile("filename.txt");
    ifstream myfile("filename.txt");
    // myfile << "Files can be tricky, but it is fun enough!";

    begin = myfile.tellg();
    myfile.seekg(0, ios::end);
    end = myfile.tellg();

    myfile.close();

    cout << "size is: " << (end - begin) << " bytes.\n";
    return 0;
}