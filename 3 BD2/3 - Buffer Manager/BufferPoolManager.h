#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "GestorAlmacenamiento.h"
#include "Page.h"

class BufferPoolManager
{
private:
    std::shared_ptr<GestorAlmacenamiento> gestor;
    std::vector<Page> pages;
    std::function<void()> replacement_strategy;
    std::string strategy;

public:
    BufferPoolManager(std::shared_ptr<GestorAlmacenamiento> gestor, std::string strategy);
    Page *FetchPage(int page_id);
    void NewPage(int page_id);
    bool UnpinPage(int page_id, bool is_dirty);
    bool FlushPage(int page_id);
    void FlushAllPages();
    void DeletePage(int page_id);
    void PrintFrames();
};
