// GestorAlmacenamiento.cpp

#include "GestorAlmacenamiento.h"

// Define la constante del tamaño de página
constexpr int PAGE_SIZE = 4096;

GestorAlmacenamiento::GestorAlmacenamiento(const std::string &filename)
{
    // Abre el archivo en modo lectura/escritura/binario
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    // Lee el número de páginas desde el principio del archivo
    file.seekg(0, std::ios::beg);
    int numPages;
    file.read((char *)&numPages, sizeof(numPages));

    // Inicializa la posición actual de la página
    posPag = 0;
}

GestorAlmacenamiento::~GestorAlmacenamiento()
{
    // Cierra el archivo
    file.close();
}

void GestorAlmacenamiento::leerBloque(int numPag, std::vector<char> &buffer)
{
    if (numPag >= numTotalPags)
    {
        throw std::runtime_error("Número de página fuera de límites");
    }

    // Mueve el puntero del archivo a la página especificada
    file.seekg(numPag * PAGE_SIZE, std::ios::beg);

    // Lee el contenido de la página en el búfer
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
        throw std::runtime_error("No se puede leer la página anterior");
    }

    // Mueve a la página anterior y lee su contenido
    posPag--;
    leerBloque(posPag, buffer);
}

void GestorAlmacenamiento::leerSiguienteBloque(std::vector<char> &buffer)
{
    if (posPag == numTotalPags - 1)
    {
        throw std::runtime_error("No se puede leer la siguiente página");
    }

    // Mueve a la siguiente página y lee su contenido
    posPag++;
    leerBloque(posPag, buffer);
}

void GestorAlmacenamiento::leerBloqueActual(std::vector<char> &buffer)
{
    // Lee el contenido de la página actual en el búfer
    leerBloque(posPag, buffer);
}

void GestorAlmacenamiento::escribirBloque(int numPag, const std::vector<char> &buffer)
{
    if (numPag >= numTotalPags)
    {
        throw std::runtime_error("Número de página fuera de límites");
    }

    // Mueve el puntero del archivo a la página especificada
    file.seekp(numPag * PAGE_SIZE, std::ios::beg);

    // Maneja los casos donde el tamaño del búfer es menor o mayor que PAGE_SIZE
    if (buffer.size() != PAGE_SIZE)
    {
        std::vector<char> temp(PAGE_SIZE - buffer.size(), '0');
        temp.insert(temp.begin(), buffer.begin(), buffer.end());
        file.write(temp.data(), PAGE_SIZE);
        return;
    }

    // Escribe el contenido del búfer en el archivo
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
