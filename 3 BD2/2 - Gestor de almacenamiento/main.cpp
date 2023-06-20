#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

constexpr int PAGE_SIZE = 4096;

class FileManager
{
public:
    FileManager(const string &filename)
    {
        file.open(filename, ios::in | ios::out | ios::binary);
        if (!file.is_open())
        {
            throw runtime_error("Could not open file");
        }

        // Read the number of pages from the beginning of the file.
        file.seekg(0, ios::beg);
        int numPages;
        file.read((char *)&numPages, sizeof(numPages));

        // Initialize the current page position.
        posPag = 0;
    }

    ~FileManager()
    {
        file.close();
    }

    void leerBloque(int numPag, vector<char> &buffer)
    {
        if (numPag >= numTotalPags)
        {
            throw runtime_error("Page number out of bounds");
        }

        file.seekg(numPag * PAGE_SIZE, ios::beg);
        file.read(buffer.data(), PAGE_SIZE);
    }

    int getPosBloque()
    {
        return posPag;
    }

    void leerPrimerBloque(vector<char> &buffer)
    {
        leerBloque(0, buffer);
    }

    void leerUltimoBloque(vector<char> &buffer)
    {
        leerBloque(numTotalPags - 1, buffer);
    }

    void leerAnteriorBloque(vector<char> &buffer)
    {
        if (posPag == 0)
        {
            throw runtime_error("Cannot read previous page");
        }

        posPag--;
        leerBloque(posPag, buffer);
    }

    void leerSiguienteBloque(vector<char> &buffer)
    {
        if (posPag == numTotalPags - 1)
        {
            throw runtime_error("Cannot read next page");
        }

        posPag++;
        leerBloque(posPag, buffer);
    }

    void leerBloqueActual(vector<char> &buffer)
    {
        leerBloque(posPag, buffer);
    }

    void escribirBloque(int numPag, const vector<char> &buffer)
    {
        if (numPag >= numTotalPags)
        {
            throw runtime_error("Page number out of bounds");
        }

        file.seekp(numPag * PAGE_SIZE, ios::beg);
        if (buffer.size() != PAGE_SIZE)
        {
            throw runtime_error("Invalid page size");
        }

        file.write(buffer.data(), PAGE_SIZE);
    }

    void escribirBloqueActual(const vector<char> &buffer)
    {
        escribirBloque(posPag, buffer);
    }

    void agregarBloqueVacio()
    {
        posPag++;
        numTotalPags++;

        vector<char> buffer(PAGE_SIZE, '0');
        escribirBloqueActual(buffer);
    }

private:
    int numTotalPags;
    int posPag;
    fstream file;
};

int main()
{
    FileManager fileManager("file.txt");

    vector<char> buffer(PAGE_SIZE);

    // Read the first page.
    fileManager.leerPrimerBloque(buffer);
    cout << "First page: " << endl;
    cout << buffer.data() << endl;

    // Read the current page.
    fileManager.leerBloqueActual(buffer);

    // Read the next page.
    // fileManager.leerSiguienteBloque(buffer);
    // cout << "Next page: " << endl;
    // cout << buffer.data() << endl;

    // Read the last page.
    // cout << "Last page: " << endl;
    // fileManager.leerUltimoBloque(buffer);
    // cout << buffer.data() << endl;

    // Write a new page.
    vector<char> v(PAGE_SIZE, '7');
    fileManager.escribirBloque(1, v);
    // fileManager.escribirBloqueActual(v);

    // Add a new empty page.
    // fileManager.agregarBloqueVacio();

    return 0;
}
