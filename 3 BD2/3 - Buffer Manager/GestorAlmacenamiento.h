// GestorAlmacenamiento.h

#ifndef GESTORALMACENAMIENTO_H
#define GESTORALMACENAMIENTO_H

#include <fstream>
#include <vector>
#include <iostream>

class GestorAlmacenamiento
{
public:
    GestorAlmacenamiento(const std::string &filename);
    ~GestorAlmacenamiento();

    void leerBloque(int numPag, std::vector<char> &buffer);
    int getPosBloque();
    void leerPrimerBloque(std::vector<char> &buffer);
    void leerUltimoBloque(std::vector<char> &buffer);
    void leerAnteriorBloque(std::vector<char> &buffer);
    void leerSiguienteBloque(std::vector<char> &buffer);
    void leerBloqueActual(std::vector<char> &buffer);
    void escribirBloque(int numPag, const std::vector<char> &buffer);
    void escribirBloqueActual(const std::vector<char> &buffer);
    void agregarBloqueVacio();

private:
    int numTotalPags;
    int posPag;
    std::fstream file;
};

#endif // GESTORALMACENAMIENTO_H
