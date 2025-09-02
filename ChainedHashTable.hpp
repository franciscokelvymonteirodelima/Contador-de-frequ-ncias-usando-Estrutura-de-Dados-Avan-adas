#ifndef CHAINEDHASHTABLE_HPP
#define CHAINEDHASHTABLE_HPP

#include <iostream>
#include <cmath>
#include <string>
#include <list>
#include <vector>
#include <utility>
#include <functional> // Para std::hash
#include <algorithm>  // Para std::move

// A função de hash para o par não é mais usada diretamente para o hash do bucket,
// já que a tabela agora funciona como um mapa KeyType -> ValueType.
// A função de hash será baseada apenas na KeyType.

template <typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
class ChainedHashTable {
private:
    // Estrutura interna para armazenar cada elemento na tabela hash.
    struct Elemento {
        KeyType key;    // A chave do elemento.
        ValueType value; // O valor associado à chave (frequência, por exemplo).
        // Construtor que inicializa a chave e o valor.
        Elemento(const KeyType& k, const ValueType& v) : key(k), value(v) {}
    };

    size_t m_number_of_elements = 0; // Número total de elementos únicos na tabela.
    size_t m_table_size;             // Tamanho atual da tabela (número de buckets).
    float m_max_load_factor;         // Fator de carga máximo permitido antes de um rehash.
    std::vector<std::list<Elemento>> m_table; // A tabela hash, implementada como um vetor de listas (encadeamento).
    Hash m_hashing;                          // Objeto de função hash para calcular os códigos hash.

    long long comparacoes_principal = 0; // Contador para o número de comparações de chaves realizadas.
    long long contador_rehash = 0;       // Contador para o número de operações de rehash realizadas.

    // Retorna o próximo número primo maior ou igual a 'x'.
    // Usado para garantir que o tamanho da tabela seja um número primo, o que ajuda a distribuir melhor os hashes.
    size_t get_next_prime(size_t x) {
        if(x <= 2) return 3; // Os primeiros primos são 2, 3. Se x for 0, 1 ou 2, retorna 3.
        x = (x % 2 == 0) ? x + 1 : x; // Garante que 'x' seja ímpar para otimizar a verificação.
        bool not_prime = true;
        while(not_prime) {
            not_prime = false;
            // Itera apenas sobre números ímpares até a raiz quadrada de 'x' para verificar a primalidade.
            for(int i = 3; i <= sqrt(x); i += 2) {
                if(x % i == 0) { // Se 'x' for divisível por 'i', não é primo.
                    not_prime = true;
                    break;
                }
            }
            x += 2; // Tenta o próximo número ímpar.
        }
        return x - 2; // Retorna o primo encontrado.
    }

    // Calcula o código hash para uma dada chave e mapeia-o para um slot na tabela.
    size_t hash_code(const KeyType& key) const {
        return m_hashing(key) % m_table_size; // Aplica a função hash e usa o módulo para obter o índice do bucket.
    }

public:
    // Construtor da tabela hash.
    // Inicializa o tamanho da tabela com um primo e define o fator de carga máximo.
    ChainedHashTable(size_t tableSize = 19, float load_factor = 1.0) {
        m_table_size = get_next_prime(tableSize); // Define o tamanho da tabela para o próximo primo.
        m_table.resize(m_table_size);             // Redimensiona o vetor para o tamanho da tabela.
        // Garante que o fator de carga seja um valor positivo.
        m_max_load_factor = (load_factor <= 0) ? 1.0f : load_factor;
    }

    // Adiciona uma chave e um valor à tabela hash. Se a chave já existe, seu valor é atualizado.
    void add(const KeyType& key, const ValueType& value) {
        // Verifica se o fator de carga atual excede o máximo permitido e faz um rehash se necessário.
        if(load_factor() >= m_max_load_factor)
            rehash(2 * m_table_size); // Dobra o tamanho da tabela para o rehash.

        size_t slot = hash_code(key); // Calcula o slot (índice do bucket) para a chave.
        // Percorre a lista no slot para verificar se a chave já existe.
        for(auto& elem : m_table[slot]) {
            comparacoes_principal++; // Incrementa o contador de comparações.
            if(elem.key == key) {     // Se a chave for encontrada, atualiza seu valor.
                elem.value = value;
                return; // Sai da função, pois a atualização foi feita.
            }
        }
        // Se a chave não foi encontrada, adiciona um novo elemento ao final da lista no slot.
        m_table[slot].push_back(Elemento(key, value));
        m_number_of_elements++; // Incrementa o número de elementos únicos.
    }

