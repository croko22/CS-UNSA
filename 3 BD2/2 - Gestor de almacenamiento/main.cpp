#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include "GestorAlmacenamiento.h"
using namespace std;

constexpr int PAGE_SIZE = 4096;

int main()
{
    GestorAlmacenamiento fileManager("file.txt");

    vector<char> buffer(PAGE_SIZE);

    // Read the first page.
    fileManager.leerPrimerBloque(buffer);
    cout << "First page: " << endl;
    cout << buffer.data() << endl;

    // Read the current page.
    // fileManager.leerBloqueActual(buffer);

    // Read the next page.
    // fileManager.leerSiguienteBloque(buffer);
    // cout << "Next page: " << endl;
    // cout << buffer.data() << endl;

    // // Read the last page.
    // cout << "Last page: " << endl;
    // fileManager.leerUltimoBloque(buffer);
    // cout << buffer.data() << endl;

    // Write a new page.
    string s = "Hello world!";
    vector<char> v(s.begin(), s.end());
    // vector<char> v(PAGE_SIZE, '7');
    fileManager.escribirBloque(1, v);
    fileManager.escribirBloqueActual(v);

    // Add a new empty page.
    // fileManager.agregarBloqueVacio();

    return 0;
}
