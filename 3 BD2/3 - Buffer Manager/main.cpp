#include <iostream>
#include <algorithm>
#include "GestorAlmacenamiento.h"
#include "BufferPoolManager.h"

void displayMenu()
{
    std::cout << "===== Menú del Buffer Pool Manager =====" << std::endl;
    std::cout << "1. Recuperar una página" << std::endl;
    std::cout << "2. Crear una nueva página" << std::endl;
    std::cout << "3. Desanclar una página" << std::endl;
    std::cout << "4. Escribir página en disco" << std::endl;
    std::cout << "5. Escribir todas las páginas en disco" << std::endl;
    std::cout << "6. Eliminar una página" << std::endl;
    std::cout << "7. Mostrar marcos del buffer" << std::endl;
    std::cout << "8. Salir del programa" << std::endl;
    std::cout << "Ingrese el número de opción: ";
}

void displaySubMenu()
{
    std::cout << "===== Submenú de manipulación de página =====" << std::endl;
    std::cout << "1. Leer la página" << std::endl;
    std::cout << "2. Escribir en la página" << std::endl;
    std::cout << "3. Volver al menú principal" << std::endl;
    std::cout << "Ingrese el número de opción: ";
}

int main()
{
    auto gestor = std::make_shared<GestorAlmacenamiento>("file.txt");
    BufferPoolManager bufferPoolManager(gestor);
    bufferPoolManager.NewPage(1);

    int option;
    int page_id;

    while (true)
    {
        displayMenu();
        std::cin >> option;

        if (option == 1)
        {
            std::cout << "Ingrese el ID de la página: ";
            std::cin >> page_id;

            Page *page = bufferPoolManager.FetchPage(page_id);
            if (page)
            {
                std::cout << "Página recuperada con éxito." << std::endl;

                bool subMenuActive = true;
                int subOption;
                while (subMenuActive)
                {
                    displaySubMenu();
                    std::cin >> subOption;

                    switch (subOption)
                    {
                    case 1:
                        page->read_page();
                        break;
                    case 2:
                        page->write_page();
                        break;
                    case 3:
                        subMenuActive = false;
                        break;
                    default:
                        std::cout << "Opción inválida. Intente de nuevo." << std::endl;
                        break;
                    }
                }
            }
            else
            {
                std::cout << "Error: No se pudo recuperar la página." << std::endl;
            }
        }
        else if (option == 2)
        {
            int page_id;
            std::cout << "Id de la nueva página: ";
            std::cin >> page_id;
            bufferPoolManager.NewPage(page_id);
        }
        else if (option == 3)
        {
            std::cout << "Ingrese el ID de la página a desanclar: ";
            std::cin >> page_id;
            bool is_dirty;
            std::cout << "¿La página está sucia? (1: Sí, 0: No): ";
            std::cin >> is_dirty;

            bool result = bufferPoolManager.UnpinPage(page_id, is_dirty);
            if (result)
            {
                std::cout << "Página desanclada con éxito." << std::endl;
            }
            else
            {
                std::cout << "Error: No se pudo desanclar la página." << std::endl;
            }
        }
        else if (option == 4)
        {
            std::cout << "Ingrese el ID de la página a escribir en disco: ";
            std::cin >> page_id;

            bool result = bufferPoolManager.FlushPage(page_id);
            if (result)
            {
                std::cout << "Página escrita en disco con éxito." << std::endl;
            }
            else
            {
                std::cout << "Error: No se pudo escribir la página en disco." << std::endl;
            }
        }
        else if (option == 5)
        {
            std::cout << "Escribiendo todas las páginas en disco..." << std::endl;
            bufferPoolManager.FlushAllPages();
        }
        else if (option == 6)
        {
            std::cout << "Ingrese el ID de la página a eliminar: ";
            std::cin >> page_id;

            bufferPoolManager.DeletePage(page_id);
            std::cout << "Página eliminada con éxito." << std::endl;
        }
        else if (option == 7)
        {
            std::cout << "Mostrando marcos del buffer..." << std::endl;
            bufferPoolManager.PrintFrames();
        }
        else if (option == 8)
        {
            std::cout << "Saliendo del programa..." << std::endl;
            break;
        }
        else
        {
            std::cout << "Opción inválida. Intente de nuevo." << std::endl;
        }
    }

    return 0;
}