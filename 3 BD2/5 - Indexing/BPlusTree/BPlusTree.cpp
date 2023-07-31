#include <iostream>

using namespace std;

class BPlusTree
{
private:
    struct Node
    {
        int keys[100];
        Node *children[101];
        int n;
    };

    Node *root;
    int order;

public:
    BPlusTree(int order)
    {
        this->order = order;
        root = nullptr;
    }

    void insert(int key)
    {
        Node *node = root;
        while (node != nullptr)
        {
            int i = 0;
            while (i < node->n && node->keys[i] < key)
            {
                i++;
            }
            if (i < node->n)
            {
                // The key already exists in the tree.
                return;
            }
            else
            {
                // The key is not in the tree, so we need to insert it.
                if (node->n == order - 1)
                {
                    // The node is full, so we need to split it.
                    Node *newNode = new Node();
                    newNode->n = 0;
                    for (int j = order - 1; j >= 1; j--)
                    {
                        newNode->keys[j - 1] = node->keys[j];
                    }
                    newNode->children[0] = node->children[order];
                    node->children[order] = newNode;
                    node->n = 1;
                    node->keys[0] = key;
                }
                else
                {
                    // The node is not full, so we can just insert the key.
                    node->keys[node->n++] = key;
                    node->children[node->n] = nullptr;
                }
                return;
            }
        }

        // The root is empty, so we need to create a new node and make it the root.
        root = new Node();
        root->keys[0] = key;
        root->n = 1;
        root->children[0] = nullptr;
    }

    void search(int key)
    {
        Node *node = root;
        while (node != nullptr)
        {
            int i = 0;
            while (i < node->n && node->keys[i] < key)
            {
                i++;
            }
            if (i < node->n && node->keys[i] == key)
            {
                // The key was found.
                cout << "The key " << key << " was found." << endl;
                return;
            }
            else
            {
                node = node->children[i];
            }
        }

        // The key was not found.
        cout << "The key " << key << " was not found." << endl;
    }
};

// int main()
// {
//     BPlusTree tree(3);
//     tree.insert(1);
//     tree.insert(2);
//     tree.insert(3);
//     tree.insert(4);
//     tree.insert(5);
//     tree.insert(6);
//     tree.insert(7);
//     tree.insert(8);
//     tree.insert(9);
//     tree.insert(10);

//     tree.search(1);
//     tree.search(5);
//     tree.search(10);

//     return 0;
// }
