#include <iostream>
#include <algorithm>
#include <vector>
#include "GestorAlmacenamiento.h"
#include "Page.cpp"
class BufferPoolManager
{
private:
    std::vector<Page> pages;
    std::shared_ptr<GestorAlmacenamiento> gestor;
    void *ReplacementStrategy()
    {
        return nullptr;
    }
    //* LRU/MRU Helper function
    void movePageToFront(int page_id)
    {
        auto it = std::find_if(pages.begin(), pages.end(), [page_id](Page &page)
                               { return page.get_page_id() == page_id; });

        if (it != pages.end())
        {
            std::rotate(pages.begin(), it, it + 1);
        }
    }

public:
    BufferPoolManager(std::shared_ptr<GestorAlmacenamiento> gestor)
        : gestor(gestor) {}
    Page *FetchPage(int page_id)
    {
        // Page *page = nullptr;
        // for (Page &pg : pages)
        // {
        //     if (pg.get_page_id() == page_id)
        //     {
        //         page = &pg;
        //         break;
        //     }
        // }

        // if (page)
        // {
        //     // Page found, move it to the front (MRU)
        //     movePageToFront(page_id);
        //     page->subprocess_count++;
        // }
        // return page;
        for (Page &page : pages)
        {
            if (page.get_page_id() == page_id)
            {
                page.subprocess_count++;
                return &page;
            }
        }
        return nullptr;
    }
    void NewPage(int page_id)
    {
        pages.push_back(Page(gestor, page_id));
    }
    bool UnpinPage(int page_id, bool is_dirty)
    {
        for (Page &page : pages)
        {
            if (page.get_page_id() == page_id)
            {
                page.subprocess_count--;
                //? Si el contador de pines es cero, la funcion agregar ́a el objeto de pagina al rastreador LRUReplacer.
                // if (page.subprocess_count == 0 && page.dirty)
                // {
                //     gestor->escribirBloque(page_id, page.buffer);
                //     std::cout << "Page " << page_id << " was written" << std::endl;
                // }
                return true;
            }
        }
        return false;
    }

    bool FlushPage(int page_id)
    {
        for (Page &page : pages)
        {
            if (page.get_page_id() == page_id)
            {
                if (page.dirty)
                {
                    gestor->escribirBloque(page_id, page.buffer);
                    std::cout << "Page " << page_id << " was written" << std::endl;
                }
                return true;
            }
        }
        return false;
    }
    void FlushAllPages()
    {
        for (Page &page : pages)
        {
            if (page.dirty)
            {
                gestor->escribirBloque(page.get_page_id(), page.buffer);
                std::cout << "Page " << page.get_page_id() << " was written" << std::endl;
            }
        }
    }
    void DeletePage(int page_id)
    {
        for (Page &page : pages)
        {
            if (page.get_page_id() == page_id)
            {
                pages.erase(std::remove_if(pages.begin(), pages.end(), [&](Page &page)
                                           { return page.get_page_id() == page_id; }),
                            pages.end());
                return;
            }
        }
    }
};

int main()
{
    auto gestor = std::make_shared<GestorAlmacenamiento>("file.txt");
    BufferPoolManager buffer_pool_manager(gestor);

    buffer_pool_manager.NewPage(1);
    buffer_pool_manager.NewPage(2);
    buffer_pool_manager.NewPage(3);

    // Fetch page 1
    Page *page = buffer_pool_manager.FetchPage(1);
    page->read_page();

    // Fetch page 2 | read | write buffer
    page = buffer_pool_manager.FetchPage(2);
    page->read_page();
    page->write_page();

    // Unpin page 2
    buffer_pool_manager.UnpinPage(2, true);

    // Flush page 1
    buffer_pool_manager.FlushPage(1);
    buffer_pool_manager.FlushPage(2);

    // Delete page 2
    buffer_pool_manager.DeletePage(2);

    // Fetch page 3
    page = buffer_pool_manager.FetchPage(3);
    page->read_page();
    page->write_page();
    // Flush all pages
    buffer_pool_manager.FlushAllPages();

    return 0;
}
