#include "Set.hpp"
#include <iostream>
#include <string>
#include <utility> // para std::pair
#include <chrono> // medir tempo

int main() {
    using Pair = std::pair<std::string, std::string>; // chave: fruta, valor: categoria ou qualquer coisa
    Set<Pair> fruitSet;

    auto inicio = std::chrono::high_resolution_clock::now();

    // Inserindo frutas com alguma "categoria"
    fruitSet.Insert({"apple", "common"});
    fruitSet.Insert({"banana", "common"});
    fruitSet.Insert({"orange", "citrus"});
    fruitSet.Insert({"grape", "vine"});
    fruitSet.Insert({"mango", "tropical"});
    fruitSet.Insert({"kiwi", "exotic"});
    fruitSet.Insert({"strawberry", "berry"});
    fruitSet.Insert({"blueberry", "berry"});
    fruitSet.Insert({"raspberry", "berry"});
    fruitSet.Insert({"pineapple", "tropical"});
    fruitSet.Insert({"watermelon", "melon"});
    fruitSet.Insert({"melon", "melon"});
    fruitSet.Insert({"peach", "stone"});
    fruitSet.Insert({"pear", "common"});
    fruitSet.Insert({"plum", "stone"});
    fruitSet.Insert({"cherry", "stone"});
    fruitSet.Insert({"lemon", "citrus"});
    fruitSet.Insert({"lime", "citrus"});
    fruitSet.Insert({"coconut", "tropical"});
    fruitSet.Insert({"fig", "exotic"});

    // Inserindo repetidos (com mesma chave/valor)
    for (int i = 0; i < 3; ++i) {
        fruitSet.Insert({"apple", "common"});
        fruitSet.Insert({"banana", "common"});
        fruitSet.Insert({"orange", "citrus"});
        fruitSet.Insert({"grape", "vine"});
        fruitSet.Insert({"mango", "tropical"});
    }

    for (int i = 0; i < 2; ++i) {
        fruitSet.Insert({"kiwi", "exotic"});
        fruitSet.Insert({"strawberry", "berry"});
        fruitSet.Insert({"blueberry", "berry"});
        fruitSet.Insert({"raspberry", "berry"});
        fruitSet.Insert({"pineapple", "tropical"});
    }

    fruitSet.Insert({"dragonfruit", "exotic"});
    fruitSet.Insert({"dragonfruit", "exotic"});
    fruitSet.Insert({"passionfruit", "exotic"});
    fruitSet.Insert({"passionfruit", "exotic"});
    fruitSet.Insert({"passionfruit", "exotic"});

    std::cout << "\n--- Elementos (em ordem) ---\n";
    fruitSet.PrintInOrder();
    std::cout << "\n\nTamanho do conjunto (elementos unicos): " << fruitSet.Size() << "\n";

    std::cout << "\n--- Visualizacao da arvore ---\n";
    fruitSet.bshow();

    std::cout << "\nContem 'banana'? " << (fruitSet.Contains({"banana", "common"}) ? "Sim" : "Nao") << "\n";

    std::cout << "Minimo: " << fruitSet.Minimum().first << ":" << fruitSet.Minimum().second << "\n";
    std::cout << "Maximo: " << fruitSet.Maximum().first << ":" << fruitSet.Maximum().second << "\n";

    try {
        auto suc = fruitSet.Successor({"grape", "vine"});
        std::cout << "Sucessor de grape: " << suc.first << ":" << suc.second << "\n";
    } catch (const std::exception& e) {
        std::cout << "Erro no sucessor: " << e.what() << "\n";
    }

    try {
        auto pred = fruitSet.Predecessor({"kiwi", "exotic"});
        std::cout << "Predecessor de kiwi: " << pred.first << ":" << pred.second << "\n";
    } catch (const std::exception& e) {
        std::cout << "Erro no predecessor: " << e.what() << "\n";
    }

    std::cout << "\n--- Remocao ---\n";
    fruitSet.Erase({"banana", "common"});
    fruitSet.Erase({"fig", "exotic"});
    fruitSet.Erase({"watermelon", "melon"});

    std::cout << "\nArvore apos remocoes:\n";
    fruitSet.bshow();

    fruitSet.Clear();
    std::cout << "\nConjunto limpo. Tamanho: " << fruitSet.Size() << ". Vazio? " << (fruitSet.Empty() ? "Sim" : "Nao") << "\n";

    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
    std::cout << "\nTempo total de execucao: " << duracao.count() << " ms\n";

    return 0;
}
