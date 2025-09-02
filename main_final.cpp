#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip> 
#include <vector>  
#include <algorithm> 
#include <functional> 


#include <unicode/unistr.h>
#include <unicode/uchar.h>


#include "dicionarioavl.hpp"
#include "dicionariochained.hpp" 
#include "dicionarioopen.hpp"       
#include "dicionariorb.hpp"     

// Funções Auxiliares Comuns

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

// Funções de Processamento Específicas para Cada Estrutura

// Processa arquivo usando DicionarioAvl
void processar_com_avl(const std::string& caminho_arquivo) {
    DicionarioAvl<std::string, int> dicionario;

    // Resetar contadores (assumindo que DicionarioAvl tem resetComparacoes e resetRotacoes)
    dicionario.resetComparacoes();
    dicionario.resetRotacoes();

    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream arquivo(caminho_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << caminho_arquivo << std::endl;
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
    arquivo.close();
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9;

    std::ofstream saida("saida_avl.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida_avl.txt" << std::endl;
        return;
    }

    saida << "A ESTRUTURA AVL TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    saida << "número de comparações de chaves: " << dicionario.getComparacoesPrincipais() << "\n";
    saida << "número de rotações: " << dicionario.getRotacoes() << "\n\n";

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    auto vetor_palavras_frequencias = dicionario.getAllOrdered(); // AVL já retorna ordenado
    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    saida.close();
    std::cout << "Arquivo 'saida_avl.txt' gerado com sucesso!\n";
}

// Processa arquivo usando DicionarioChained (Hash Encadeada)
void processar_com_chained(const std::string& caminho_arquivo) {
    DicionarioChained<std::string, int> dicionario;

    // Resetar contadores (assumindo que DicionarioChained tem resetComparacoes e resetRehash)
    dicionario.resetComparacoes();
    dicionario.resetRehash();

    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream arquivo(caminho_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << caminho_arquivo << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        std::string palavra;
        while (iss >> palavra) {
            std::string limpa = limpar_e_minusculo(palavra);
            if (!limpa.empty()) {
                // Para DicionarioChained, 'add' já atualiza o valor se a chave existe
                int atual = dicionario.count(limpa); // Se não existe, retorna 0 (ValueType default)
                dicionario.add(limpa, atual + 1);    // Adiciona ou atualiza
            }
        }
    }
    arquivo.close();
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9;

    std::ofstream saida("saida_chained.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida_chained.txt" << std::endl;
        return;
    }

    saida << "A ESTRUTURA HASH ENCADEADA TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    // Corrigido para getComparacoesPrincipal() conforme seu código
    saida << "número de comparações de chaves: " << dicionario.getComparacoesPrincipal() << "\n";
    // Corrigido para getContadorRehash() conforme seu código
    saida << "número de rehashes: " << dicionario.getContadorRehash() << "\n\n";

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    std::vector<std::pair<std::string, int>> vetor_palavras_frequencias;
    dicionario.getAllPairs(vetor_palavras_frequencias); // Coleta todos os pares

    // Opcional: Ordenar o vetor para ter a saída em ordem alfabética (Hash Tables não garantem ordem)
    std::sort(vetor_palavras_frequencias.begin(), vetor_palavras_frequencias.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.first < b.first;
              });

    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    saida.close();
    std::cout << "Arquivo 'saida_chained.txt' gerado com sucesso!\n";
}

// Processa arquivo usando HashAberto (Endereçamento Aberto)
void processar_com_open(const std::string& caminho_arquivo) {
    HashAberto<std::string, int> dicionario;

    // Resetar contadores (assumindo que HashAberto tem resetComparacoes e resetRehash)
    //dicionario.resetComparacoes();
    //dicionario.resetRehash(); // No seu HashAberto, isso é m_rehashes

    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream arquivo(caminho_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << caminho_arquivo << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        std::string palavra;
        while (iss >> palavra) {
            std::string limpa = limpar_e_minusculo(palavra);
            if (!limpa.empty()) {
                
                try {
                    int atual = dicionario.at(limpa); 
                    dicionario.insert(limpa, atual + 1); // Atualiza com novo valor
                } catch (const std::out_of_range& e) {
                    dicionario.insert(limpa, 1); // Insere pela primeira vez
                }
            }
        }
    }
    arquivo.close();
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9;

    std::ofstream saida("saida_open.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida_open.txt" << std::endl;
        return;
    }

    saida << "A ESTRUTURA HASH ABERTO TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    saida << "número de comparações de chaves: " << dicionario.getComparacoesPrincipais() << "\n";
    saida << "número de rehashes: " << dicionario.getRehashes() << "\n\n";

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    std::vector<std::pair<std::string, int>> vetor_palavras_frequencias;
    // Iterar sobre a HashAberto para coletar os pares.
    for (size_t i = 0; i < dicionario.bucket_count(); ++i) {
        try {
            vetor_palavras_frequencias.push_back(dicionario.getPairAt(i));
        } catch (const std::out_of_range& e) {
            // Slot VAZIO ou REMOVIDO, ignora.
        }
    }

    // ordenar o vetor para ter a saída em ordem alfabética
    std::sort(vetor_palavras_frequencias.begin(), vetor_palavras_frequencias.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.first < b.first;
              });

    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    saida.close();
    std::cout << "Arquivo 'saida_open.txt' gerado com sucesso!\n";
}

