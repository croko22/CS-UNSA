#include <iostream>
#include "DTree.h"

using namespace std;

int main(int argc, char const *argv[])
{
    DTree *dtree = new DTree();
    dtree->insert("uno", 1);
    dtree->insert("union", 2);
    dtree->insert("siete", 3);
    dtree->insert("sietemil", 4);
    cout << "Buscando uuu, " << dtree->search("uuu") << endl;
    cout << "Buscando uno, " << dtree->search("uno") << endl;
    cout << "Buscando sietemil, " << dtree->search("sietemil") << endl;
    cout << "Buscando siette, " << dtree->startsWith("siette") << endl;
    dtree->deleteWord("uno");
    cout << "Buscando uno, " << dtree->search("uno") << endl;
    cout << "Buscando union, " << dtree->search("union") << endl;
    return 0;
}