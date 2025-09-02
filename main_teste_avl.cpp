#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip> // std::setw
#include <vector>  // Para std::vector

// Inclua as bibliotecas Unicode se você as tiver configuradas.
// Se não tiver, pode comentar estas linhas e usar uma versão mais simples de limpeza de string.
#include <unicode/unistr.h>
#include <unicode/uchar.h>

#include "dicionarioavl.hpp" // Inclui o cabeçalho do seu dicionário AVL

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
                // A lógica de contagem de frequência é tratada aqui.
                // O método 'add' do DicionarioAvl (que usa Insert do Set)
                // agora atualiza o valor se a chave já existe.
                if (dicionario.contains(limpa)) { // CHAMA contains APENAS com a chave
                    int atual = dicionario.count(limpa); // CHAMA count APENAS com a chave
                    dicionario.add(limpa, atual + 1);    // add com a chave e a nova frequência
                } else {
                    dicionario.add(limpa, 1); // Primeira ocorrência, frequência 1
                }
            }
        }
    }
}

int main() {
    // Verifique o caminho do arquivo. Use um caminho absoluto ou relativo correto.
    std::string caminho = "/home/kelvy_lima/Documentos/TRABALHO FINAL DO ATILIO_alterado/exemplo.txt";

    DicionarioAvl<std::string, int> dicionario;

    // Resetar contadores antes do teste principal para garantir que só o teste seja medido
    dicionario.resetComparacoes();
    dicionario.resetRotacoes();

    auto start = std::chrono::high_resolution_clock::now();
    ler_arquivo_e_inserir(caminho, dicionario);
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9; // Converter para segundos

    // Criar arquivo de saída
    std::ofstream saida("saida.txt"); // Nome do arquivo de saída
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida.txt" << std::endl;
        return 1; // Retorna erro
    }

    saida << "A ESTRUTURA TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    saida << "número de comparações de chaves: " << dicionario.getComparacoesPrincipais() << "\n";
    saida << "número de rotações: " << dicionario.getRotacoes() << "\n\n";
    // Removida a linha de "colisões", pois não se aplica a AVL.

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    // Obter todas as palavras e frequências ordenadas
    auto vetor_palavras_frequencias = dicionario.getAllOrdered();
    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    std::cout << "Arquivo 'saida.txt' gerado com sucesso!\n";

    return 0;
}