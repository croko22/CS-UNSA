#include "BufferPoolManager.h"

BufferPoolManager::BufferPoolManager(std::shared_ptr<GestorAlmacenamiento> gestor, std::string strategy)
    : gestor(gestor), strategy(strategy)
{
}

Page *BufferPoolManager::FetchPage(int page_id)
{
    for (auto it = pages.begin(); it != pages.end(); ++it)
    {
        if (it->get_page_id() == page_id)
        {
            std::rotate(pages.begin(), it, it + 1);
            return &pages.front();
        }
    }

    Page new_page(gestor, page_id);
    new_page.subprocess_count++;

    if (strategy == "LRU")
    {
        pages.back() = new_page;
        return &pages.back();
    }
    if (strategy == "MRU")
    {
        pages.front() = new_page;
        return &pages.front();
    }

    return &pages.front();
}

void BufferPoolManager::NewPage(int page_id)
{
    pages.push_back(Page(gestor, page_id));
}

bool BufferPoolManager::UnpinPage(int page_id, bool is_dirty)
{
    for (Page &page : pages)
    {
        if (page.get_page_id() == page_id)
        {
            page.subprocess_count--;

            return true;
        }
    }
    return false;
}

bool BufferPoolManager::FlushPage(int page_id)
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

void BufferPoolManager::FlushAllPages()
{
    for (Page &page : pages)
    {
        if (page.dirty)
        {
            // TODO: Arreglar esto, no funciona bien
            gestor->escribirBloque(page.get_page_id(), page.buffer);
            std::cout << "Page " << page.get_page_id() << " was written" << std::endl;
        }
    }
}

void BufferPoolManager::DeletePage(int page_id)
{
    pages.erase(std::remove_if(pages.begin(), pages.end(), [&](Page &page)
                               { return page.get_page_id() == page_id; }),
                pages.end());
}

void BufferPoolManager::PrintFrames()
{
    int count{0};
    std::cout << "Frames\tPageID\tCount\tDirty" << std::endl;
    for (Page &page : pages)
    {
        count++;
        std::cout << count << "\t" << page.get_page_id() << "\t" << page.subprocess_count << "\t" << page.dirty << std::endl;
    }
}
