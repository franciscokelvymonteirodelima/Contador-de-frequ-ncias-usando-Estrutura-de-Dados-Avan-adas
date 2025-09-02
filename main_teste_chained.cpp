#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip> // std::setw
#include <vector>  // Para std::vector
#include <algorithm> // Para std::sort

// Inclua as bibliotecas Unicode se você as tiver configuradas.
// Se não tiver, pode comentar estas linhas e usar uma versão mais simples de limpeza de string.
#include <unicode/unistr.h>
#include <unicode/uchar.h>

// AGORA INCLUIMOS O CABEÇALHO DO SEU DICIONÁRIO ENCADEADO
#include "dicionariochained.hpp" // Nome do seu arquivo de dicionário encadeado

// Função que limpa e retorna palavra só com letras, e em minúsculo Unicode
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

// Função para ler arquivo e inserir no dicionário, incrementando contagem
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
                    int atual = dicionario.count(limpa);
                    dicionario.add(limpa, atual + 1);
                } else {
                    dicionario.add(limpa, 1);
                }
            }
        }
    }
}

int main() {
    // Verifique o caminho do arquivo. Use um caminho absoluto ou relativo correto.
    std::string caminho = "/home/kelvy_lima/Documentos/TRABALHO FINAL DO ATILIO_alterado/exemplo.txt";

    // INSTANCIANDO O DICIONÁRIO ENCADEADO
    DicionarioChained<std::string, int> dicionario_chained;

    // Resetar contadores antes do teste principal para garantir que só o teste seja medido
    dicionario_chained.resetComparacoes();
    dicionario_chained.resetRehash(); // Resetar o contador de rehash para a tabela hash

    auto start = std::chrono::high_resolution_clock::now();
    ler_arquivo_e_inserir(caminho, dicionario_chained);
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9; // Converter para segundos

    // Criar arquivo de saída específico para a tabela hash encadeada
    std::ofstream saida("saida_chained.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida_chained.txt" << std::endl;
        return 1;
    }

    saida << "A ESTRUTURA TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    saida << "número de comparações de chaves: " << dicionario_chained.getComparacoesPrincipal() << "\n";
    saida << "número de colisões (rehash): " << dicionario_chained.getContadorRehash() << "\n\n";

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    // Obter todos os pares de palavras e frequências
    std::vector<std::pair<std::string, int>> vetor_palavras_frequencias;
    dicionario_chained.getAllPairs(vetor_palavras_frequencias);

    // Opcional: Ordenar o vetor para ter a saída em ordem alfabética (tabelas hash não garantem ordem)
    std::sort(vetor_palavras_frequencias.begin(), vetor_palavras_frequencias.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.first < b.first;
              });

    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    std::cout << "Arquivo 'saida_chained.txt' gerado com sucesso!\n";

    return 0;
}