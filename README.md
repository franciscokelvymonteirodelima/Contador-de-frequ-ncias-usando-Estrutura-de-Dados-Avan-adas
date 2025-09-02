Guia Rápido: Compilando e Executando seu Programa de Frequência de Palavras

Este guia mostra como compilar seu programa main_final e executá-lo para analisar a frequência de palavras usando diferentes estruturas de dados.

Organização dos Arquivos

Para que tudo funcione sem problemas, seu arquivo de texto de exemplo (exemplo.txt) e o programa executável (main_final) devem estar na mesma pasta, que é o diretório output/.

Organize seus arquivos da seguinte forma:

trabalho_final_EDA/
├── main_final.cpp          (seu código principal)
├── dicionarioavl.hpp
├── dicionariochained.hpp
├── hashaberto.hpp
├── dicionariorb.hpp
└── output/
    ├── exemplo.txt         (coloque seu arquivo de texto AQUI)
    └── (aqui será criado o 'main_final' após a compilação)

Passo importante: Se seu exemplo.txt estiver fora da pasta output/, mova-o para dentro dela antes de executar o programa pra facilitar.

2. Compilação do Programa 

Para compilar, abra seu terminal e execute o seguinte comando a partir do diretório principal do seu projeto (/home/kelvy_lima/Documentos/trabalho_final_EDA/):

cd /home/kelvy_lima/Documentos/trabalho_final_EDA/

cd /home/kelvy_lima/Documentos/trabalho_final_EDA/

g++ -Wall -Wextra -g3 \
    main_final.cpp \
    -o ./output/main_final \
    -licuuc -licui18n

ou

g++ -Wall -Wextra -g3 \
    /home/kelvy_lima/Documentos/trabalho_final_EDA/main_final.cpp \
    -o /home/kelvy_lima/Documentos/trabalho_final_EDA/output/main_final \
    -licuuc -licui18n

Este comando cria o arquivo executável main_final e o coloca dentro da pasta output/. Se não aparecerem erros, a compilação foi um sucesso!

3. Execução do Programa 

Depois de compilar, você precisa ir para a pasta output/ para executar o programa:

Navegue até a pasta output/:

cd /home/kelvy_lima/Documentos/trabalho_final_EDA/output/

Execute o programa:
Agora que exemplo.txt está na pasta output/ junto com main_final, você pode simplesmente referenciá-lo pelo nome.
Você precisa informar qual estrutura de dados usar (avl, chained, open ou rb) e o nome do arquivo de texto:

Para Árvore AVL:

./main_final avl exemplo.txt

Para Hash Encadeada:

./main_final chained exemplo.txt

Para Hash com Endereçamento Aberto:

./main_final open exemplo.txt

Para Árvore Rubro-Negra:

./main_final rb exemplo.txt

Após a execução, um arquivo de saída (saida_avl.txt, saida_chained.txt, etc.) será gerado na mesma pasta output/ com os resultados da análise.

Caso ocorra algum problema verifique novamente os primeiros topicos e tente novamente, lembrando que o diretorio que esta dentro do codigo pode ter resalvas pois no meu computador ele so executa de eu colocar todo o diretorio, caso tenha algum outro erro pode me contactar pelo email: kelvyemergencial@gmail.com.
