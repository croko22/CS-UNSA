#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const int BUFFER_SIZE = 1024;
std::vector<char> buffer(BUFFER_SIZE + 1, 0);

// TODO: Useful links
// https://marketsplash.com/tutorials/cpp/cplusplus-ifstream/
// https://stackoverflow.com/questions/18816126/c-read-the-whole-file-in-buffer
//  https://opendsa-server.cs.vt.edu/OpenDSA/Books/PIFLAS21/html/index.html
//  https://github.com/krshrimali/CPP-File-Manager/blob/master/src/FileManager.cpp
//  https://gist.github.com/andschwa/d120be76ba8ebd66cf50
//  https://codereview.stackexchange.com/questions/270928/reading-from-large-binary-files
//  https://cplusplus.com/doc/tutorial/files/
//  https://stackoverflow.com/questions/20911584/how-to-read-a-file-in-multiple-chunks-until-eof-c

int main(int argc, char const *argv[])
{
    std::ifstream file("filename.txt", std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        std::cout << "read " << size << " bytes\n";
    }
    else
    {
        std::cout << "error: only " << file.gcount() << " could be read\n";
    }

    return 0;
}