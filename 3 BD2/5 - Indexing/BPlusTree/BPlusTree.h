template <typename T>
class BPlusTree
{
    Node<T> *root;
    std::size_t degree;

public:
    BPlusTree(std::size_t _degree)
    { // Constructor
        this->root = nullptr;
        this->degree = _degree;
    }
    ~BPlusTree()
    { // Destructor
        clear(this->root);
    }

    Node<T> *getroot()
    {
        return this->root;
    }

    Node<T> *BPlusTreeSearch(Node<T> *node, T key)
    {
        if (node == nullptr)
        { // if root is null, return nullptr
            return nullptr;
        }
        else
        {
            Node<T> *cursor = node; // cursor finding key

            while (!cursor->is_leaf)
            { // until cusor pointer arrive leaf
                for (int i = 0; i < cursor->size; i++)
                { // in this index node, find what we want key
                    if (key < cursor->item[i])
                    { // find some range, and let find their child also.
                        cursor = cursor->children[i];
                        break;
                    }
                    if (i == (cursor->size) - 1)
                    {
                        cursor = cursor->children[i + 1];
                        break;
                    }
                }
            }

            // search for the key if it exists in leaf node.
            for (int i = 0; i < cursor->size; i++)
            {
                if (cursor->item[i] == key)
                {
                    return cursor;
                }
            }

            return nullptr;
        }
    }
    Node<T> *BPlusTreeRangeSearch(Node<T> *node, T key)
    {
        if (node == nullptr)
        { // if root is null, return nullptr
            return nullptr;
        }
        else
        {
            Node<T> *cursor = node; // cursor finding key

            while (!cursor->is_leaf)
            { // until cusor pointer arrive leaf
                for (int i = 0; i < cursor->size; i++)
                { // in this index node, find what we want key
                    if (key < cursor->item[i])
                    { // find some range, and let find their child also.
                        cursor = cursor->children[i];
                        break;
                    }
                    if (i == (cursor->size) - 1)
                    {
                        cursor = cursor->children[i + 1];
                        break;
                    }
                }
            }
            return cursor;
        }
    }
    int range_search(T start, T end, T *result_data, int arr_length)
    {
        int index = 0;

        Node<T> *start_node = BPlusTreeRangeSearch(this->root, start);
        Node<T> *cursor = start_node;
        T temp = cursor->item[0];

        while (temp <= end)
        {
            if (cursor == nullptr)
            {
                break;
            }
            for (int i = 0; i < cursor->size; i++)
            {
                temp = cursor->item[i];
                if ((temp >= start) && (temp <= end))
                {
                    result_data[index] = temp;
                    index++;
                }
            }
            cursor = cursor->children[cursor->size];
        }
        return index;
    }
    bool search(T data)
    { // Return true if the item exists. Return false if it does not.
        return BPlusTreeSearch(this->root, data) != nullptr;
    }

    int find_index(T *arr, T data, int len)
    {
        int index = 0;
        for (int i = 0; i < len; i++)
        {
            if (data < arr[i])
            {
                index = i;
                break;
            }
            if (i == len - 1)
            {
                index = len;
                break;
            }
        }
        return index;
    }
    T *item_insert(T *arr, T data, int len)
    {
        int index = 0;
        for (int i = 0; i < len; i++)
        {
            if (data < arr[i])
            {
                index = i;
                break;
            }
            if (i == len - 1)
            {
                index = len;
                break;
            }
        }

        for (int i = len; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }

        arr[index] = data;

        return arr;
    }
    Node<T> **child_insert(Node<T> **child_arr, Node<T> *child, int len, int index)
    {
        for (int i = len; i > index; i--)
        {
            child_arr[i] = child_arr[i - 1];
        }
        child_arr[index] = child;
        return child_arr;
    }
    Node<T> *child_item_insert(Node<T> *node, T data, Node<T> *child)
    {
        int item_index = 0;
        int child_index = 0;
        for (int i = 0; i < node->size; i++)
        {
            if (data < node->item[i])
            {
                item_index = i;
                child_index = i + 1;
                break;
            }
            if (i == node->size - 1)
            {
                item_index = node->size;
                child_index = node->size + 1;
                break;
            }
        }
        for (int i = node->size; i > item_index; i--)
        {
            node->item[i] = node->item[i - 1];
        }
        for (int i = node->size + 1; i > child_index; i--)
        {
            node->children[i] = node->children[i - 1];
        }

        node->item[item_index] = data;
        node->children[child_index] = child;

        return node;
    }
    void insert(T data)
    {
        if (this->root == nullptr)
        { // if the tree is empty
            this->root = new Node<T>(this->degree);
            this->root->is_leaf = true;
            this->root->item[0] = data;
            this->root->size = 1; //
        }
        else
        { // if the tree has at least one node
            Node<T> *cursor = this->root;

            // move to leaf node
            cursor = BPlusTreeRangeSearch(cursor, data);

            // overflow check
            if (cursor->size < (this->degree - 1))
            { // not overflow, just insert in the correct position
                // item insert and rearrange
                cursor->item = item_insert(cursor->item, data, cursor->size);
                cursor->size++;
                // edit pointer(next node)
                cursor->children[cursor->size] = cursor->children[cursor->size - 1];
                cursor->children[cursor->size - 1] = nullptr;
            }
            else
            { // overflow case
                // make new node
                auto *Newnode = new Node<T>(this->degree);
                Newnode->is_leaf = true;
                Newnode->parent = cursor->parent;

                // copy item
                T *item_copy = new T[cursor->size + 1];
                for (int i = 0; i < cursor->size; i++)
                {
                    item_copy[i] = cursor->item[i];
                }

                // insert and rearrange
                item_copy = item_insert(item_copy, data, cursor->size);

                // split nodes
                cursor->size = (this->degree) / 2;
                if ((this->degree) % 2 == 0)
                {
                    Newnode->size = (this->degree) / 2;
                }
                else
                {
                    Newnode->size = (this->degree) / 2 + 1;
                }

                for (int i = 0; i < cursor->size; i++)
                {
                    cursor->item[i] = item_copy[i];
                }
                for (int i = 0; i < Newnode->size; i++)
                {
                    Newnode->item[i] = item_copy[cursor->size + i];
                }

                cursor->children[cursor->size] = Newnode;
                Newnode->children[Newnode->size] = cursor->children[this->degree - 1];
                cursor->children[this->degree - 1] = nullptr;

                delete[] item_copy;

                // parent check
                T paritem = Newnode->item[0];

                if (cursor->parent == nullptr)
                { // if there are no parent node(root case)
                    auto *Newparent = new Node<T>(this->degree);
                    cursor->parent = Newparent;
                    Newnode->parent = Newparent;

                    Newparent->item[0] = paritem;
                    Newparent->size++;

                    Newparent->children[0] = cursor;
                    Newparent->children[1] = Newnode;

                    this->root = Newparent;
                }
                else
                { // if there already have parent node
                    InsertPar(cursor->parent, Newnode, paritem);
                }
            }
        }
    }

    void clear(Node<T> *cursor)
    {
        if (cursor != nullptr)
        {
            if (!cursor->is_leaf)
            {
                for (int i = 0; i <= cursor->size; i++)
                {
                    clear(cursor->children[i]);
                }
            }
            delete[] cursor->item;
            delete[] cursor->children;
            delete cursor;
        }
    }
};