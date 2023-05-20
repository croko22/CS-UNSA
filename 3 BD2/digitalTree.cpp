#include <iostream>
#include <string>
using namespace std;
#define N 128

typedef struct DTree DTree;
// TODO: Terminar el arbol como en la imagen que siga la key y eso es algo como un trie
// https://gist.github.com/datlife/70607579c7377ad9f4e86bc5d1d81f41
// https://leetcode.com/problems/implement-trie-prefix-tree/solutions/1626293/modern-c-trie-solution/
struct DTree
{
    char data;
    DTree *children[N];
    int is_leaf;
};

DTree *make_DTree(char data)
{
    DTree *node = (DTree *)calloc(1, sizeof(DTree));
    for (int i = 0; i < N; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    node->data = data;
    return node;
}

void free_DTree(DTree *node)
{
    for (int i = 0; i < N; i++)
    {
        if (node->children[i] != NULL)
        {
            free_DTree(node->children[i]);
        }
        else
            continue;
    }
    delete node;
}

DTree *insert(DTree *root, char *word)
{
    DTree *temp = root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int idx = (int)word[i] - 'a';
        if (temp->children[idx] == NULL)
        {
            temp->children[idx] = make_DTree(word[i]);
        }
        else
        {
        }
        temp = temp->children[idx];
    }
    temp->is_leaf = 1;
    return root;
}

int search_trie(DTree *root, char *word)
{
    DTree *temp = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int position = word[i] - 'a';
        if (temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
    if (temp != NULL && temp->is_leaf == 1)
    {
        cout << temp->data << endl;
        return 1;
    }
    return 0;
}

void print_search(DTree *root, char *word)
{
    printf("Searching for %s: ", word);
    if (search_trie(root, word) == 0)
        printf("Not Found\n");
    else
        printf("Found!\n");
}

void print_trie(DTree *root)
{
    if (!root)
        return;
    DTree *temp = root;
    cout << temp->data << " -> ";
    for (int i = 0; i < N; i++)
    {
        print_trie(temp->children[i]);
    }
}

int main()
{
    // Raiz del arbol
    DTree *root = make_DTree('\0');
    // Inserciones
    root = insert(root, "uno");
    root = insert(root, "union");
    root = insert(root, "siete");
    root = insert(root, "sietemil");
    cout << endl;
    print_trie(root);
    cout << endl;
    print_search(root, "uuu");
    print_search(root, "uno");
    print_search(root, "sietemil");
    free_DTree(root);
    return 0;
}