// DESAFIO NÍVEL MESTRE
// JOGO WAR EM C COM SISTEMA DE MISSÕES

// ---------- BIBLIOTECAS ----------
#include <stdio.h>   // Entrada e saída de dados
#include <stdlib.h>  // malloc, calloc, free, rand
#include <string.h>  // strcpy, strcmp, strstr
#include <time.h>    // time, srand

// ---------- CONSTANTES ----------
#define MAX_TERRITORIOS 5     // Quantidade total de territórios
#define TOTAL_MISSOES 3      // Quantidade de missões
#define TROPAS_INICIAIS 5

// ---------- ESTRUTURA ----------
typedef struct {
    char nome[30];   // Nome do território
    char cor[10];    // Cor do exército
    int tropas;      // Quantidade de tropas
} Territorio;

// ---------- VARIÁVEIS GLOBAIS ----------
char corMissao[10];        // Cor usada na missão de eliminação
int vitoriasSeguidas = 0;  // Contador de vitórias consecutivas
int territoriosZerados = 0; // Conta quantos territórios inimigos foram reduzidos a zero tropas

// ---------- PROTÓTIPOS ----------
void limparBuffer();
void exibirMapa(Territorio *mapa, int tamanho);
int atacar(Territorio *atacante, Territorio *defensor);
void sortearCorMissao(Territorio *mapa, int tamanho);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes, Territorio *mapa, int tamanho);
void exibirMissao(char *missao);
int verificarMissao(char *missao, Territorio *mapa, int tamanho);
void resetarJogo(Territorio *mapa, int tamanho);
void liberarMemoria(Territorio *mapa, char *missaoJogador);

