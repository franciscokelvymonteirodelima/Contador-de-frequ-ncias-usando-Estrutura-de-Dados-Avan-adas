#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip> // std::setw
#include <vector>  // Para std::vector
#include <algorithm> // Para std::sort

// Inclua as bibliotecas Unicode se você as tiver configuradas.
#include <unicode/unistr.h>
#include <unicode/uchar.h>

// AGORA INCLUIMOS O CABEÇALHO DO SEU DICIONÁRIO RUBRO-NEGRA
#include "dicionariorb.hpp"

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
                // :: CORREÇÃO :: Usar dicionario.count() para obter a frequência atual
                // e dicionario.add() para inserir/atualizar.
                // O método 'add' do DicionarioRb e da rbtree subjacente já lida
                // com a lógica de incrementar a contagem se a palavra já existe.
                int freq_atual = dicionario.count(limpa); // Obtém a frequência atual (0 se não existe)
                dicionario.add(limpa, freq_atual + 1); // Adiciona/atualiza com a nova frequência
            }
        }
    }
}

int main() {
    // Verifique o caminho do arquivo. Use um caminho absoluto ou relativo correto.
    std::string caminho = "/home/kelvy_lima/Documentos/TRABALHO FINAL DO ATILIO_alterado/exemplo.txt";

    // INSTANCIANDO O DICIONÁRIO RUBRO-NEGRA
    DicionarioRb<std::string, int> dicionario_rb;

    // Resetar contadores antes do teste principal
    dicionario_rb.resetComparacoes();
    dicionario_rb.resetRotacoes(); // :: CORREÇÃO :: Chamando getRotacoes()

    auto start = std::chrono::high_resolution_clock::now();
    ler_arquivo_e_inserir(caminho, dicionario_rb);
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9; // Converter para segundos

    // Criar arquivo de saída específico para a árvore rubro-negra
    std::ofstream saida("saida_rb.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida_rb.txt" << std::endl;
        return 1;
    }

    saida << "A ESTRUTURA TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    saida << "número de comparações de chaves: " << dicionario_rb.getComparacoesPrincipais() << "\n";
    saida << "número de rotações: " << dicionario_rb.getRotacoes() << "\n\n"; // :: CORREÇÃO :: Chamando getRotacoes()

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    // Obter todos os pares de palavras e frequências
    std::vector<std::pair<std::string, int>> vetor_palavras_frequencias;
    dicionario_rb.getAllPairs(vetor_palavras_frequencias);

    // As árvores rubro-negras já garantem a ordem alfabética ao coletar.
    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    std::cout << "Arquivo 'saida_rb.txt' gerado com sucesso!\n";

    return 0;
}