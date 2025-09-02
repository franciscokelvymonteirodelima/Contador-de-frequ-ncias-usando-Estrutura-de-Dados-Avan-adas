#ifndef DICIONARIO_AVL_HPP
#define DICIONARIO_AVL_HPP

#include <iostream>
#include <utility>
#include <vector>
#include "ARVORE_AVL/Set.hpp" // Inclui o cabeçalho da classe Set baseada em AVL.

template<typename Key, typename Value>
class DicionarioAvl {
private:
    // A implementação do dicionário utiliza uma instância da classe Set (agora um Map AVL).
    // O Set armazena Key e Value separadamente, e balanceia pela Key.
    Set<Key, Value> m_avl;

public:
    // Adiciona um par chave-valor ao dicionário.
    // Se a chave já existe, o método Insert do Set atualizará o valor.
    void add(const Key& key, const Value& value) {
        m_avl.Insert(key, value);
    }

    // Remove uma chave do dicionário.
    void remove(const Key& key) {
        m_avl.Erase(key);
    }

    // Verifica se uma chave específica está presente no dicionário.
    bool contains(const Key& key) const {
        return m_avl.Contains(key);
    }

    // Retorna o valor (frequência) associado a uma chave específica.
    // Se a chave não for encontrada, retorna um valor padrão (ex: 0 para int).
    Value count(const Key& key) const {
        return m_avl.getCount(key);
    }

    // Exibe todos os pares chave-valor do dicionário em ordem crescente.
    void show() const {
        m_avl.PrintInOrder([](const Key& k, const Value& v) {
            std::cout << k << ":" << v << " ";
        });
        std::cout << std::endl;
    }

    // Exibe a estrutura da árvore AVL subjacente, para fins de depuração.
    void bshow() const {
        m_avl.bshow();
    }

    // Limpa o dicionário, removendo todos os pares chave-valor.
    void clear() {
        m_avl.Clear();
    }

    // Verifica se o dicionário está vazio.
    bool empty() const {
        return m_avl.Empty();
    }

    // Retorna o número de elementos únicos (chaves) no dicionário.
    int size() const {
        return m_avl.Size();
    }

    // Retorna o menor par chave-valor presente no dicionário.
    std::pair<Key, Value> minimum() const {
        return m_avl.Minimum();
    }

    // Retorna o maior par chave-valor presente no dicionário.
    std::pair<Key, Value> maximum() const {
        return m_avl.Maximum();
    }

    // Retorna o sucessor de uma determinada chave no dicionário.
    std::pair<Key, Value> successor(const Key& key) const {
        return m_avl.Successor(key);
    }

    // Retorna o predecessor de uma determinada chave no dicionário.
    std::pair<Key, Value> predecessor(const Key& key) const {
        return m_avl.Predecessor(key);
    }

    // Retorna um vetor contendo todos os pares chave-valor do dicionário, em ordem crescente.
    std::vector<std::pair<Key, Value>> getAllOrdered() const {
        return m_avl.ToVector();
    }

    // Métodos para acessar as métricas de desempenho da AVL interna
    long long getComparacoesPrincipais() const { return m_avl.getComparacoesPrincipais(); }
    long long getRotacoes() const { return m_avl.getRotacoes(); }
    void resetComparacoes() { m_avl.resetComparacoes(); }
    void resetRotacoes() { m_avl.resetRotacoes(); }
};

#endif // DICIONARIO_AVL_HPP