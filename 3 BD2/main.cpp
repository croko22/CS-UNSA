#include <iostream>
#include <variant>
#include "DTree.h"

using namespace std;

int main(int argc, char const *argv[])
{
    DTree *dtree = new DTree();
    //* Inserciones
    dtree->insert("ave", 15);
    dtree->insert("o", 45);
    dtree->insert("oso", "25a");
    // //* Busquedas
    std::variant<int, std::string> result;
    result = dtree->search("ave");
    cout << "Buscando ave, " << std::get<int>(result) << endl;
    result = dtree->search("oso");
    cout << "Buscando oso, " << std::get<std::string>(result) << endl;
    result = dtree->search("os");
    cout << "Buscando os, " << std::get<int>(result) << endl;
    // //* Eliminaciones
    dtree->deleteWord("o");
    result = dtree->search("o");
    cout << "Buscando o, " << std::get<int>(result) << endl;
    result = dtree->search("oso");
    cout << "Buscando oso, " << std::get<std::string>(result) << endl;
    return 0;
}