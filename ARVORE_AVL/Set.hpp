#ifndef SET_HPP
#define SET_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <utility> // Para std::pair

template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>
struct AVLNode {
    KeyType key;
    ValueType value; // Agora armazena o valor separadamente
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const KeyType& k, const ValueType& v)
        : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>
class Set { // Renomeado para 'Set' mas funciona como um 'Map' AVL
public:
    Set() = default;
    ~Set() { destroy(root); }

    // Insere um par chave-valor na árvore AVL. Se a chave já existe, atualiza seu valor.
    void Insert(const KeyType& key, const ValueType& value) {
        root = _insert(root, key, value);
    }

    // Remove um elemento da árvore AVL pela chave.
    void Erase(const KeyType& key) {
        root = _erase(root, key);
    }

    // Verifica se uma chave está presente no conjunto.
    bool Contains(const KeyType& key) const {
        // Resetamos as comparações para que 'Contains' conte apenas as suas.
        // Se você quiser o total acumulado, não resete aqui.
        // Para testes de desempenho isolados, resetar é útil.
        // m_comparisons = 0; // Remova este reset se quiser acumular
        return _contains(root, key);
    }

    // Limpa todos os elementos do conjunto.
    void Clear() {
        destroy(root);
        root = nullptr;
        size = 0;
        m_comparisons = 0;
        m_rotations = 0;
    }

    // Retorna o menor elemento do conjunto (o par chave-valor).
    std::pair<KeyType, ValueType> Minimum() const {
        AVLNode<KeyType, ValueType>* node = find_min(root);
        if (!node) throw std::runtime_error("Conjunto vazio");
        return {node->key, node->value};
    }

    // Retorna o maior elemento do conjunto (o par chave-valor).
    std::pair<KeyType, ValueType> Maximum() const {
        AVLNode<KeyType, ValueType>* node = find_max(root);
        if (!node) throw std::runtime_error("Conjunto vazio");
        return {node->key, node->value};
    }

    // Retorna o sucessor de uma determinada chave no conjunto.
    std::pair<KeyType, ValueType> Successor(const KeyType& key) const {
        AVLNode<KeyType, ValueType>* node = find_successor(root, key);
        if (!node) throw std::runtime_error("Nao ha sucessor");
        return {node->key, node->value};
    }

    // Retorna o predecessor de uma determinada chave no conjunto.
    std::pair<KeyType, ValueType> Predecessor(const KeyType& key) const {
        AVLNode<KeyType, ValueType>* node = find_predecessor(root, key);
        if (!node) throw std::runtime_error("Nao ha predecessor");
        return {node->key, node->value};
    }

    // Verifica se o conjunto está vazio.
    bool Empty() const {
        return root == nullptr;
    }

    // Retorna o número de elementos únicos no conjunto.
    int Size() const {
        return size;
    }

    // Retorna o valor (frequência) associado a uma chave específica. Retorna ValueType{} se a chave não for encontrada.
    ValueType getCount(const KeyType& key) const {
        AVLNode<KeyType, ValueType>* curr = root;
        while (curr != nullptr) {
            m_comparisons++; // Comparação para decidir o caminho
            if (compare(key, curr->key)) { // key < curr->key
                curr = curr->left;
            } else if (compare(curr->key, key)) { // key > curr->key
                curr = curr->right;
            } else { // key == curr->key
                return curr->value;
            }
        }
        return ValueType{}; // Retorna um valor padrão (ex: 0 para int) se não encontrar
    }

    // Percorre a árvore em ordem (in-order) e aplica uma função a cada elemento (par chave-valor).
    void PrintInOrder(std::function<void(const KeyType&, const ValueType&)> func) const {
        in_order_print(root, func);
    }

    // Exibe a estrutura da árvore de forma visual (para depuração).
    void bshow() const {
        bshow(root, "");
    }

    // NOVO: retorna vetor ordenado com todos os elementos (pares chave-valor)
    std::vector<std::pair<KeyType, ValueType>> ToVector() const {
        std::vector<std::pair<KeyType, ValueType>> result;
        in_order_collect(root, result);
        return result;
    }

