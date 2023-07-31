template <typename T>
struct Node
{
    bool is_leaf;
    std::size_t degree; // maximum number of children
    std::size_t size;   // current number of item
    T *item;
    Node<T> **children;
    Node<T> *parent;

public:
    Node(std::size_t _degree)
    { // Constructor
        this->is_leaf = false;
        this->degree = _degree;
        this->size = 0;

        T *_item = new T[degree - 1];
        for (int i = 0; i < degree - 1; i++)
        {
            _item[i] = 0;
        }
        this->item = _item;

        Node<T> **_children = new Node<T> *[degree];
        for (int i = 0; i < degree; i++)
        {
            _children[i] = nullptr;
        }
        this->children = _children;

        this->parent = nullptr;
    }
};