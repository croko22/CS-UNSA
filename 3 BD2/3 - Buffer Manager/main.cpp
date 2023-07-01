#include <iostream>
#include <vector>
#include "GestorAlmacenamiento.h"

constexpr int PAGE_SIZE = 5;

class Page
{
private:
    int page_id;
    int subprocess_count;
    bool dirty;
    std::vector<char> buffer;
    GestorAlmacenamiento *gestor;

public:
    Page(GestorAlmacenamiento &gestor, int page_id) : page_id(page_id), gestor(&gestor)
    {
        buffer.resize(PAGE_SIZE, 0);
    }
    int get_page_id() { return page_id; }
    void read_page()
    {
        gestor->leerBloque(page_id, buffer);
        std::cout << buffer.data() << std::endl;
    }
    void write_page()
    {
        gestor->escribirBloque(page_id, buffer);
    }
};

int main()
{
    GestorAlmacenamiento gestor("file.txt");
    Page page(gestor, 1);

    page.read_page();
    page.write_page();
    return 0;
}