    // Métodos para acessar as métricas
    long long getComparacoesPrincipais() const { return m_comparisons; }
    long long getRotacoes() const { return m_rotations; }
    void resetComparacoes() { m_comparisons = 0; }
    void resetRotacoes() { m_rotations = 0; }

private:
    AVLNode<KeyType, ValueType>* root{nullptr};
    int size{0};
    mutable long long m_comparisons = 0; // 'mutable' para permitir incremento em métodos const
    long long m_rotations = 0;
    Compare compare; // Objeto comparador para chaves

    // Retorna a altura de um nó
    int height(AVLNode<KeyType, ValueType>* node) const {
        return node ? node->height : 0;
    }

    // Calcula o fator de balanceamento de um nó.
    // Já corrigido para 'const'
    int balance(AVLNode<KeyType, ValueType>* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Realiza uma rotação simples à direita.
    AVLNode<KeyType, ValueType>* right_rotation(AVLNode<KeyType, ValueType>* y) {
        AVLNode<KeyType, ValueType>* x = y->left;
        y->left = x->right;
        x->right = y;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    // Realiza uma rotação simples à esquerda.
    AVLNode<KeyType, ValueType>* left_rotation(AVLNode<KeyType, ValueType>* x) {
        AVLNode<KeyType, ValueType>* y = x->right;
        x->right = y->left;
        y->left = x;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    // Função auxiliar recursiva para inserir um elemento na árvore AVL e rebalancear.
    AVLNode<KeyType, ValueType>* _insert(AVLNode<KeyType, ValueType>* node, const KeyType& key, const ValueType& value) {
        if (!node) {
            size++;
            return new AVLNode<KeyType, ValueType>(key, value);
        }

        m_comparisons++; // Comparação para decidir o caminho
        if (compare(key, node->key)) { // key < node->key
            node->left = _insert(node->left, key, value);
        } else if (compare(node->key, key)) { // key > node->key
            node->right = _insert(node->right, key, value);
        } else { // key == node->key (chave já existe, atualiza o valor)
            node->value = value;
            return node;
        }

        node->height = std::max(height(node->left), height(node->right)) + 1;
        int bal = balance(node); // Chama a versão const de balance

        // Casos de rotação AVL
        // Rotação simples à direita (LL case)
        if (bal > 1 && compare(key, node->left->key)) {
            m_rotations++;
            return right_rotation(node);
        }
        // Rotação simples à esquerda (RR case)
        if (bal < -1 && compare(node->right->key, key)) {
            m_rotations++;
            return left_rotation(node);
        }
        // Rotação dupla à esquerda-direita (LR case)
        if (bal > 1 && compare(node->left->key, key)) { // key > node->left->key
            m_rotations += 2;
            node->left = left_rotation(node->left);
            return right_rotation(node);
        }
        // Rotação dupla à direita-esquerda (RL case)
        if (bal < -1 && compare(key, node->right->key)) { // key < node->right->key
            m_rotations += 2;
            node->right = right_rotation(node->right);
            return left_rotation(node);
        }
        return node;
    }

    // Função auxiliar recursiva para remover um elemento da árvore AVL e rebalancear.
    AVLNode<KeyType, ValueType>* _erase(AVLNode<KeyType, ValueType>* node, const KeyType& key) {
        if (!node) return nullptr;

        m_comparisons++; // Comparação para decidir o caminho
        if (compare(key, node->key)) { // key < node->key
            node->left = _erase(node->left, key);
        } else if (compare(node->key, key)) { // key > node->key
            node->right = _erase(node->right, key);
        } else {
            // Elemento encontrado
            // Caso 1: Nó sem filho ou com um filho
            if (!node->left || !node->right) {
                AVLNode<KeyType, ValueType>* temp = node->left ? node->left : node->right;
                delete node;
                size--;
                return temp;
            } else { // Caso 2: Nó com dois filhos
                AVLNode<KeyType, ValueType>* temp = find_min(node->right); // Encontra o sucessor in-order
                node->key = temp->key;
                node->value = temp->value; // Copia o valor também
                node->right = _erase(node->right, temp->key); // Remove o sucessor
            }
        }

        if (!node) return node; // Se o nó se tornou nulo após a remoção (caso de folha)

        node->height = std::max(height(node->left), height(node->right)) + 1;
        int bal = balance(node); // Chama a versão const de balance

        // Casos de rebalanceamento após a remoção
        // Rotação simples à direita
        if (bal > 1 && balance(node->left) >= 0) { // Chama a versão const de balance
            m_rotations++;
            return right_rotation(node);
        }
        // Rotação dupla à esquerda-direita
        if (bal > 1 && balance(node->left) < 0) { // Chama a versão const de balance
            m_rotations += 2;
            node->left = left_rotation(node->left);
            return right_rotation(node);
        }
        // Rotação simples à esquerda
        if (bal < -1 && balance(node->right) <= 0) { // Chama a versão const de balance
            m_rotations++;
            return left_rotation(node);
        }
        // Rotação dupla à direita-esquerda
        if (bal < -1 && balance(node->right) > 0) { // Chama a versão const de balance
            m_rotations += 2;
            node->right = right_rotation(node->right);
            return left_rotation(node);
        }
        return node;
    }

    // Função auxiliar recursiva para liberar a memória de todos os nós da árvore.
    void destroy(AVLNode<KeyType, ValueType>* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // Função auxiliar recursiva para verificar se um elemento está na árvore.
    bool _contains(AVLNode<KeyType, ValueType>* node, const KeyType& key) const {
        if (!node) return false;
        m_comparisons++; // Comparação
        if (compare(key, node->key)) return _contains(node->left, key);
        m_comparisons++; // Comparação
        if (compare(node->key, key)) return _contains(node->right, key);
        return true; // key == node->key
    }

    // Encontra o nó com o menor valor na subárvore.
    AVLNode<KeyType, ValueType>* find_min(AVLNode<KeyType, ValueType>* node) const {
        if (!node) return nullptr;
        while (node->left) {
            m_comparisons++; // Comparação ao ir para a esquerda
            node = node->left;
        }
        return node;
    }

    // Encontra o nó com o maior valor na subárvore.
    AVLNode<KeyType, ValueType>* find_max(AVLNode<KeyType, ValueType>* node) const {
        if (!node) return nullptr;
        while (node->right) {
            m_comparisons++; // Comparação ao ir para a direita
            node = node->right;
        }
        return node;
    }

    // Encontra o sucessor de um elemento na árvore.
    AVLNode<KeyType, ValueType>* find_successor(AVLNode<KeyType, ValueType>* node, const KeyType& key) const {
        AVLNode<KeyType, ValueType>* succ = nullptr;
        while (node) {
            m_comparisons++; // Comparação
            if (compare(key, node->key)) { // key < node->key
                succ = node;
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return succ;
    }

    // Encontra o predecessor de um elemento na árvore.
    AVLNode<KeyType, ValueType>* find_predecessor(AVLNode<KeyType, ValueType>* node, const KeyType& key) const {
        AVLNode<KeyType, ValueType>* pred = nullptr;
        while (node) {
            m_comparisons++; // Comparação
            if (compare(node->key, key)) { // node->key < key
                pred = node;
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return pred;
    }

    // Percorre a árvore em ordem (in-order) e aplica uma função a cada elemento (par chave-valor).
    void in_order_print(AVLNode<KeyType, ValueType>* node, const std::function<void(const KeyType&, const ValueType&)>& func) const {
        if (!node) return;
        in_order_print(node->left, func);
        func(node->key, node->value);
        in_order_print(node->right, func);
    }

    // Percorre a árvore em ordem (in-order) e coleta os elementos em um vetor.
    void in_order_collect(AVLNode<KeyType, ValueType>* node, std::vector<std::pair<KeyType, ValueType>>& vec) const {
        if (!node) return;
        in_order_collect(node->left, vec);
        vec.emplace_back(node->key, node->value);
        in_order_collect(node->right, vec);
    }

    // Exibe a estrutura da árvore de forma visual recursivamente.
    void bshow(AVLNode<KeyType, ValueType>* node, std::string heranca) const {
        if (node != nullptr && (node->left != nullptr || node->right != nullptr))
            bshow(node->right, heranca + "r");

        for (size_t i = 0; i + 1 < heranca.size(); i++)
            std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");

        if (!heranca.empty())
            std::cout << (heranca.back() == 'r' ? "┐───" : "└───");

        if (!node) {
            std::cout << "#" << std::endl;
            return;
        }

        // Ambas as chamadas a 'height' e 'balance' agora são const, resolvendo o erro.
        std::cout << node->key << ":" << node->value << " (h: " << node->height << ", bal: " << balance(node) << ")" << std::endl;

        if (node != nullptr && (node->left != nullptr || node->right != nullptr))
            bshow(node->left, heranca + "l");
    }
};

#endif // SET_HPP