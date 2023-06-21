// GestorAlmacenamiento.cpp

#include "GestorAlmacenamiento.h"
constexpr int PAGE_SIZE = 4096;

GestorAlmacenamiento::GestorAlmacenamiento(const std::string &filename)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    // Read the number of pages from the beginning of the file.
    file.seekg(0, std::ios::beg);
    int numPages;
    file.read((char *)&numPages, sizeof(numPages));

    // Initialize the current page position.
    posPag = 0;
}

GestorAlmacenamiento::~GestorAlmacenamiento()
{
    file.close();
}

void GestorAlmacenamiento::leerBloque(int numPag, std::vector<char> &buffer)
{
    if (numPag >= numTotalPags)
    {
        throw std::runtime_error("Page number out of bounds");
    }

    file.seekg(numPag * PAGE_SIZE, std::ios::beg);
    file.read(buffer.data(), PAGE_SIZE);
}

int GestorAlmacenamiento::getPosBloque()
{
    return posPag;
}

void GestorAlmacenamiento::leerPrimerBloque(std::vector<char> &buffer)
{
    leerBloque(0, buffer);
}

void GestorAlmacenamiento::leerUltimoBloque(std::vector<char> &buffer)
{
    leerBloque(numTotalPags - 1, buffer);
}

void GestorAlmacenamiento::leerAnteriorBloque(std::vector<char> &buffer)
{
    if (posPag == 0)
    {
        throw std::runtime_error("Cannot read previous page");
    }

    posPag--;
    leerBloque(posPag, buffer);
}

void GestorAlmacenamiento::leerSiguienteBloque(std::vector<char> &buffer)
{
    if (posPag == numTotalPags - 1)
    {
        throw std::runtime_error("Cannot read next page");
    }

    posPag++;
    leerBloque(posPag, buffer);
}

void GestorAlmacenamiento::leerBloqueActual(std::vector<char> &buffer)
{
    leerBloque(posPag, buffer);
}

void GestorAlmacenamiento::escribirBloque(int numPag, const std::vector<char> &buffer)
{
    if (numPag >= numTotalPags)
    {
        throw std::runtime_error("Page number out of bounds");
    }

    file.seekp(numPag * PAGE_SIZE, std::ios::beg);
    if (buffer.size() != PAGE_SIZE)
    {
        // throw std::runtime_error("Invalid page size");
        // Handle less than PAGE_SIZE
        std::vector<char> temp(PAGE_SIZE - buffer.size(), '0');
        temp.insert(temp.begin(), buffer.begin(), buffer.end());
        file.write(temp.data(), PAGE_SIZE);
        return;
    }

    file.write(buffer.data(), PAGE_SIZE);
}

void GestorAlmacenamiento::escribirBloqueActual(const std::vector<char> &buffer)
{
    escribirBloque(posPag, buffer);
}

void GestorAlmacenamiento::agregarBloqueVacio()
{
    posPag++;
    numTotalPags++;

    std::vector<char> buffer(PAGE_SIZE, '0');
    escribirBloqueActual(buffer);
}