#ifndef DICIONARIO_RB_HPP 
#define DICIONARIO_RB_HPP

#include <string>      
#include <vector>      
#include <stdexcept>   
#include "ARVORE_RUBRO_NEGRA/RBTree.hpp" 

// Define a classe DicionarioRb, que atua como uma 'interface' para sua RBTree.
// Ela permite usar a RBTree de forma mais amigável, como um dicionário.
template <typename Key, typename Value> // É um template, pode funcionar com qualquer tipo de Chave e Valor
class DicionarioRb {
private:
    // rb_tree é a sua Árvore Rubro-Negra real.
    // Ela é o "motor" que guarda e organiza os dados.
    rbtree<Key, Value> rb_tree; 

public:
    // Método para adicionar um par (chave e valor) ao dicionário.
    // Ele delega a tarefa para o método 'insert' da sua RBTree.
    void add(const Key& key, const Value& value) {
        rb_tree.insert(key, value);
    }

    // Método para remover uma chave (e seu valor) do dicionário.
    // Ele delega a tarefa para o método 'remove' da sua RBTree.
    void remove(const Key& key) {
        rb_tree.remove(key);
    }

    // Método para verificar se uma chave existe no dicionário.
    // Ele delega a tarefa para o método 'contains' da sua RBTree.
    bool contains(const Key& key) const {
        return rb_tree.contains(key);
    }

    // Método para obter o valor associado a uma chave.
    // Lança uma exceção 'std::out_of_range' se a chave não for encontrada.
    Value at(const Key& key) const {
        // Tenta encontrar o nó correspondente à chave na RBTree.
        RBNode<std::pair<Key, Value>>* node = rb_tree.searchNode(key);
        if (node == nullptr) { // Se o nó não for encontrado (ponteiro nulo)
            // Lança uma exceção indicando que a chave não está no dicionário.
            throw std::out_of_range("Chave nao encontrada no dicionario.");
        }
        // Retorna o segundo elemento do par (que é o valor) do nó encontrado.
        return node->key_value.second;
    }

    // Método para obter a frequência de ocorrências de uma chave.
    // Retorna 0 se a chave não for encontrada.
    int getFrequency(const Key& key) const {
        // Tenta encontrar o nó correspondente à chave.
        RBNode<std::pair<Key, Value>>* node = rb_tree.searchNode(key);
        // Se o nó for nulo, retorna 0 (chave não existe), senão, retorna o contador de ocorrências do nó.
        return (node == nullptr) ? 0 : node->ocorrencias;
    }

    // Aliás para o método getFrequency.
    // O nome 'count' é comumente usado em dicionários para obter a frequência/valor.
    int count(const Key& key) const {
        return getFrequency(key);
    }

    // Verifica se o dicionário está vazio.
    // Delega para o método 'empty' da RBTree.
    bool empty() const {
        return rb_tree.empty();
    }

    // Limpa todos os elementos do dicionário.
    // Delega para o método 'clear' da RBTree.
    void clear() {
        rb_tree.clear();
    }

    // Retorna o número de comparações de chaves realizadas na RBTree.
    // Usado para medir o desempenho.
    long long getComparacoesPrincipais() const {
        return rb_tree.getComparacoesPrincipais();
    }

    // Retorna o número de rotações realizadas na RBTree.
    // Usado para medir o desempenho de balanceamento.
    long long getRotacoes() const {
        return rb_tree.getRotacoes();
    }

    // Reseta o contador de comparações de chaves na RBTree.
    void resetComparacoes() {
        rb_tree.resetComparacoes();
    }

    // Reseta o contador de rotações na RBTree.
    void resetRotacoes() {
        rb_tree.resetRotacoes();
    }

    // Coleta todos os pares (chave e valor) do dicionário e os adiciona a um vetor.
    // Os pares serão coletados em ordem, pois árvores de busca mantêm essa propriedade.
    void getAllPairs(std::vector<std::pair<Key, Value>>& out_vector) const {
        // Chama um método da RBTree que percorre a árvore e preenche o vetor.
        rb_tree.inorderCollect(out_vector);
    }

    // Exibe a estrutura da árvore (útil para depuração visual).
    // Delega para o método 'bshow' da RBTree.
    void show() const {
        rb_tree.bshow();
    }
};

#endif // DICIONARIO_RB_HPP // Fim da guarda de inclusão