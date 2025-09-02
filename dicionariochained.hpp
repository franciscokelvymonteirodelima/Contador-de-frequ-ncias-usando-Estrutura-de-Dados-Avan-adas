#ifndef DICCIONARIO_CHAINED_HPP
#define DICCIONARIO_CHAINED_HPP

#include "ChainedHashTable.hpp" // Inclui o cabeçalho da sua tabela hash com encadeamento
#include <vector>
#include <utility> // Para std::pair

template<typename Key, typename Value>
class DicionarioChained {
private:
    
    ChainedHashTable<Key, Value> m_chainedHash;

public:
    // Construtor do dicionário. Passa os parâmetros iniciais para a ChainedHashTable interna.
    DicionarioChained(size_t tableSize = 19, float load_factor = 1.0)
        : m_chainedHash(tableSize, load_factor) {}

    // Adiciona um par chave-valor ao dicionário.
    // O método 'add' da ChainedHashTable já lida com a atualização se a chave existe.
    // :: Corrigido :: O método add não retorna bool aqui, pois a ChainedHashTable agora sempre atualiza se existe.
    void add(const Key& key, const Value& value) {
        m_chainedHash.add(key, value);
    }

    // Verifica se uma chave específica está presente no dicionário.
    // :: Corrigido :: Agora aceita APENAS a chave, como deveria ser para 'contains'.
    bool contains(const Key& key) const {
        return m_chainedHash.contains(key);
    }

    // Retorna o valor (frequência) associado a uma chave específica.
    // :: Corrigido :: Agora aceita APENAS a chave, como deveria ser para 'count'.
    // Retorna um valor padrão (ex: 0 para int) se a chave não for encontrada.
    Value count(const Key& key) const {
        return m_chainedHash.count(key);
    }

    // Remove uma chave do dicionário.
    void remove(const Key& key) {
        m_chainedHash.remove(key);
    }

    // Coleta todos os pares chave-valor do dicionário em um vetor.
    void getAllPairs(std::vector<std::pair<Key, Value>>& out) const {
        m_chainedHash.getAllPairs(out);
    }

    // Métodos para acessar as métricas de desempenho da tabela hash interna
    long long getComparacoesPrincipal() const { return m_chainedHash.getComparacoesPrincipal(); }
    long long getContadorRehash() const { return m_chainedHash.getContadorRehash(); }

    // Métodos para resetar os contadores da tabela hash interna
    void resetComparacoes() { m_chainedHash.resetComparacoes(); }
    void resetRehash() { m_chainedHash.resetRehash(); }

    // Retorna o número de elementos únicos no dicionário.
    size_t size() const { return m_chainedHash.size(); }

    void show() const {
    m_chainedHash.show();
}

};

#endif // DICCIONARIO_CHAINED_HPP