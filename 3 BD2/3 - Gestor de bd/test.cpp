#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    fstream file("file.txt", fstream::binary);

    // Get the current position of the file pointer.
    int current_position = file.tellg();

    // Calculate the size of the last block of data.
    int block_size = 5;

    // Set the file pointer to the position of the last block of data.
    file.seekg(block_size);

    // Read the last block of data from the file.
    char buffer[block_size];
    file.read(buffer, block_size);

    // Print the last block of data.
    for (int i = 0; i < block_size; i++)
    {
        cout << buffer[i];
    }

    return 0;
}
