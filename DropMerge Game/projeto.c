                    //////////////////////////////////////////////////////////////////////
                    //                 Projeto de APC - DropMerge                       //
                    //                                                                  //
                    //               Nome: Ricardo Pedrosa Ramos Filho                  //
                    //                   Matrícula: 242032587                           //
                    //                                                                  //
                    //////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dimensões fixas da matriz
#define LINHAS 10
#define COLUNAS 5
#define PROXIMAS_LINHAS 2
#define PROXIMAS_COLUNAS 2
#define TAMANHO_MAX_NICKNAME 20
#define ARQUIVO_RANKING "ranking.bin"
#define ARQUIVO_NUMEROS "numeros.txt"

// struct para armazenar dados do jogador
struct dadosjogador {
    char nickname[TAMANHO_MAX_NICKNAME];
    int pontuacao;
};

//variaveis globais
int matriz_jogo[LINHAS][COLUNAS] = {0};       // Matriz do jogo
int proximos_numbers[PROXIMAS_LINHAS][PROXIMAS_COLUNAS];  // Próximos números
int altura[COLUNAS] = {0};                    // Altura das colunas
int pontuacao = 0;                            // Pontuação do jogador  
int pontuacaoMartelo = 0;                     // Pontuação base para conceder o martelo       
int numero;                                   // Variável p/ armazenar os proximos numeros
int ganhou;                                   // Variável p/ armazenar se o jogador ganhar ao atingir 60000 pontos ou mais
int martelos = 0;                             // Número de martelos
char nickname[TAMANHO_MAX_NICKNAME];          // Nickname do jogador
int processado[LINHAS][COLUNAS] = {0};        // Matriz de controle

// Declaração previa das funções
void processarMartelo(); 
void usarMartelo();
void reinicializarProcessado();
void mostrarBoasVindas();
void mostrarMenuPrincipal();
void jogar();
void mostrarInstrucoes();
void mostrarRanking();
void configurarJogo();
void zerarRanking();
void limparTela();
void mostrarGrid();
void gerarProximosNumeros(FILE *file);
int inserirNumero(int coluna, int numero);
void verificarVizinhos(int linha, int coluna);
void aplicarGravidade();
void salvarRanking();
void carregarRanking();
void ordenarRanking(struct dadosjogador ranking[], int tamanho);

// Função principal
int main() {
    mostrarBoasVindas();
    mostrarMenuPrincipal();
    return 0;
}

// Função para limpar a tela
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Função para reinicializar os numeros que ja foram processados na verificarVizinhos
void reinicializarProcessado() {
    memset(processado, 0, sizeof(processado));
}

// Função para mostrar a tela de boas-vindas e scannear o nick do jogador
void mostrarBoasVindas() {
    limparTela();
    printf("Bem-vindo ao Drop Merge de APC do semestre 24.2!\n");
    printf("Digite seu nickname: ");
    scanf("%19s", nickname);
    getchar();
}

