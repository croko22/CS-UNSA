#include "Page.h"
#include "GestorAlmacenamiento.h"
#include <iostream>
constexpr int PAGE_SIZE = 4096; // Tamaño constante de la página

Page::Page(std::shared_ptr<GestorAlmacenamiento> gestor, int page_id)
    : page_id(page_id), gestor(gestor), subprocess_count(0), dirty(false)
{
    buffer.resize(PAGE_SIZE, 0);         // Se redimensiona el búfer de la página al tamaño especificado
    gestor->leerBloque(page_id, buffer); // Se lee el bloque correspondiente al ID de la página desde el gestor de almacenamiento
}

int Page::get_page_id()
{
    return page_id; // Retorna el ID de la página
}

void Page::read_page()
{
    std::cout << buffer.data() << std::endl; // Imprime el contenido del búfer de la página en la salida estándar
    subprocess_count++;                      // Incrementa el contador de subprocesos
}

void Page::write_page()
{
    std::cout << "Page " << page_id << " data: ";
    std::cin >> buffer.data();   // Lee la entrada del usuario y almacena los datos en el búfer de la página
    buffer.resize(PAGE_SIZE, 0); // Se redimensiona el búfer de la página al tamaño especificado
    subprocess_count++;          // Incrementa el contador de subprocesos
    dirty = true;                // Marca la página como modificada (dirty)
}

Page::~Page()
{
    // Si la página está marcada como modificada, se escribe el bloque correspondiente al gestor de almacenamiento
    if (dirty)
    {
        gestor->escribirBloque(page_id, buffer);
        // Imprime un mensaje indicando que la página se ha escrito
        std::cout << "Page " << page_id << " was written" << std::endl;
    }
}
