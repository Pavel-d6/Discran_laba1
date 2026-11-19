#include <iostream>
#include <string>
#include <cstdint>
#include <cctype>

using namespace std;

class AVL {
private:
    struct tree
    {
        int size;
        uint64_t value;
        tree* left;
        tree* right;
        tree* parent;
        string key;
        int height;
        tree(string k, uint64_t v) : size(1), value(v), left(nullptr), right(nullptr), parent(nullptr), key(k), height(1) {}
    };
 
    tree *root = nullptr; 

    static int height(tree* node) {
        return node ? node->height : 0;
    }
    static int balanceFactor(tree* node) {
        return height(node->left) - height(node->right);
    }
    static int size(tree* node) {
        return node ? node->size : 0;
    }

    static void update(tree* node) {
        if (node) {
            node->height = 1 + max(height(node->left), height(node->right));
            node->size = 1 + size(node->left) + size(node->right);
        }
    }

    static tree* rotateLeft(tree* node) {
        tree* newRoot = node->right;
        node->right = newRoot->left;
        if (newRoot->left) newRoot->left->parent = node;
        newRoot->left = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        update(node);
        update(newRoot);
        return newRoot;
    }

    static tree* rotateRight(tree* node) {
        tree* newRoot = node->left;
        node->left = newRoot->right;
        if (newRoot->right) newRoot->right->parent = node;
        newRoot->right = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        update(node);
        update(newRoot);
        return newRoot;
    }

    static tree* balance(tree* node) {
        update(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    
    tree* insert_in_tree(tree* node, const string& key, uint64_t value, bool& inserted) {
        if (!node) {
            inserted = true;
            return new tree(key, value);
        }
        if (key < node->key) {
            node->left = insert_in_tree(node->left, key, value, inserted);
            node->left->parent = node;
        } else if (key > node->key) {
            node->right = insert_in_tree(node->right, key, value, inserted);
            node->right->parent = node;
        } else {
            inserted = false;
            return node;
        }
        return balance(node);
    }

    tree* erase_in_tree(tree* node, const string& key, bool& erased) {
        if (!node) return nullptr;
        if (key < node->key) {
            node->left = erase_in_tree(node->left, key, erased);
            if (node->left) node->left->parent = node;
        } else if (key > node->key) {
            node->right = erase_in_tree(node->right, key, erased);
            if (node->right) node->right->parent = node;
        } else {
            erased = true;
            if (!node->left || !node->right) {
                tree* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            tree* s = node->right;
            while (s->left) s = s->left;
            node->key = s->key;
            node->value = s->value;
            bool successorErased = false;
            node->right = erase_in_tree(node->right, s->key, successorErased);
            if (node->right) node->right->parent = node;
        }
        return balance(node);
        
    }

    tree* find_min(tree* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    void Clear_tree(tree* node) {
        if (!node) return;
        Clear_tree(node->left);
        Clear_tree(node->right);
        delete node;
    }

public:
    AVL() = default;
    ~AVL() {
        clear();
    }
    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    bool insert(const string& key, uint64_t value) {
        bool inserted = false;
        root = insert_in_tree(root, key, value, inserted);
        if (root) root->parent = nullptr;
        return inserted;
    }
    bool erase(const string& key) {
        bool erased = false;
        root = erase_in_tree(root, key, erased);
        if (root) root->parent = nullptr;
        return erased;
    }
    void clear() {
        Clear_tree(root);
        root = nullptr;
    }
    bool find(const string& key, uint64_t& value) const {
        tree* node = root;
        while (node) {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else {
                value = node->value;
                return true;
            }
        }
        return false;
    }
    
};

static string toLower(string word) {
    for (char& ch : word) {
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }
    return word;
}

int main() {
    AVL dictionary;
    string command;

    while (cin >> command) {
        if (command == "+") {
            string word;
            uint64_t value;
            if (!(cin >> word >> value))
                break;
            word = toLower(word);
            cout << (dictionary.insert(word, value) ? "OK" : "Exist") << '\n';
        } else if (command == "-") {
            string word;
            if (!(cin >> word))
                break;
            word = toLower(word);
            cout << (dictionary.erase(word) ? "OK" : "NoSuchWord") << '\n';
        } else if (command == "?" || command == "find" || command == "search") {
            string word;
            if (!(cin >> word))
                break;
            word = toLower(word);
            uint64_t value;
            cout << (dictionary.find(word, value)
                         ? "OK: " + to_string(value)
                         : "NoSuchWord")
                 << '\n';
        } else {
            uint64_t value;
            string word = toLower(command);
            if (dictionary.find(word, value))
                cout << "OK: " << value << '\n';
            else
                cout << "NoSuchWord" << '\n';
        }
    }

    return 0;
}