// Função para mostrar o menu principal
void mostrarMenuPrincipal() {
    int opcao;
    do {
        limparTela();
        printf("Menu Principal\n");
        printf("1. Jogar\n2. Instrucoes\n3. Configuracoes\n4. Ranking\n5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1: jogar(); break;
            case 2: mostrarInstrucoes(); break;
            case 3: configurarJogo(); break;
            case 4: mostrarRanking(); break;
            case 5: printf("Saindo...\n"); exit(0);
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 5);
}

// Função do gameLoop
void jogar() {
    FILE *file = fopen(ARQUIVO_NUMEROS, "r");
    if (!file) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    int numeros[1000];
    int total_numeros = 0;
    while (fscanf(file, "%d", &numeros[total_numeros]) == 1 && total_numeros < 1000) {
        total_numeros++;
    }
    fclose(file);

    if (total_numeros == 0) {
        printf("Arquivo vazio!\n");
        return;
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            matriz_jogo[i][j] = 0;
        }
    }
    for (int j = 0; j < COLUNAS; j++) {
        altura[j] = 0;
    }

    pontuacao = 0;
    martelos = 0;

    int indice_numero = 0;
    while (indice_numero < total_numeros) {
        mostrarGrid();
        int numero_atual = numeros[indice_numero];
        proximos_numbers[0][0] = numero_atual;
        proximos_numbers[0][1] = (indice_numero + 1 < total_numeros) ? numeros[indice_numero + 1] : 0;

        int coluna;
        do {
            printf("Escolha a coluna (1-%d) para colocar o numero %d: ", COLUNAS, numero_atual);
            scanf("%d", &coluna);
            coluna--;
        } while (coluna < 0 || coluna >= COLUNAS || altura[coluna] >= LINHAS);

        inserirNumero(coluna, numero_atual);
        indice_numero++;

        int perdeu = 1;
        for (int i = 0; i < COLUNAS; i++) {
            if (altura[i] < LINHAS) {
                perdeu = 0;
                break;
            }
        }

        if (perdeu) {
            while (martelos > 0) {
                printf("Usar martelo? (S/N): ");
                char resposta;
                scanf(" %c", &resposta);
                if (resposta == 'S' || resposta == 's') {
                    int linha, col;
                    printf("Linha e Coluna: ");
                    scanf("%d %d", &linha, &col);
                    linha--; col--;
                    if (linha >= 0 && linha < LINHAS && col >= 0 && col < COLUNAS && matriz_jogo[linha][col] != 0) {
                        matriz_jogo[linha][col] = 0;
                        martelos--;
                        aplicarGravidade();
                        
                        perdeu = 0;
                        for (int i = 0; i < COLUNAS; i++) {
                            if (altura[i] >= LINHAS) {
                                perdeu = 1;
                                break;
                            }
                        }
                        if (!perdeu) break;
                    } else {
                        printf("Linha e coluna invalidas!\n");
                    }
                } else {
                    break;
                }
            }
            if (perdeu) {
                printf("Game Over! Pontuacao: %d\n", pontuacao);
                salvarRanking();
                getchar();
                getchar();
                return;
            }
        }
        if (pontuacao >= 60000){
            ganhou = 1;
            break;
        }
    }
    printf("VOCE GANHOU!! TENHO DUAS PALAVRAS PARA VOCE: PARA, BENS!\n");
    getchar();
    getchar();
}

// Funcao para mostrar o grid do jogo
void mostrarGrid() {
    limparTela();
    printf("Score: %d | Martelos: %d\n", pontuacao, martelos);
    
    // Próximos números
    printf("           +----+  +----+\n");
    printf("           |%4d|  |%4d|\n", proximos_numbers[0][0], proximos_numbers[0][1]);
    printf("           +----+  +----+\n");

    // exibe a matriz da linha 9 ate a linha 0
    for (int i = LINHAS - 1; i >= 0; i--) {
        printf("  +----+----+----+----+----+\n");
        printf("  |");
        for (int j = 0; j < COLUNAS; j++) {
            if (matriz_jogo[i][j] != 0) printf("%4d|", matriz_jogo[i][j]);
            else printf("    |");
        }
        printf("\n");
    }
    printf("  +----+----+----+----+----+\n");
    printf("  |  1 |  2 |  3 |  4 |  5 |\n");
    printf("  +----+----+----+----+----+\n\n");
}


void gerarProximosNumeros(FILE *file) {
    for(int i = 0; i < PROXIMAS_LINHAS; i++)
        for(int j = 0; j < PROXIMAS_COLUNAS; j++)
            fscanf(file, "%d", &proximos_numbers[i][j]);
}

int inserirNumero(int coluna, int numero) {
    if(altura[coluna] < LINHAS) {
        matriz_jogo[altura[coluna]][coluna] = numero;
        altura[coluna]++;
        aplicarGravidade();
        verificarVizinhos(altura[coluna]-1, coluna);
        return 1;
    }
    return 0;
}

void aplicarGravidade() {
    for(int j = 0; j < COLUNAS; j++) {
        int linha_destino = 0;
        for(int i = 0; i < LINHAS; i++) {
            if(matriz_jogo[i][j]) {
                matriz_jogo[linha_destino][j] = matriz_jogo[i][j];
                if(linha_destino != i) matriz_jogo[i][j] = 0;
                linha_destino++;
            }
        }
        altura[j] = linha_destino;
    }
}

