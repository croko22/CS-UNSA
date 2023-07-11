#include <iostream>
#include "GestorAlmacenamiento.h"
#include "Page.h"
constexpr int PAGE_SIZE = 5;

Page::Page(std::shared_ptr<GestorAlmacenamiento> gestor, int page_id)
    : page_id(page_id), gestor(gestor), subprocess_count(0), dirty(false)
{
    buffer.resize(PAGE_SIZE, 0);
    gestor->leerBloque(page_id, buffer);
}

int Page::get_page_id()
{
    return page_id;
}

void Page::read_page()
{
    // gestor->leerBloque(page_id, buffer);
    std::cout << buffer.data() << std::endl;
    subprocess_count++;
}

void Page::write_page()
{
    std::cout << "Page " << page_id << " data: ";
    std::cin >> buffer.data();
    buffer.resize(PAGE_SIZE, 0);
    subprocess_count++;
    dirty = true;
}

Page::~Page()
{
    if (dirty)
    {
        gestor->escribirBloque(page_id, buffer);
        std::cout << "Page " << page_id << " was written" << std::endl;
    }
}