    // Verifica se uma chave está presente na tabela hash.
    bool contains(const KeyType& key) const {
        size_t slot = hash_code(key); // Calcula o slot da chave.
        // Percorre a lista no slot para verificar a presença do elemento.
        for(const auto& elem : m_table[slot]) {
            if(elem.key == key) return true; // Se encontrado, retorna true.
        }
        return false; // Se não encontrado após percorrer a lista, retorna false.
    }

    // Retorna o valor associado a uma chave específica.
    // Retorna um valor padrão (ValueType()) se a chave não for encontrada.
    ValueType count(const KeyType& key) const {
        size_t slot = hash_code(key); // Calcula o slot da chave.
        // Percorre a lista no slot para encontrar o elemento.
        for(const auto& elem : m_table[slot]) {
            if(elem.key == key) return elem.value; // Se encontrado, retorna seu valor.
        }
        return ValueType(); // Se não encontrado, retorna um valor padrão para ValueType (ex: 0 para int, string vazia para string).
    }

    // Remove uma chave da tabela hash.
    void remove(const KeyType& key) {
        size_t slot = hash_code(key);
        // Itera pela lista para encontrar e remover o elemento.
        // Usa `std::list::erase` com o iterador.
        for(auto it = m_table[slot].begin(); it != m_table[slot].end(); ++it) {
            if(it->key == key) {
                m_table[slot].erase(it);
                m_number_of_elements--;
                return;
            }
        }
    }

    // Realiza uma operação de rehash, redimensionando a tabela para um novo tamanho.
    // Isso é feito para manter o fator de carga abaixo do limite, melhorando o desempenho.
    void rehash(size_t new_size) {
        size_t prime = get_next_prime(new_size); // Obtém o próximo número primo para o novo tamanho.
        if(prime > m_table_size) { // Só faz rehash se o novo tamanho for maior que o atual.
            contador_rehash++; // Incrementa o contador de rehash.
            std::vector<std::list<Elemento>> old_table = std::move(m_table); // Move para evitar cópia desnecessária.
            m_table.clear();                                      // Limpa a tabela atual.
            m_table.resize(prime);                                // Redimensiona a tabela para o novo tamanho primo.
            m_table_size = prime;                                 // Atualiza o tamanho da tabela.
            m_number_of_elements = 0;                             // Reseta o número de elementos (serão reinseridos).
            // Percorre todos os elementos da tabela antiga e os readiciona à nova tabela.
            for(auto& bucket : old_table) {
                for(auto& elem : bucket) {
                    add(elem.key, elem.value); // Readiciona cada elemento com sua chave e valor.
                }
            }
        }
    }

    // Coleta todos os pares (chave, valor) da tabela hash em um vetor.
    void getAllPairs(std::vector<std::pair<KeyType, ValueType>>& out) const {
        out.clear(); // Limpa o vetor de saída.
        for(const auto& bucket : m_table) { // Itera sobre cada bucket.
            for(const auto& elem : bucket) { // Itera sobre cada elemento na lista do bucket.
                // Adiciona o par (chave do item, valor) ao vetor de saída.
                out.emplace_back(elem.key, elem.value);
            }
        }
    }

    // Calcula e retorna o fator de carga atual da tabela hash.
    float load_factor() const {
        // Fator de carga = (Número de elementos únicos) / (Tamanho da tabela).
        if (m_table_size == 0) return 0.0f; // Evita divisão por zero se a tabela for vazia.
        return static_cast<float>(m_number_of_elements) / m_table_size;
    }

    // Retorna o número total de comparações de chaves realizadas durante as operações de adição.
    long long getComparacoesPrincipal() const { return comparacoes_principal; }
    // Retorna o número de vezes que a tabela foi rehashada.
    long long getContadorRehash() const { return contador_rehash; }

    // Métodos para resetar os contadores.
    void resetComparacoes() { comparacoes_principal = 0; }
    void resetRehash() { contador_rehash = 0; }

    // Retorna o número de elementos únicos.
    size_t size() const { return m_number_of_elements; }

    void show() const {
    std::cout << "ChainedHashTable (" << m_number_of_elements << " elementos, "
              << "tamanho da tabela = " << m_table_size << ")\n";
    for (size_t i = 0; i < m_table_size; ++i) {
        std::cout << "Slot " << i << ": ";
        if (m_table[i].empty()) {
            std::cout << "(vazio)";
        } else {
            bool first = true;
            for (const auto& elem : m_table[i]) {
                if (!first) std::cout << " -> ";
                std::cout << "(" << elem.key << ", " << elem.value << ")";
                first = false;
            }
        }
        std::cout << "\n";
    }
}

};

#endif // CHAINEDHASHTABLE_HPP