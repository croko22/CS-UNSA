#include <iostream>
#include <variant>
#include "DTree.h"

using namespace std;
int main(int argc, char const *argv[])
{
    DTree *dtree = new DTree();
    //* Inserciones
    dtree->insert("n", 23);
    dtree->insert("ninho", "Juan");
    dtree->insert("ninha", "Maria");
    dtree->insert("ninhez", 4);
    dtree->insert("ninhera", 3);

    dtree->insert("aninhado", 174);
    // //* Busquedas
    std::variant<int, std::string> result;
    result = dtree->search("ninhas");
    cout << "Buscando ninhas, " << std::get<int>(result) << endl;

    result = dtree->search("ninha");
    cout << "Buscando ninha, " << std::get<string>(result) << endl;
    result = dtree->search("ninho");
    cout << "Buscando ninho, " << std::get<string>(result) << endl;
    // //* Eliminaciones
    dtree->deleteWord("ninhera");
    result = dtree->search("ninhera");
    cout << "Buscando ninhera, " << std::get<int>(result) << endl;
    return 0;
}