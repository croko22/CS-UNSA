#include <iostream>
#include <array>
#include <string>
#include <memory>
using namespace std;
constexpr int N = 26;

class DTree
{
public:
    DTree() : root(std::make_unique<Node>('\0')) {}
    void insert(string word, int data = 0)
    {
        auto current = root.get();

        for (auto &c : word)
        {
            const int index = c - 'a';
            auto &child = current->children.at(index);

            if (child == nullptr)
            {
                child = std::make_unique<Node>(c);
            };

            current = child.get();
        }
        current->isWord = true;
        current->data = data;
    }

    int search(string word)
    {
        auto node = getNode(word);
        // TESTING
        // cout << node->data << endl;
        // return node != nullptr && node->isWord;
        if (node != nullptr && node->isWord)
        {
            return node->data;
        }
        else
        {
            return -1;
        }
    }

    bool startsWith(string prefix)
    {
        return getNode(prefix) != nullptr;
    }

private:
    class Node
    {
    public:
        Node(char ch = '\0') : c(ch){};
        const char c = '\0';
        std::array<std::unique_ptr<Node>, N> children;
        // buscar parte de la palabra
        bool isWord = false;
        int data;
    };

    Node *getNode(string word)
    {
        auto current = root.get();
        for (auto &c : word)
        {
            const int index = c - 'a';

            auto child = current->children.at(index).get();
            // if null, return null,
            if (child == nullptr)
                return nullptr;
            // continuar
            current = child;
        }
        return current;
    }
    std::unique_ptr<Node> root;
};

int main()
{
    DTree *dtree = new DTree();
    dtree->insert("uno", 1);
    dtree->insert("union", 2);
    dtree->insert("siete", 3);
    dtree->insert("sietemil", 4);
    cout << endl;
    cout << dtree->search("uuu") << endl;
    cout << dtree->search("uno") << endl;
    cout << dtree->search("sietemil") << endl;
    // cout << dtree->startsWith("gn") << endl;
    return 0;
}