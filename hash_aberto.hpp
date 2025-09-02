#ifndef HASH_ABERTO_HPP
#define HASH_ABERTO_HPP

#include <vector>
#include <functional>
#include <stdexcept>
#include <utility>
#include <optional>
#include <iostream>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashAberto {
private:
    enum class Estado { VAZIO, OCUPADO, REMOVIDO };

    struct Slot {
        std::optional<Key> chave;
        std::optional<Value> valor;
        int contador = 0;
        Estado estado = Estado::VAZIO;
        Slot() = default;
    };

    std::vector<Slot> m_table;
    size_t m_table_size;
    size_t m_number_of_elements;
    float m_max_load_factor;
    Hash m_hashing;

    mutable long m_comparacoes_principais = 0; // contador comparações de chaves
    size_t m_rehashes = 0;                // contador de rehashes

    size_t hash_code(const Key& k) const {
        return m_hashing(k) % m_table_size;
    }

    int aux_hash_search(const Key& k) const {
        size_t i = 0;
        while (i < m_table_size) {
            size_t j = (hash_code(k) + i) % m_table_size;
            if (m_table[j].estado == Estado::OCUPADO) {
                m_comparacoes_principais++; // comparação de chave
                if (m_table[j].chave && *(m_table[j].chave) == k) {
                    return static_cast<int>(j);
                }
            }
            if (m_table[j].estado == Estado::VAZIO) {
                return -1;
            }
            ++i;
        }
        return -1;
    }

public:
    HashAberto(size_t tableSize = 19, float load_factor = 0.7)
        : m_table_size(tableSize), m_number_of_elements(0), m_max_load_factor(load_factor) {
        m_table.resize(m_table_size);
    }

    size_t size() const { return m_number_of_elements; }
    bool empty() const { return m_number_of_elements == 0; }
    size_t bucket_count() const { return m_table_size; }
    float load_factor() const { return static_cast<float>(m_number_of_elements) / m_table_size; }
    float max_load_factor() const { return m_max_load_factor; }

    void clear() {
        m_table.clear();
        m_table.resize(m_table_size);
        m_number_of_elements = 0;
        m_comparacoes_principais = 0;
        m_rehashes = 0;
    }

    bool insert(const Key& k, const Value& v) {
        if (load_factor() > m_max_load_factor) {
            rehash(m_table_size * 2 + 1);
        }
        size_t i = 0;
        int index = -1;
        while (i < m_table_size) {
            size_t j = (hash_code(k) + i) % m_table_size;

            if (m_table[j].estado == Estado::OCUPADO) {
                m_comparacoes_principais++; // comparação chave
                if (m_table[j].chave && *(m_table[j].chave) == k) {
                    m_table[j].valor = v;
                    m_table[j].contador++;
                    return true;
                }
            } else if (m_table[j].estado == Estado::VAZIO) {
                if (index == -1)
                    index = j;
                break;
            } else if (m_table[j].estado == Estado::REMOVIDO) {
                if (index == -1)
                    index = j;
            }
            ++i;
        }
        if (index != -1) {
            m_table[index].chave = k;
            m_table[index].valor = v;
            m_table[index].contador = 1;
            m_table[index].estado = Estado::OCUPADO;
            ++m_number_of_elements;
            return true;
        }
        return false;
    }

    bool remove(const Key& k) {
        size_t i = 0;
        while (i < m_table_size) {
            size_t j = (hash_code(k) + i) % m_table_size;
            if (m_table[j].estado == Estado::OCUPADO) {
                m_comparacoes_principais++; // comparação chave
                if (m_table[j].chave && *(m_table[j].chave) == k) {
                    m_table[j].estado = Estado::REMOVIDO;
                    --m_number_of_elements;
                    return true;
                }
            } else if (m_table[j].estado == Estado::VAZIO) {
                return false;
            }
            ++i;
        }
        return false;
    }

    bool contains(const Key& k) const {
        return aux_hash_search(k) != -1;
    }

    Value& at(const Key& k) {
        int idx = aux_hash_search(k);
        if (idx == -1) throw std::out_of_range("Chave não encontrada");
        return *(m_table[idx].valor);
    }

    const Value& at(const Key& k) const {
        int idx = aux_hash_search(k);
        if (idx == -1) throw std::out_of_range("Chave não encontrada");
        return *(m_table[idx].valor);
    }

    void rehash(size_t new_size) {
        m_rehashes++; // conta rehash
        std::vector<Slot> old_table = m_table;
        m_table_size = new_size;
        m_table.clear();
        m_table.resize(m_table_size);
        m_number_of_elements = 0;

        for (const auto& slot : old_table) {
            if (slot.estado == Estado::OCUPADO && slot.chave && slot.valor) {
                size_t i = 0;
                int index = -1;
                while (i < m_table_size) {
                    size_t j = (hash_code(*(slot.chave)) + i) % m_table_size;
                    if (m_table[j].estado != Estado::OCUPADO) {
                        index = j;
                        break;
                    }
                    ++i;
                }
                if (index != -1) {
                    m_table[index].chave = slot.chave;
                    m_table[index].valor = slot.valor;
                    m_table[index].contador = slot.contador;
                    m_table[index].estado = Estado::OCUPADO;
                    ++m_number_of_elements;
                }
            }
        }
    }

    void show() const {
        std::cout << "Indice\tEstado\tChave\tValor\tContador\n";
        for (size_t i = 0; i < m_table_size; ++i) {
            std::cout << i << "\t";
            switch (m_table[i].estado) {
                case Estado::VAZIO: std::cout << "VAZIO\t"; break;
                case Estado::OCUPADO: std::cout << "OCUPADO\t"; break;
                case Estado::REMOVIDO: std::cout << "REMOVIDO\t"; break;
            }
            if (m_table[i].estado == Estado::OCUPADO) {
                std::cout << *(m_table[i].chave) << "\t" << *(m_table[i].valor) << "\t" << m_table[i].contador;
            }
            std::cout << "\n";
        }
    }

    // Getters para estatísticas
    size_t getComparacoesPrincipais() const { return m_comparacoes_principais; }
    size_t getRehashes() const { return m_rehashes; }

    // Para acessar todos pares para o DicionarioOpen
    // Retorna o par na posição i, ou lança exceção se vazio/removido
    std::pair<Key, Value> getPairAt(size_t i) const {
        if (i >= m_table_size) throw std::out_of_range("Índice fora do range");
        if (m_table[i].estado != Estado::OCUPADO || !m_table[i].chave || !m_table[i].valor) {
            throw std::out_of_range("Slot não está ocupado");
        }
        return {*(m_table[i].chave), *(m_table[i].valor)};
    }
};

#endif // HASH_ABERTO_HPP
