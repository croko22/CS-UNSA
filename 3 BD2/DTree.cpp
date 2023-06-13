#include "DTree.h"
#include "Node.h"
// Función para obtener el nodo correspondiente a una palabra
DTree::Node *DTree::getNode(std::string word)
{
    // Inicia en el nodo raíz
    auto current = root.get();

    // Recorre cada carácter de la palabra
    for (auto &c : word)
    {
        const int index = c - 'a';

        // Obtiene el hijo correspondiente al carácter
        auto child = current->children.at(index).get();

        // Si el hijo no existe, la palabra no está en el arbol digital
        if (child == nullptr)
            return nullptr;

        // Avanza al siguiente nodo
        current = child;
    }

    // Retorna el nodo correspondiente a la palabra
    return current;
}

// Constructor de la clase DTree
DTree::DTree() : root(std::make_unique<Node>('\0')) {}

// Función para insertar una palabra en el arbol digital
void DTree::insert(std::string word, std::variant<int, std::string> data)
{
    // Inicia en el nodo raíz
    auto current = root.get();

    // Recorre cada carácter de la palabra
    for (auto &c : word)
    {
        const int index = c - 'a';
        auto &child = current->children.at(index);

        // Si el hijo no existe, lo crea y lo asigna como hijo actual
        if (child == nullptr)
        {
            child = std::make_unique<Node>(c);
        }

        // Avanza al siguiente nodo
        current = child.get();
    }

    // Marca el último nodo como una palabra completa
    current->isWord = true;

    // Asigna los datos al nodo hoja
    current->hoja = new Hoja(data);
}

// Función para eliminar una palabra del arbol digital
void DTree::deleteWord(std::string word)
{
    // Inicia en el nodo raíz
    Node *current = root.get();

    // Recorre cada carácter de la palabra
    for (auto &c : word)
    {
        const int index = c - 'a';
        auto &child = current->children.at(index);

        // Si el hijo no existe, la palabra no está en el arbol digital
        if (child == nullptr)
            return;

        // Avanza al siguiente nodo
        current = child.get();
    }

    // Marca el último nodo como una palabra incompleta
    current->isWord = false;

    // Elimina los datos del nodo hoja y lo libera de memoria
    delete current->hoja;
    current->hoja = nullptr;

    // Verifica si el nodo tiene hijos
    for (auto &child : current->children)
    {
        // Si encuentra al menos un hijo, no realiza más acciones
        if (child != nullptr)
            return;
    }

    // Elimina los nodos vacíos del arbol digital
    while (current != root.get() && !current->isWord && current->children.empty())
    {
        auto parent = root.get();
        for (auto &c : word)
        {
            const int index = c - 'a';
            auto child = parent->children.at(index).get();

            if (child == current)
            {
                // Elimina el nodo hijo
                delete child;
                break;
            }

            // Avanza al siguiente nodo padre
            parent = child;
        }

        // Actualiza el nodo actual
        current = parent;
    }
}

// Función para buscar una palabra en el arbol digital
std::variant<int, std::string> DTree::search(std::string word)
{
    // Obtiene el nodo correspondiente a la palabra
    auto node = getNode(word);

    // Verifica si el nodo existe y contiene datos
    if (node != nullptr && node->hoja != nullptr && node->isWord)
        return node->hoja->data;
    else
        return -1;
}

// Función para verificar si una palabra comienza con un prefijo en el arbol digital
bool DTree::startsWith(std::string prefix)
{
    // Obtiene el nodo correspondiente al prefijo
    return getNode(prefix) != nullptr;
}
