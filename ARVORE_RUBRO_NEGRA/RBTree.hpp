#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <iostream>
#include <string>
#include <utility>    // Para std::pair
#include <vector>     // Para std::vector em inorderCollect
#include <functional> // Para std::function

// Definições de cores para os nós da árvore
#define RED true
#define BLACK false

// Estrutura do Nó da Árvore Rubro-Negra
template <typename T> // T será std::pair<Key, Value>
struct RBNode {
    bool color;       // Cor do nó (RED ou BLACK)
    T key_value;      // O par (chave, valor) armazenado no nó
    int ocorrencias;  // Contador de ocorrências do elemento

    RBNode *left;     // Ponteiro para o filho esquerdo
    RBNode *right;    // Ponteiro para o filho direito
    RBNode *parent;   // Ponteiro para o pai

    // Construtor do nó
    RBNode(const T& kv, bool c, RBNode* l, RBNode* r, RBNode* p)
        : color(c), key_value(kv), ocorrencias(1), left(l), right(r), parent(p) {}

    // Removido o destrutor com cout, pois em grandes árvores isso causa muito output.
    // Se precisar para depuração, pode recolocá-lo temporariamente.
    // ~RBNode() {
    //     std::cout << "O nó (" << key_value.first << ", " << key_value.second << ") foi deletado." << std::endl;
    // }
};

// Classe da Árvore Rubro-Negra
template <typename Key, typename Value>
class rbtree {
private:
    using Pair = std::pair<Key, Value>; // Alias para o tipo de par

    RBNode<Pair>* root; // Raiz da árvore
    RBNode<Pair>* nil;  // Nó sentinela (NIL)

    mutable long long comparacoes_principais = 0; // Contador de comparações (mutable para const methods)
    long long comparacoes_rotacoes = 0;           // Contador de rotações

    // Função auxiliar para criar e inicializar o nó NIL
    RBNode<Pair>* create_nil_node() {
        // O nó NIL armazena um Pair padrão, mas sua Key e Value não importam.
        // É essencial que ele seja BLACK.
        return new RBNode<Pair>(Pair(), BLACK, nullptr, nullptr, nullptr);
    }