// Processa arquivo usando DicionarioRb (Árvore Rubro-Negra)
void processar_com_rb(const std::string& caminho_arquivo) {
    DicionarioRb<std::string, int> dicionario;

    // Resetar contadores (assumindo que DicionarioRb tem resetComparacoes e resetRotacoes)
    dicionario.resetComparacoes();
    dicionario.resetRotacoes();

    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream arquivo(caminho_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << caminho_arquivo << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        std::string palavra;
        while (iss >> palavra) {
            std::string limpa = limpar_e_minusculo(palavra);
            if (!limpa.empty()) {
                // Lógica de frequência para DicionarioRb
                int freq_atual = dicionario.count(limpa); // Obtém a frequência atual (0 se não existe)
                dicionario.add(limpa, freq_atual + 1);    // Adiciona/atualiza com a nova frequência
            }
        }
    }
    arquivo.close();
    auto end = std::chrono::high_resolution_clock::now();

    long long duracao_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duracao_s = static_cast<double>(duracao_ns) / 1e9;

    std::ofstream saida("saida_rb.txt");
    if (!saida.is_open()) {
        std::cerr << "Erro ao criar arquivo de saída: saida_rb.txt" << std::endl;
        return;
    }

    saida << "A ESTRUTURA RUBRO-NEGRA TEM AS SEGUINTES INFORMAÇÕES: \n";
    saida << "tempo de montagem: " << duracao_ns << " nanosegundos (" << std::fixed << std::setprecision(9) << duracao_s << " segundos)\n";
    saida << "número de comparações de chaves: " << dicionario.getComparacoesPrincipais() << "\n";
    saida << "número de rotações: " << dicionario.getRotacoes() << "\n\n";

    saida << std::left << std::setw(25) << "Palavra" << "Frequencia\n";
    saida << "--------------------------------------\n";

    std::vector<std::pair<std::string, int>> vetor_palavras_frequencias;
    dicionario.getAllPairs(vetor_palavras_frequencias); // Coleta todos os pares (já virá ordenada da RB)

    for (const auto& p : vetor_palavras_frequencias) {
        saida << std::left << std::setw(25) << p.first << p.second << "\n";
    }

    saida.close();
    std::cout << "Arquivo 'saida_rb.txt' gerado com sucesso!\n";
}

// Main Principal do Programa (Ponto de Entrada)

int main(int argc, char* argv[]) {
    // 1. Validação dos Argumentos da Linha de Comando
    // Esperamos: ./freq <estrutura> <arquivo_entrada>
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <estrutura> <arquivo_entrada>\n";
        std::cerr << "Estruturas suportadas: 'avl', 'chained', 'open', 'rb'\n";
        std::cerr << "Exemplo: " << argv[0] << " avl texto.txt\n";
        return 1; // Retorna código de erro
    }

    std::string estrutura_arg = argv[1];    // "avl", "chained", "open", "rb"
    std::string caminho_arquivo_arg = argv[2]; // "texto.txt"

    // Despacho para a Função de Processamento Correta Baseada na Estrutura
    if (estrutura_arg == "avl") {
        processar_com_avl(caminho_arquivo_arg);
    } else if (estrutura_arg == "chained") {
        processar_com_chained(caminho_arquivo_arg);
    } else if (estrutura_arg == "open") {
        processar_com_open(caminho_arquivo_arg);
    } else if (estrutura_arg == "rb") {
        processar_com_rb(caminho_arquivo_arg);
    } else {
        std::cerr << "Erro: Estrutura '" << estrutura_arg << "' não suportada.\n";
        std::cerr << "Estruturas suportadas: 'avl', 'chained', 'open', 'rb'\n";
        return 1;
    }

    return 0;
}