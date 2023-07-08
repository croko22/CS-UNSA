#include <memory>
#include <vector>

class GestorAlmacenamiento;

class Page
{
private:
    int page_id;
    std::shared_ptr<GestorAlmacenamiento> gestor;

public:
    int subprocess_count;
    bool dirty;
    std::vector<char> buffer;

    Page(std::shared_ptr<GestorAlmacenamiento> gestor, int page_id);
    int get_page_id();
    void read_page();
    void write_page();
    ~Page();
};
