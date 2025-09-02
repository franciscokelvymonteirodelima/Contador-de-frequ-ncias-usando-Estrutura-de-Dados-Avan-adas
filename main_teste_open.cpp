#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <unicode/unistr.h>
#include <unicode/uchar.h>

#include "dicionarioopen.hpp"

// Função para limpar e converter palavra para minúsculo (Unicode-safe)
std::string limpar_e_minusculo(const std::string& palavra) {
    icu::UnicodeString unicodePalavra = icu::UnicodeString::fromUTF8(palavra);
    icu::UnicodeString unicodeLimpa;

    for (int32_t i = 0; i < unicodePalavra.length(); ) {
        UChar32 c = unicodePalavra.char32At(i);
        if (u_isalpha(c)) {
            unicodeLimpa.append(c);
        }
        i += U16_LENGTH(c);
    }

    unicodeLimpa.toLower();
    std::string resultado;
    unicodeLimpa.toUTF8String(resultado);
    return resultado;
}

// Função para ler o arquivo de entrada e preencher o dicionário
template <typename Dicionario>
void ler_arquivo_e_inserir(const std::string& caminho, Dicionario& dicionario) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << caminho << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        std::string palavra;
        while (iss >> palavra) {
            std::string limpa = limpar_e_minusculo(palavra);
            if (!limpa.empty()) {
                if (dicionario.contains(limpa)) {
                    int atual = dicionario.at(limpa);
                    dicionario.remover(limpa);
                    dicionario.inserir(limpa, atual + 1);
                } else {
                    dicionario.inserir(limpa, 1);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Uso: " << argv[0] << " <arquivo_entrada.txt> <arquivo_saida.txt>\n";
        return 1;
    }

    std::string caminho_entrada = argv[1];
    std::string caminho_saida = argv[2];

    DicionarioOpen<std::string, int> dicionario;

    // Cronômetro
    auto start = std::chrono::high_resolution_clock::now();
    ler_arquivo_e_inserir(caminho_entrada, dicionario);
    auto end = std::chrono::high_resolution_clock::now();
    long long duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::ofstream saida(caminho_saida);
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída.\n";
        return 1;
    }

    // Estatísticas
    saida << "A ESTRUTURA TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "Tempo de execução: " << duracao << " nanosegundos\n";
    saida << "Número de comparações de chaves: " << dicionario.getComparacoesPrincipais() << "\n";
    saida << "Número de colisões (rehash): " << dicionario.getContadorRehash() << "\n\n";

    // Tabela de palavras ordenadas
    saida << std::left << std::setw(25) << "Palavra" << "Frequência\n";
    saida << "--------------------------------------\n";

    std::vector<std::pair<std::string, int>> pares;
    dicionario.getAllPairs(pares);
    std::sort(pares.begin(), pares.end());

    for (const auto& p : pares) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    std::cout << "Arquivo '" << caminho_saida << "' gerado com sucesso!\n";
    return 0;
}
