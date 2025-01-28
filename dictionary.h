#include <iostream>
#include <sstream>
#include <string>

template<typename Key, typename Info>
class Dictionary
{
    struct Node
    {
        Key key;
        Info info;
        Node* left;
        Node* right;
    };

    private:
        Node* parent;
        unsigned int elements;

    // AVL methods
    void leftRotation(Node*&);
    void rightRotation(Node*&);
    void balanceNode(Node *&);
    int getBalance(Node*) const;

    // Basic BST methods
    int getHeight(Node*) const;
    bool findKey(const Key&) const;

    // Insertion/Deletion
    void insertNode(Node*&, Node*);
    void removeNode(Node*&, const Key&);
    void clearTree(Node*&);
    void copyTree(Node*&, const Node*);
    void addTree(Node*);

    // Misc
    Node* getNode(const Key&) const;
    Node* findSuccessor(Node*) const;
    std::string buildTree(Node *node, bool isLeft, const std::string prefix) const;
    void printNodes(Node*, bool) const;
    
    public:
        // Basic class parts
        Dictionary();
        Dictionary(const Dictionary&);
        ~Dictionary();

        // Public Inserion/Deletion
        bool insert(const Key&, const Info&);
        bool remove(const Key&);
        bool clear();
        bool copy(const Dictionary&);
        void add(const Dictionary&);

        // modify the existin key's info
        bool update(const Key&, const Info&);

        // Misc 
        void drawDictionary() const;
        void printElements(bool asc = 1) const;
        void printKey(const Key&) const;
        const int getSize() const;

