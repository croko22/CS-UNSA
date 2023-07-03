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
        subprocess_count = 0;
        dirty = false;
    }
    int get_page_id() { return page_id; }
    void read_page()
    {
        gestor->leerBloque(page_id, buffer);
        std::cout << buffer.data() << std::endl;
        subprocess_count++;
    }
    void write_page()
    {
        gestor->escribirBloque(page_id, buffer);
        subprocess_count++;
        dirty = true;
    }
    ~Page()
    {
        if (dirty)
        {
            // write_page();
            std::cout << "Page " << page_id << " was written" << std::endl;
        }
    }
};

class BufferPoolManager
{
private:
    std::vector<Page> pages;
    GestorAlmacenamiento *gestor;
    void *ReplacementStrategy()
    {
        return nullptr;
    }

public:
    BufferPoolManager(GestorAlmacenamiento &gestor) : gestor(&gestor)
    {
    }
    Page *FetchPage(int page_id);
    void NewPage(int page_id);
    bool UnpinPage(int page_id, bool is_dirty);
    bool FlushPage(int page_id);
    void FlushAllPages();
    void DeletePage(int page_id);
};

int main()
{
    GestorAlmacenamiento gestor("file.txt");
    Page page(gestor, 1);

    page.read_page();
    page.write_page();
    return 0;
}
