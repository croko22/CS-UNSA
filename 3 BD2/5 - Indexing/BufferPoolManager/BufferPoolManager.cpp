#include "BufferPoolManager.h"

BufferPoolManager::BufferPoolManager(std::shared_ptr<GestorAlmacenamiento> gestor, std::string strategy = "LRU")
    : gestor(gestor), strategy(strategy) {}

Page *BufferPoolManager::FetchPage(int page_id)
{
    for (auto it = pages.begin(); it != pages.end(); ++it)
    {
        if (it->get_page_id() == page_id)
        {
            active_page_index = it - pages.begin(); // Posición de la página activa (usada en la estrategia CLOCK)
            if (strategy == "CLOCK")
                return &pages[active_page_index]; // Si la estrategia es CLOCK, se retorna la página activa
            else
            {
                std::rotate(pages.begin(), it, it + 1); // Rotar las páginas para mantener la más reciente al principio
                return &pages.front();                  // Se retorna la página más reciente (MRU)
            }
        }
    }

    // Si no se encontró la página en el buffer, se necesita cargarla
    // Se aplica la estrategia de reemplazo correspondiente
    Page new_page(gestor, page_id);
    new_page.subprocess_count++;

    if (strategy == "LRU")
    {
        pages.back() = new_page;
        return &pages.back(); // Se reemplaza la última página del buffer (LRU)
    }
    if (strategy == "MRU")
    {
        pages.front() = new_page;
        return &pages.front(); // Se reemplaza la primera página del buffer (MRU)
    }
    if (strategy == "CLOCK")
    {
        active_page_index = (active_page_index + 1) % pages.size();
        pages[active_page_index] = new_page; // Se reemplaza la página siguiente en la estrategia CLOCK
    }

    return &pages.front(); // Por defecto, se retorna la primera página del buffer
}

void BufferPoolManager::NewPage(int page_id)
{
    pages.push_back(Page(gestor, page_id)); // Se agrega una nueva página al buffer
}

bool BufferPoolManager::UnpinPage(int page_id, bool is_dirty)
{
    for (Page &page : pages)
    {
        if (page.get_page_id() == page_id)
        {
            page.subprocess_count--; // Se disminuye el contador de subprocesos de la página
            if (page.subprocess_count == 0)
            {
                if (strategy == "LRU")
                    std::rotate(pages.begin(), pages.begin() + 1, pages.end()); // Rotar las páginas para mantener la más reciente al principio (LRU)
                if (strategy == "MRU")
                    std::rotate(pages.begin(), pages.end() - 1, pages.end()); // Rotar las páginas para mantener la más reciente al principio (MRU)
                if (strategy == "CLOCK")
                    active_page_index = (active_page_index + 1) % pages.size(); // Actualizar la posición de la página activa (CLOCK)
            }
            return true;
        }
    }
    return false;
}

bool BufferPoolManager::FlushPage(int page_id)
{
    Page *page = FetchPage(page_id); // Se obtiene la página desde el buffer
    if (page->dirty)
    {
        gestor->escribirBloque(page_id, page->buffer); // Se escribe el bloque en el almacenamiento
        std::cout << "Page " << page_id << " was written" << std::endl;
        page->dirty = false; // Se marca la página como no modificada
        return true;
    }
    return false;
}

void BufferPoolManager::FlushAllPages()
{
    for (Page &page : pages)
    {
        if (page.dirty)
        {
            gestor->escribirBloque(page.get_page_id(), page.buffer); // Se escribe el bloque en el almacenamiento
            std::cout << "Page " << page.get_page_id() << " was written" << std::endl;
            page.dirty = false; // Se marca la página como no modificada
        }
    }
}

void BufferPoolManager::DeletePage(int page_id)
{
    pages.erase(std::remove_if(pages.begin(), pages.end(), [&](Page &page)
                               { return page.get_page_id() == page_id; }),
                pages.end()); // Se elimina la página del buffer
}

void BufferPoolManager::PrintFrames()
{
    int count{0};
    std::cout << "Frames\tPageID\tCount\tDirty" << std::endl;
    for (Page &page : pages)
    {
        count++;
        std::cout << count << "\t" << page.get_page_id() << "\t" << page.subprocess_count << "\t" << page.dirty << std::endl; // Se imprime información de cada página en el buffer
    }
}

int BufferPoolManager::GetPageCount()
{
    return pages.size(); // Se retorna la cantidad de páginas en el buffer
}