#ifndef NODE_HPP
#define NODE_HPP
#include <iostream>

#define RED true
#define BLACK false

template <typename T>
class RBNode {
    public:
    
    bool color;
    int ocorrencias = 1;
    T key;
    Node* left;
    Node* right;
    Node* parent;     

    Node(T key, bool color, Node* l, Node* r, Node* parent){
        this->key = key;
        this->color = color;
        left = l;
        right = r;
        this->parent = parent;
    }

    ~Node() {
    std::cout << "O nó (" << key.first << ", " << key.second << ") foi deletado." << std::endl;
}

};

#endif