// ---------- LIMPA BUFFER ----------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------- EXIBE MAPA ----------
void exibirMapa(Territorio *mapa, int tamanho) {
    printf("\n===== MAPA DO MUNDO =====\n");

    for (int i = 0; i < tamanho; i++) {
        printf("%d - %s         | Exército: %s               | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// ---------- FUNÇÃO DE ATAQUE ----------
int atacar(Territorio *atacante, Territorio *defensor) {

    // Sorteio dos dados (1 a 6)
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf(
    "O Atacante %s rolou o dado e tirou: %d | O Defensor %s rolou o dado e tirou: %d\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor
);

    if (dadoAtacante == dadoDefensor) {
    printf("Empate nos dados! Ninguém venceu esta batalha.\n");
    return -1;  // Indica empate, não conta como vitória nem derrota
} else 

    // Caso o atacante vença
if (dadoAtacante > dadoDefensor) {

        printf("Vitória do ataque!\n");
        defensor->tropas--;

        if (defensor->tropas == 0) {

            territoriosZerados++;

            printf("\nTerritório conquistado!\n");

            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = TROPAS_INICIAIS;

            printf("O território agora pertence ao exército %s!\n", defensor->cor);
        }

        return 1;

    } else {

        printf("Vitória da defesa!\n");

        atacante->tropas--;
        if (atacante->tropas < 0)
            atacante->tropas = 0;

        return 0;
    }
    
}

// ---------- SORTEIA COR DA MISSÃO ----------
void sortearCorMissao(Territorio *mapa, int tamanho) {
    int indice = rand() % tamanho; // Escolhe índice aleatório
    strcpy(corMissao, mapa[indice].cor); // Copia a cor do território
}

// ---------- ATRIBUI MISSÃO ----------
void atribuirMissao(char *destino, char *missoes[], int totalMissoes, Territorio *mapa, int tamanho) {

    int sorteio = rand() % totalMissoes; // Sorteia uma missão

    strcpy(destino, missoes[sorteio]); // Copia a missão para o jogador

    // Se a missão for eliminar todas as tropas da cor
    if (strcmp(destino, "ELIMINE TODAS AS TROPAS DA COR") == 0) {
        sortearCorMissao(mapa, tamanho); // Sorteia uma cor do mapa
        strcat(destino, " ");            // Adiciona espaço
        strcat(destino, corMissao);      // Concatena a cor sorteada
    }
}

// ---------- EXIBE MISSÃO ----------
void exibirMissao(char *missao) {
    printf("\n===== SUA MISSÃO =====\n");
    printf("%s\n", missao);
    printf("======================\n");
}

// ---------- VERIFICA MISSÃO ----------
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {

    if (strstr(missao, "ELIMINE TODAS AS TROPAS DA COR")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corMissao) == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    }

    if (strstr(missao, "VENÇA 3 BATALHAS SEGUIDAS!")) {
        return vitoriasSeguidas >= 3;
    }

    if (strstr(missao, "REDUZA UM TERRIÓRIO INIMIGO A ZERO TROPAS!")) {
        if (territoriosZerados >= 1) {
            printf("\nParabéns você concluiu a missão, e conquistou o território inimigo!\n");
            return 1;
        }
    }

    return 0;
}

// ---------- RESETAR JOGO ----------
void resetarJogo(Territorio *mapa, int tamanho) {

    printf("\nResetando o jogo...\n");

    vitoriasSeguidas = 0;
    territoriosZerados = 0;

    for (int i = 0; i < tamanho; i++) {
        mapa[i].tropas = TROPAS_INICIAIS;
    }
}
    

// ---------- LIBERA MEMÓRIA ----------
void liberarMemoria(Territorio *mapa, char *missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

// ---------- MAIN ----------
int main() {

    srand(time(NULL));

    Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof(Territorio));
    char *missaoJogador = malloc(100 * sizeof(char));

    char *missoes[TOTAL_MISSOES] = {
        "ELIMINE TODAS AS TROPAS DA COR",
        "VENÇA 3 BATALHAS SEGUIDAS!",
        "REDUZA UM TERRIÓRIO INIMIGO A ZERO TROPAS!"
    };

    // ---------- CADASTRO ----------
    printf("--- Vamos cadastrar os 5 territórios iniciais do nosso mundo! ---\n\n");
    
    for (int i = 0; i < MAX_TERRITORIOS; i++) {

        printf("== Cadastrando Território %d ==\n", i + 1);

        printf("Nome do território: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome,"\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();

        printf("-------------------------------------------\n\n");
    }

    // Atribui e exibe missão
    atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
    exibirMissao(missaoJogador);

    

    // ---------- LOOP PRINCIPAL ----------
while (1) {

    // Exibe o mapa atualizado
    exibirMapa(mapa, MAX_TERRITORIOS);

    printf("\n\n--- FASE DE ATAQUE ---\n");

    // Escolha do território atacante
    int atacante, defensor;
    printf("\nEscolha o território atacante (1 a 5, 0 para sair): ");
    scanf("%d", &atacante);

    // Se digitar 0, encerra o jogo
    if (atacante == 0)
        break;

    // Escolha do território defensor
    printf("Escolha o território defensor (1 a 5): ");
    scanf("%d", &defensor);
    limparBuffer();

    // Ajusta índices para vetor (0 a 4)
    atacante--;
    defensor--;

    // Validação das escolhas
    if (atacante < 0 || defensor < 0 ||
            atacante >= MAX_TERRITORIOS || defensor >= MAX_TERRITORIOS ||
            atacante == defensor) {

            printf("Jogada inválida!\n");
            continue;
        }

    // ---------- MENU DE AÇÕES ----------
    int opcao;
        printf("\n1 - Atacar\n2 - Verificar missão\n0 - Sair\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {

            int resultado = atacar(&mapa[atacante], &mapa[defensor]);

            if (resultado == 1)
                vitoriasSeguidas++;  // Vitória do atacante
            else if (resultado == 0)
                vitoriasSeguidas = 0; // Derrota do atacante
            else if (resultado == -1)
                ; // Empate: não altera vitoriasSeguidas

            // PAUSA ANTES DE CONTINUAR
            printf("\nPressione ENTER para continuar para o próximo turno...");
            getchar();


            if (verificarMissao(missaoJogador, mapa, MAX_TERRITORIOS)) {

                    // Se a missão for "Reduzir um território inimigo a zero tropas"
                if (strstr(missaoJogador, "REDUZA UM TERRIÓRIO INIMIGO A ZERO TROPAS!")) {

                    // Mensagem de conclusão da missão
                    printf("\nMISSÃO CONCLUÍDA! VOCÊ VENCEU!\n");

                    // Mensagem informando nova missão
                    printf("\nSerá gerada uma nova missão para você!\n");
                    printf("Pressione ENTER para continuar...");
                    getchar();  // pausa para o jogador ver a mensagem

                    // Gera nova missão
                    atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
                    exibirMissao(missaoJogador);

                    // Continua o jogo sem encerrar
                    continue;
                }

        // Verifica se a missão "Vencer três batalhas seguidas" foi concluída
        if (strstr(missaoJogador, "Vencer três batalhas seguidas") &&
            vitoriasSeguidas >= 3) {

        // Exibe mensagem de vitória
            printf("\n🎉 MISSÃO CONCLUÍDA! VOCÊ VENCEU! 🎉\n");

            // Exibe o menu pós-missão
            int opcaoMissao;
                do {
                    printf("\n===== MENU PÓS-MISSÃO =====\n");
                    printf("1 - Continuar jogo com nova missão\n");
                    printf("2 - Resetar jogo\n");
                    printf("0 - Sair do jogo\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoMissao);
                    limparBuffer();

                    switch(opcaoMissao) {
                        case 1:
                            // Gerar nova missão
                            atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
                            exibirMissao(missaoJogador);
                            vitoriasSeguidas = 0; // Reseta contador
                            break;

                        case 2:
                            // Resetar jogo
                            for (int i = 0; i < MAX_TERRITORIOS; i++) {
                                mapa[i].tropas = TROPAS_INICIAIS;
                            }
                            vitoriasSeguidas = 0;
                            atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
                            exibirMissao(missaoJogador);
                            break;

                        case 0:
                            printf("\nSaindo do jogo...\n");
                            liberarMemoria(mapa, missaoJogador);
                            return 0;

                        default:
                            printf("\nOpção inválida!\n");
                    }

                } while(opcaoMissao < 1 || opcaoMissao > 2); // Sai do menu após ação
            }

    liberarMemoria(mapa, missaoJogador);
    printf("\nJogo encerrado. Obrigada por jogar!\n");

    return 0;
            }
        }

    }

}

//incluir no jogo quando a defesa zerar a tropa do atacante exibir a mensagem de vitória e conquista do território,
//a cor e o território do atacante passa a ser o mesmo da defesa.

