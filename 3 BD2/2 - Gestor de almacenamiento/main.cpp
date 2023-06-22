#include <iostream>
#include <vector>
#include "GestorAlmacenamiento.h"

constexpr int PAGE_SIZE = 4096;

// Función para imprimir el menú de opciones
void printMenu()
{
    std::cout << "------- Menú -------" << std::endl;
    std::cout << "1. Leer primer bloque" << std::endl;
    std::cout << "2. Leer último bloque" << std::endl;
    std::cout << "3. Leer bloque anterior" << std::endl;
    std::cout << "4. Leer bloque siguiente" << std::endl;
    std::cout << "5. Leer bloque actual" << std::endl;
    std::cout << "6. Escribir bloque actual" << std::endl;
    std::cout << "7. Agregar bloque vacío" << std::endl;
    std::cout << "8. Salir" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Ingrese el número de la opción: ";
}

int main()
{
    //? Nombre del archivo
    // std::string filename;
    // std::cout << "Ingrese el nombre del archivo: ";
    // std::cin >> filename;

    // Crear una instancia de GestorAlmacenamiento
    // GestorAlmacenamiento gestor(filename);
    GestorAlmacenamiento gestor("file.txt");

    int option;
    std::vector<char> buffer(PAGE_SIZE, 0);

    do
    {
        printMenu();
        std::cin >> option;

        switch (option)
        {
        case 1:
            gestor.leerPrimerBloque(buffer);
            std::cout << "Contenido del primer bloque: " << std::endl;
            // Imprimir el contenido del bloque
            std::cout << buffer.data() << std::endl;
            std::cout << std::endl;
            break;

        case 2:
            gestor.leerUltimoBloque(buffer);
            std::cout << "Contenido del último bloque: " << std::endl;
            // Imprimir el contenido del bloque
            std::cout << buffer.data() << std::endl;
            std::cout << std::endl;
            break;

        case 3:
            gestor.leerAnteriorBloque(buffer);
            std::cout << "Contenido del bloque anterior: " << std::endl;
            // Imprimir el contenido del bloque
            std::cout << buffer.data() << std::endl;
            std::cout << std::endl;
            break;

        case 4:
            gestor.leerSiguienteBloque(buffer);
            std::cout << "Contenido del bloque siguiente: " << std::endl;
            // Imprimir el contenido del bloque
            std::cout << buffer.data() << std::endl;
            std::cout << std::endl;
            break;

        case 5:
            gestor.leerBloqueActual(buffer);
            std::cout << "Contenido del bloque actual: " << std::endl;
            // Imprimir el contenido del bloque
            std::cout << buffer.data() << std::endl;
            std::cout << std::endl;
            break;

        case 6:
            std::cout << "Ingrese el nuevo contenido del bloque: ";
            std::cin.ignore(); // Ignorar el carácter de nueva línea
            std::cin.getline(buffer.data(), PAGE_SIZE);
            gestor.escribirBloqueActual(buffer);
            std::cout << "Bloque actual actualizado." << std::endl;
            break;

        case 7:
            gestor.agregarBloqueVacio();
            std::cout << "Bloque vacío agregado." << std::endl;
            break;

        case 8:
            std::cout << "Saliendo del programa." << std::endl;
            break;

        default:
            std::cout << "Opción inválida. Intente de nuevo." << std::endl;
            break;
        }
    } while (option != 8);

    return 0;
}