    // Funções de rotação
    void leftRotate(RBNode<Pair>* x) {
        comparacoes_rotacoes++; // Incrementa contador de rotações
        RBNode<Pair>* y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode<Pair>* y) {
        comparacoes_rotacoes++; // Incrementa contador de rotações
        RBNode<Pair>* x = y->left;
        y->left = x->right;
        if (x->right != nil) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nil) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    // Função de fixup após inserção
    void insertFixup(RBNode<Pair>* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBNode<Pair>* y = z->parent->parent->right; // Tio
                if (y->color == RED) { // Caso 1
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else { // Caso 2 & 3
                    if (z == z->parent->right) { // Caso 2
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK; // Caso 3
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else { // Simétrico
                RBNode<Pair>* y = z->parent->parent->left; // Tio
                if (y->color == RED) { // Caso 1
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else { // Caso 2 & 3
                    if (z == z->parent->left) { // Caso 2
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK; // Caso 3
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Função de fixup após remoção
    void removeFixup(RBNode<Pair>* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                RBNode<Pair>* w = x->parent->right; // Irmão
                if (w->color == RED) { // Caso 1
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) { // Caso 2
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) { // Caso 3
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color; // Caso 4
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else { // Simétrico
                RBNode<Pair>* w = x->parent->left; // Irmão
                if (w->color == RED) { // Caso 1
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) { // Caso 2
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) { // Caso 3
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color; // Caso 4
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Substitui a subárvore enraizada em 'u' pela subárvore enraizada em 'v'.
    void transplant(RBNode<Pair>* u, RBNode<Pair>* v) {
        if (u->parent == nil)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    // Encontra o nó com a menor chave em uma subárvore.
    RBNode<Pair>* minimum(RBNode<Pair>* node) {
        while (node->left != nil)
            node = node->left;
        return node;
    }

    // Limpa a árvore recursivamente
    void clearInternal(RBNode<Pair>* node) {
        if (node == nil) return;
        clearInternal(node->left);
        clearInternal(node->right);
        delete node;
    }

    // Percorre a árvore em ordem e coleta os pares (chave, valor), incluindo ocorrências
    void inorderCollectNode(RBNode<Pair>* node, std::vector<Pair>& out) const {
        if (node == nil) return;
        inorderCollectNode(node->left, out);
        out.push_back(node->key_value);
        inorderCollectNode(node->right, out);
    }

    // Busca interna que retorna o nó, ou nil se não encontrado
    RBNode<Pair>* search(RBNode<Pair>* node, const Key& key) const {
        while (node != nil) {
            comparacoes_principais++; // Incrementa o contador de comparações.
            if (key == node->key_value.first)
                return node;
            if (key < node->key_value.first)
                node = node->left;
            else
                node = node->right;
        }
        return nil; // Chave não encontrada.
    }

    // *** CORREÇÃO NA FUNÇÃO bshow_internal ***
    // Esta versão tenta desenhar a árvore de forma mais tradicional (esquerda-direita, de cima para baixo).
    void bshow_internal(RBNode<Pair>* node, std::string prefix, bool is_left) const {
        if (node == nil) {
            // Se você quiser mostrar os NILS, descomente a linha abaixo
            // std::cout << prefix << (is_left ? "├──" : "└──") << " (NIL)\n";
            return;
        }

        // Desenha o filho direito primeiro para que a árvore pareça "crescer" para a esquerda
        // ou para manter a estrutura típica de bshow (raiz, direita, esquerda)
        bshow_internal(node->right, prefix + (is_left ? "│   " : "    "), false);

        std::cout << prefix;
        if (is_left) {
            std::cout << "└── ";
        } else {
            std::cout << "├── ";
        }
        std::cout << "(" << (node->color == RED ? "R" : "B") << ") "
                  << node->key_value.first << ":" << node->key_value.second
                  << " (ocor: " << node->ocorrencias << ")\n";

        // Desenha o filho esquerdo
        bshow_internal(node->left, prefix + (is_left ? "    " : "│   "), true);
    }


public:
    // Construtor da rbtree
    rbtree() {
        nil = create_nil_node(); // Inicializa o nó sentinela
        nil->left = nil->right = nil->parent = nil; // Sentinela aponta para si mesmo
        root = nil;             // A árvore inicialmente está vazia (raiz aponta para nil)
    }

    // Destrutor da rbtree
    ~rbtree() {
        clearInternal(root); // Libera todos os nós da árvore
        delete nil;        // Libera o nó sentinela
    }

    // Insere uma chave e um valor (considerando a frequência/ocorrência)
    void insert(const Key& key, const Value& value) {
        RBNode<Pair>* parent = nil;
        RBNode<Pair>* current = root;

        // Percorre a árvore para encontrar o local de inserção ou a chave existente.
        while (current != nil) {
            parent = current;
            comparacoes_principais++; // Incrementa o contador de comparações
            if (key == current->key_value.first) { // Se a chave já existe, atualiza o valor e incrementa ocorrências
                current->key_value.second = value; // Atualiza o valor
                current->ocorrencias++;      // Incrementa ocorrências
                return;
            }
            if (key < current->key_value.first) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Cria o novo nó, com a cor VERMELHA e ocorrências = 1.
        RBNode<Pair>* newNode = new RBNode<Pair>(Pair(key, value), RED, nil, nil, parent);
        newNode->ocorrencias = 1; // Garante que novas inserções iniciem com 1 ocorrência

        // Conecta o novo nó ao seu pai.
        if (parent == nil) {
            root = newNode;
        } else if (key < parent->key_value.first) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        insertFixup(newNode); // Chama a função para corrigir as propriedades da Árvore Vermelho-Preta.
    }

    // Remove uma chave. Se tiver mais de uma ocorrência, decrementa o contador. Se for 1, remove o nó.
    void remove(const Key& key) {
        RBNode<Pair>* z = search(root, key); // Encontra o nó a ser removido/decrementado

        if (z == nil) return; // Nó não encontrado.

        if (z->ocorrencias > 1) { // Se tem mais de uma ocorrência, apenas decrementa
            z->ocorrencias--;
            return;
        }

        // Se ocorrencias == 1, procede com a remoção normal do nó RBTree
        RBNode<Pair>* y = z;
        RBNode<Pair>* x;
        bool y_original_color = y->color;

        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            y->ocorrencias = z->ocorrencias; // Copia as ocorrências para o nó substituto
        }

        delete z; // Libera o nó original

        if (y_original_color == BLACK) {
            removeFixup(x);
        }
    }

    // Limpa todos os elementos da árvore, tornando-a vazia.
    void clear() {
        clearInternal(root);
        root = nil; // A raiz volta a ser o nó nil
        // Resetar contadores ao limpar
        comparacoes_principais = 0;
        comparacoes_rotacoes = 0;
    }

    // Verifica se uma chave está presente na árvore.
    bool contains(const Key& key) const {
        return search(root, key) != nil;
    }

    // Procura por um nó com a chave especificada e retorna um ponteiro para ele.
    // Retorna nullptr se a chave não for encontrada.
    // Este método é const porque não modifica a árvore.
    RBNode<Pair>* searchNode(const Key& key) const {
        RBNode<Pair>* res = search(root, key);
        return (res == nil) ? nullptr : res;
    }

    // Verifica se a árvore está vazia.
    bool empty() const {
        return root == nil;
    }

    // Retorna o número de comparações principais realizadas.
    long long getComparacoesPrincipais() const { return comparacoes_principais; }
    // Retorna o número de rotações realizadas.
    long long getRotacoes() const { return comparacoes_rotacoes; }
    // Reseta o contador de comparações principais.
    void resetComparacoes() { comparacoes_principais = 0; }
    // Reseta o contador de rotações.
    void resetRotacoes() { comparacoes_rotacoes = 0; }

    // Função para coletar todos os pares em ordem crescente (para uso externo).
    // Preenche um vetor com todos os elementos da árvore, incluindo as ocorrências.
    void inorderCollect(std::vector<Pair>& out) const {
        out.clear();
        inorderCollectNode(root, out);
    }

    // Função para exibir a árvore de forma visual.
    void bshow() const {
        if (root == nil) {
            std::cout << "Arvore vazia.\n";
            return;
        }
        bshow_internal(root, "", true); // A raiz sempre é tratada como "filho esquerdo" para a indentação inicial
    }
};

#endif // RBTREE_HPP