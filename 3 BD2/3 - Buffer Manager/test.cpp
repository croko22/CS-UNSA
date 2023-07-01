#include <iostream>
#include <fstream>

using namespace std;
//* IOS::CUR MOLA
int main()
{
    // TODO: Mover esto a otro folder
    //  TODO: Read the titanic csv
    //  Useful links
    //  https://classroom.google.com/u/0/c/NjEwNTMzOTQzNjgz/a/NjE1MjgwNDQ0ODAw/details
    //   https://github.com/datasciencedojo/datasets/blob/master/titanic.csv
    //   https://stackoverflow.com/questions/67206032/c-reading-a-table-from-a-text-file
    //   https://cplusplus.com/reference/ostream/ostream/put/
    //   https://java2blog.com/read-csv-file-in-cpp/
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