void verificarVizinhos(int linha, int coluna) {
    int houve_fusao;
    do {
        houve_fusao = 0;
        reinicializarProcessado();

        for(int i = 0; i < LINHAS; i++) {
            for(int j = 0; j < COLUNAS; j++) {
                if(matriz_jogo[i][j] == 0 || processado[i][j]) continue;

                int valor = matriz_jogo[i][j];
                int fusao_valor = valor;
                int total_vizinhos = 0;

                // Busca em largura para encontrar vizinhos conectados
                int fila[LINHAS * COLUNAS][2];
                int inicio = 0; 
                int fim = 0;
                
                // Adiciona a célula atual à fila
                fila[fim][0] = i;
                fila[fim][1] = j;
                fim++;
                processado[i][j] = 1;

                while(inicio < fim) {
                    int x = fila[inicio][0];
                    int y = fila[inicio][1];
                    inicio++;
                    total_vizinhos++;

                    // Verifica os 4 vizinhos
                    int dx[] = {-1, 1, 0, 0};
                    int dy[] = {0, 0, -1, 1};
                    
                    for(int k = 0; k < 4; k++) {
                        int nx = x + dx[k];
                        int ny = y + dy[k];

                        // Verifica se está dentro dos limites
                        if(nx >= 0 && nx < LINHAS && ny >= 0 && ny < COLUNAS) {
                            // Verifica se é igual e não foi processado
                            if(matriz_jogo[nx][ny] == valor && !processado[nx][ny]) {
                                fila[fim][0] = nx;
                                fila[fim][1] = ny;
                                fim++;
                                processado[nx][ny] = 1;
                            }
                        }
                    }
                }

                // Se houver pelo menos 2 células conectadas
                if(total_vizinhos >= 2) {
                    // Remove todas as células do grupo
                    for(int k = 0; k < fim; k++) {
                        int x = fila[k][0];
                        int y = fila[k][1];
                        matriz_jogo[x][y] = 0;
                    }

                    // Atualiza a célula central com o novo valor
                    matriz_jogo[i][j] = valor * total_vizinhos;
                    pontuacao += matriz_jogo[i][j];
                    pontuacaoMartelo += matriz_jogo[i][j];
                    houve_fusao = 1;
                    if(pontuacaoMartelo >= 10000){
                        int soma = pontuacaoMartelo/10000;
                        printf("%d\n", soma);
                        getchar();
                        martelos += soma;
                        pontuacaoMartelo -= soma*10000;
                    }
                }
            }
        }

    aplicarGravidade(); // Reorganiza os números após a fusão

    } while(houve_fusao);
}

void salvarRanking() {
    FILE *file = fopen(ARQUIVO_RANKING, "ab");
    if(file) {
        struct dadosjogador jogador;
        strncpy(jogador.nickname, nickname, TAMANHO_MAX_NICKNAME);
        jogador.pontuacao = pontuacao;
        fwrite(&jogador, sizeof(struct dadosjogador), 1, file);
        fclose(file);
    }
}

void mostrarRanking() {
    FILE *file = fopen(ARQUIVO_RANKING, "rb");
    if(file) {
        struct dadosjogador ranking[100];
        int tamanho = 0;

        while(fread(&ranking[tamanho], sizeof(struct dadosjogador), 1, file)) {
            tamanho++;
        }

        ordenarRanking(ranking, tamanho);

        printf("=== Ranking ===\n");
        for(int i = 0; i < tamanho && i < 10; i++) {
            printf("%2d. %-20s %5d pontos\n", i+1, ranking[i].nickname, ranking[i].pontuacao);
        }
        printf("Pressione enter para voltar para o menu.");
        getchar();
        fclose(file);
    }
}

void ordenarRanking(struct dadosjogador ranking[], int tamanho) {
    for(int i = 0; i < tamanho-1; i++) {
        for(int j = 0; j < tamanho-i-1; j++) {
            if(ranking[j].pontuacao < ranking[j+1].pontuacao) {
                struct dadosjogador temp = ranking[j];
                ranking[j] = ranking[j+1];
                ranking[j+1] = temp;
            }
        }
    }
}

void mostrarInstrucoes() {
    limparTela();
    printf("=== Instrucoes ===\n");
    printf("1. Insira numeros nas colunas para combina-los.\n");
    printf("2. Numeros iguais adjacentes se fundem em um novo numero.\n");
    printf("3. Mantenha o grid organizado para evitar transbordar.\n");
    printf("Pressione Enter para voltar...");
    getchar();
}

void configurarJogo() {
    int opcao;
    do {
        limparTela();
        printf("=== Configuracoes ===\n");
        printf("1. Zerar Ranking\n");
        printf("2. Alterar Dimensoes do Grid\n");
        printf("3. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1: zerarRanking(); break;
            case 2: printf("Funcionalidade em desenvolvimento.\n"); break;
            case 3: return;
            default: printf("Opcao invalida!\n");
        }
        getchar();
    } while(opcao != 3);
}

void zerarRanking() {
    printf("Tem certeza que deseja apagar o ranking? (S/N): ");
    char confirmacao = getchar();
    if(confirmacao == 'S' || confirmacao == 's') {
        FILE *file = fopen(ARQUIVO_RANKING, "wb");
        if(file) {
            fclose(file);
            printf("Ranking zerado com sucesso!\n");
        }
        else {
            printf("Erro ao acessar o arquivo!\n");
        }
    }
    else {
        printf("Operacao cancelada.\n");
    }
}