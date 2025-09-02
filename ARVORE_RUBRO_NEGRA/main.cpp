#include <iostream>
#include <string>
#include <utility> // para std::pair
#include "RBTree.hpp"
#include <chrono>

int main() {
    // Árvores agora usam par (chave, valor): chave = string (fruta), valor = int (id ou dado qualquer)
    rbtree<std::string, int> arvoreStr;

    auto inicio = std::chrono::high_resolution_clock::now();

    // Inserindo pares (chave, valor). O valor aqui pode ser qualquer número inteiro representativo
    arvoreStr.insert({"banana", 10});
    arvoreStr.insert({"abacaxi", 5});
    arvoreStr.insert({"laranja", 7});
    arvoreStr.insert({"banana", 10});
    arvoreStr.insert({"banana", 10});
    arvoreStr.insert({"laranja", 7});
    arvoreStr.insert({"uva", 12});
    arvoreStr.insert({"uva", 12});
    arvoreStr.insert({"uva", 12});
    arvoreStr.insert({"uva", 12});
    arvoreStr.insert({"manga", 15});
    arvoreStr.insert({"caju", 9});
    arvoreStr.insert({"caju", 9});
    arvoreStr.insert({"caju", 9});
    arvoreStr.insert({"caju", 9});

    std::cout << "Árvore após inserções (com repetições):\n";
    arvoreStr.show();

    std::cout << "\nRemovendo 'banana' e 'caju'...\n";
    arvoreStr.remove("banana");
    arvoreStr.remove("caju");
    arvoreStr.show();

    std::cout << "\nInserindo 'banana' novamente...\n";
    arvoreStr.insert({"banana", 10});
    arvoreStr.show();

    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
    std::cout << "Tempo de execução: " << duracao.count() << " ms\n";

    std::cout << "\n--- Todos os testes concluídos! ---\n";
    return 0;
}
