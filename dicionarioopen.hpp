#ifndef DICIONARIO_OPEN_HPP 
#define DICIONARIO_OPEN_HPP

#include "hash_aberto.hpp" 

// Esta é a classe DicionarioOpen, ela "empacota" sua HashAberto para ser usada como um dicionário
template<typename Key, typename Value> // Ela funciona com qualquer tipo de Chave (Key) e Valor (Value)
class DicionarioOpen {
private:
    // Aqui criamos uma instância da sua HashAberto, é onde os dados serão realmente guardados
    HashAberto<Key, Value> tabela;

public:
    // Adiciona uma chave e um valor ao dicionário (ou atualiza se a chave já existe)
    void inserir(const Key& k, const Value& v) {
        tabela.insert(k, v); // Chama a função 'insert' da HashAberto
    }

    // Remove uma chave do dicionário
    void remover(const Key& k) {
        tabela.remove(k); // Chama a função 'remove' da HashAberto
    }

    // Verifica se uma chave existe no dicionário
    bool pertence(const Key& k) const {
        return tabela.contains(k); // Chama a função 'contains' da HashAberto
    }

    // Outro nome para 'pertence', para ser compatível com outras partes do seu código
    bool contains(const Key& k) const {
        return pertence(k);
    }

    // Tenta encontrar o valor de uma chave. Retorna o valor ou 'nada' se a chave não existir.
    std::optional<Value> valor(const Key& k) const {
        try {
            return tabela.at(k); // Tenta pegar o valor usando 'at'
        } catch (std::out_of_range&) { // Se 'at' disser que não encontrou (erro), retorna 'nada'
            return std::nullopt;
        }
    }

    // Pega o valor de uma chave e permite que você o modifique
    Value& at(const Key& k) {
        return tabela.at(k);
    }

    // Pega o valor de uma chave para leitura (não permite modificar)
    const Value& at(const Key& k) const {
        return tabela.at(k);
    }

    // Mostra o conteúdo da tabela hash (útil para ver o que está dentro)
    void show() const {
        tabela.show();
    }

    // Retorna quantas comparações de chaves foram feitas (para medir desempenho)
    size_t getComparacoesPrincipais() const {
        return tabela.getComparacoesPrincipais();
    }

    // Retorna quantas vezes a tabela hash precisou ser redimensionada (para medir desempenho)
    size_t getContadorRehash() const {
        return tabela.getRehashes(); // Note que o nome na HashAberto é 'getRehashes'
    }

    // Pega todos os pares de chave-valor e coloca em um vetor (lista)
    void getAllPairs(std::vector<std::pair<Key, Value>>& pares) const {
        pares.clear(); // Limpa o vetor antes de preencher
        size_t buckets = tabela.bucket_count(); // Pega o número total de 'espaços' na tabela hash
        for (size_t i = 0; i < buckets; ++i) { // Percorre todos os 'espaços'
            try {
                // Tenta pegar o par do espaço. Pode dar erro se o espaço estiver vazio ou removido.
                pares.push_back(tabela.getPairAt(i));
            } catch (...) {
                // Se der erro, significa que o espaço está vazio ou removido, então ignoramos.
            }
        }
    }
};

#endif // DICIONARIO_OPEN_HPP