        // operators
        const Dictionary<Key, Info>& operator=(const Dictionary&);
        const Dictionary<Key, Info> operator+(const Dictionary&);
};

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::leftRotation(Node *&node)
{
    Node *right = node->right;
    Node *rightleft = right->left;

    right->left = node;
    node->right = rightleft;

    node = right;   
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::rightRotation(Node *&node)
{
    Node *left = node->left;
    Node *leftright = left->right;

    left->right = node;
    node->left = leftright;

    node = left;
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::balanceNode(Node *&node)
{
    int balance = getBalance(node);

    if(balance > 1)
    {
        if(getBalance(node->left) >= 1)
        {
            std::cerr << "before Right\n";
            drawDictionary();
            rightRotation(node);
            std::cerr << "after Right rotation\n";
            drawDictionary();
            return;
        }
        else
        {
            std::cerr << "before Left and Right\n";
            drawDictionary();
            leftRotation(node->left);
            std::cerr << "after Left, before Right\n";
            drawDictionary();
            rightRotation(node);
            std::cerr << "after Left Right rotation\n";
            drawDictionary();
            return; 
        }
    }

    if(balance < -1)
    {
        if(getBalance(node->right) <= 0)
        {
            std::cerr << "before Left\n";
            drawDictionary();
            leftRotation(node);
            std::cerr << "after Left rotation\n";
            drawDictionary();
            return;
        }
        else
        {
            std::cerr << "before Right and Left\n";
            drawDictionary();
            rightRotation(node->right);
            std::cerr << "after Right, before Left\n";
            drawDictionary();
            leftRotation(node);
            std::cerr << "after Right Left rotation\n";
            drawDictionary();
            return;
        }
    }

}

template <typename Key, typename Info>
inline int Dictionary<Key, Info>::getBalance(Node *node) const
{
    if(!node)
        return -1;
    
    return (getHeight(node->left) - getHeight(node->right));
}

template <typename Key, typename Info>
inline int Dictionary<Key, Info>::getHeight(Node *node) const
{
    if(!node)
        return -1;
    
    int lheight = getHeight(node->left);
    int rheight = getHeight(node->right);

    if(lheight > rheight)
        return (1 + lheight);
    else
        return (1 + rheight);
}

template <typename Key, typename Info>
inline bool Dictionary<Key, Info>::findKey(const Key &key) const
{
    if(getNode(key))
        return true;
    return false;
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::insertNode(Node *&node, Node *newNode)
{
    if(!node)
    {
        node = newNode;
        return;
    }

    if(newNode->key > node->key)
        insertNode(node->right, newNode);
    else
        insertNode(node->left, newNode);
    
    balanceNode(node);
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::removeNode(Node *&node, const Key &key)
{
    if(!node)
        return;

    if(node->key == key)
    {
        if(!node->left || !node->right)
        {
            Node* temp = node;
            node = !node->left ? node->right : node->left;
            delete temp;
            return;
        }
        else
        {
            Node* successor = findSuccessor(node);
            
            node->key = successor->key;
            successor->key = key;

            Info temp = node->info;
            node->info = successor->info;
            successor->info = temp;

            removeNode(node->right, key);
        }
    }
    else
    {
        if(node->key < key)
            removeNode(node->right, key);
        else
            removeNode(node->left, key);
    }

    balanceNode(node);
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::clearTree(Node *&node)
{
    if(!node)
        return;

    clearTree(node->left);
    clearTree(node->right);

    delete node;
    node = nullptr;
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::copyTree(Node *&curr, const Node *node)
{
    if(!node)
        return;

    curr = new Node{node->key, node->info, nullptr, nullptr};
    elements++;

    copyTree(curr->left, node->left);
    copyTree(curr->right, node->right);
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::addTree(Node *node)
{
    if(!node)
        return;
    
    addTree(node->left);
    addTree(node->right);

    insert(node->key, node->info);
}

template <typename Key, typename Info>
inline typename Dictionary<Key, Info>::Node *Dictionary<Key, Info>::getNode(const Key &key) const
{
    Node* ptr = parent;
    while(ptr)
    {
        if(ptr->key == key)
            return ptr;
        if(ptr->key < key)
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }
    return nullptr;
}

template <typename Key, typename Info>
inline typename Dictionary<Key, Info>::Node *Dictionary<Key, Info>::findSuccessor(Node *node) const
{
    Node* curr = node->right;
    while(curr && curr->left)
        curr = curr->left;
    return curr;
}

template <typename Key, typename Info>
inline std::string Dictionary<Key, Info>::buildTree(Node *node, bool isLeft, const std::string prefix) const
{
    if (!node) {
        return "";
    }

    std::ostringstream oss;

    oss << prefix;
    if(node == parent)
        oss << "    ";
    else
        oss << (isLeft ? "|-- " : "\\-- ");
    oss << node->key << "\n";

    std::string childPrefix = prefix + (isLeft ? "|   " : "    ");
    oss << buildTree(node->left, true, childPrefix);
    oss << buildTree(node->right, false, childPrefix);

    return oss.str();
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::printNodes(Node *node, bool asc) const
{
    if(!node)
        return;
    
    printNodes(asc ? node->left : node->right, asc);
    std::cout << node->key << " : " << node->info << std::endl;
    printNodes(asc ? node->right : node->left, asc);
}

template <typename Key, typename Info>
inline Dictionary<Key, Info>::Dictionary()
{
    elements = 0;
    parent = nullptr;
}

template <typename Key, typename Info>
inline Dictionary<Key, Info>::Dictionary(const Dictionary &dict)
{
    parent = nullptr;
    elements = 0;
    copyTree(parent, dict.parent);
}

template <typename Key, typename Info>
inline Dictionary<Key, Info>::~Dictionary()
{
    clear();
    std::cout << "Dictionary " << this <<  " deleted\n";
}

template <typename Key, typename Info>
inline bool Dictionary<Key, Info>::insert(const Key &key, const Info &info)
{
    if(findKey(key))
        return false;
    
    Node* node = new Node;

    node->key = key;
    node->info = info;
    node->left = nullptr;
    node->right = nullptr;

    insertNode(parent, node);
    
    elements++;
    return true;
}

template <typename Key, typename Info>
inline bool Dictionary<Key, Info>::remove(const Key &key)
{
    if(!findKey(key))
        return false;
    removeNode(parent, key);

    elements--;
    return true;
}

template <typename Key, typename Info>
inline bool Dictionary<Key, Info>::update(const Key &key, const Info &info)
{
    Node* ptr = getNode(key);
    if(ptr)
    {
        ptr->info = info;
        return true;
    }
    return false;
}

template <typename Key, typename Info>
inline bool Dictionary<Key, Info>::clear()
{
    if(!parent)
        return false;

    clearTree(parent);
    elements = 0;
    return true;
}

template <typename Key, typename Info>
inline bool Dictionary<Key, Info>::copy(const Dictionary &dict)
{
    if(&dict == this)
        return false;

    clear();

    copyTree(parent, dict.parent);
    
    return true;
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::add(const Dictionary &dict)
{
    addTree(dict.parent);
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::drawDictionary() const
{
    std::string temp = buildTree(parent, false, "");
    std::cout << temp << std::endl;
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::printElements(bool asc) const
{
    printNodes(parent, asc);
}

template <typename Key, typename Info>
inline void Dictionary<Key, Info>::printKey(const Key &key) const
{
    Node* ptr = getNode(key);
    if(ptr)
    {
        std::cout << key << " : " << ptr->info << std::endl;
    }
    else
        std::cout << "Key not found\n";
}

template <typename Key, typename Info>
inline const int Dictionary<Key, Info>::getSize() const
{
    return elements;
}

template <typename Key, typename Info>
inline const Dictionary<Key, Info> &Dictionary<Key, Info>::operator=(const Dictionary &dict)
{
    copy(dict); // self assignment if here
    return *this;
}

template <typename Key, typename Info>
inline const Dictionary<Key, Info> Dictionary<Key, Info>::operator+(const Dictionary &dict)
{
    Dictionary<Key, Info> temp(*this);
    temp.add(dict);

    return temp;
}
