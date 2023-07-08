#include <iostream>
#include <algorithm>
#include <vector>
#include "GestorAlmacenamiento.h"
#include "BufferPoolManager.h"

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
    buffer_pool_manager.PrintFrames();

    // Delete page 2
    buffer_pool_manager.DeletePage(2);

    // Fetch page 3
    page = buffer_pool_manager.FetchPage(3);
    page->read_page();
    page->write_page();
    buffer_pool_manager.PrintFrames();
    // Flush all pages
    buffer_pool_manager.FlushAllPages();
    return 0;
}
