#ifndef NODE_H
#define NODE_H

#include <iostream>

template <typename T>
struct AVLNode {
    T key;
    int height;
    int contador; // contador de inserções
    Node* left;
    Node* right;

    // Construtor padrão
    Node(const T& k)
        : key(k), height(1), contador(1), left(nullptr), right(nullptr) {}
};

#endif // NODE_H
