#include <iostream>
#include <string>

using namespace std;

class AVL {
private:
    struct tree
    {
        int size;
        uint64_t key;
        tree* left;
        tree* right;
        tree* parent;
        string value;
        int height;
        tree(uint64_t k, string v) : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), height(1), size(1) {}
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
    

};