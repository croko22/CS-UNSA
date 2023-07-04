#include <memory>

constexpr int PAGE_SIZE = 5;

class Page
{
private:
    int page_id;
    std::shared_ptr<GestorAlmacenamiento> gestor;

public:
    int subprocess_count;
    bool dirty;
    std::vector<char> buffer;
    Page(std::shared_ptr<GestorAlmacenamiento> gestor, int page_id)
        : page_id(page_id), gestor(gestor), subprocess_count(0), dirty(false)
    {
        buffer.resize(PAGE_SIZE, 0);
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
        std::cout << "Page " << page_id << " data: ";
        std::cin >> buffer.data();
        subprocess_count++;
        dirty = true;
    }
    ~Page()
    {
        if (dirty)
        {
            gestor->escribirBloque(page_id, buffer);
            std::cout << "Page " << page_id << " was written" << std::endl;
        }
    }
};
