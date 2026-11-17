#include <iostream>
#include <string>
#include <cstdint>

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
    
    tree* isert_in_tree(tree* node, const string& key, uint64_t value) {
        if (!node) return new tree(key, value);
        if (key < node->key) {
            node->left = isert_in_tree(node->left, key, value);
            node->left->parent = node;
        } else if (key > node->key) {
            node->right = isert_in_tree(node->right, key, value);
            node->right->parent = node;
        } else {
            node->value = value; 
            return node;
        }
        return balance(node);
    }

    tree* erase_in_tree(tree* node, const string& key) {
        if (!node) return nullptr;
        if (key < node->key) {
            node->left = erase_in_tree(node->left, key);
            if (node->left) node->left->parent = node;
        } else if (key > node->key) {
            node->right = erase_in_tree(node->right, key);
            if (node->right) node->right->parent = node;
        } else {
            if (!node->left || !node->right) {
                tree* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            tree* s = node->right;
            while (s->left) s = s->left;
            node->key = s->key;
            node->value = s->value;
            node->right = erase_in_tree(node->right, s->key);
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
    void insert(const string& key, uint64_t value) {
        root = isert_in_tree(root, key, value);
        if (root) root->parent = nullptr;
    }
    void erase(const string& key) {
        root = erase_in_tree(root, key);
        if (root) root->parent = nullptr;
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