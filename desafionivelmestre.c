// DESAFIO NÍVEL MESTRE
// JOGO WAR EM C COM SISTEMA DE MISSÕES

// ---------- BIBLIOTECAS ----------
#include <stdio.h>   // Entrada e saída de dados
#include <stdlib.h>  // malloc, calloc, free, rand
#include <string.h>  // strcpy, strcmp, strcat, memcpy
#include <ctype.h>   // toupper
#include <time.h>    // time, srand

// ---------- CONSTANTES ----------
#define MAX_TERRITORIOS 5    // Quantidade total de territórios
#define TOTAL_MISSOES 3      // Quantidade de missões
#define TROPAS_INICIAIS 5    // Tropas que um território recebe ao ser conquistado
#define MAX_TROPAS_CADASTRO 99
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100

// Cada missão tem um número fixo. Comparar números é mais seguro do que
// comparar textos com strstr: se o texto da missão mudar, o jogo continua funcionando.
#define MISSAO_ELIMINAR_COR 0
#define MISSAO_VENCER_TRES 1
#define MISSAO_ZERAR_TERRITORIO 2

// ---------- ESTRUTURA ----------
typedef struct {
    char nome[TAM_NOME];  // Nome do território
    char cor[TAM_COR];    // Cor do exército
    int tropas;           // Quantidade de tropas
} Territorio;

// ---------- VARIÁVEIS GLOBAIS ----------
char corMissao[TAM_COR];    // Cor usada na missão de eliminação
int vitoriasSeguidas = 0;   // Contador de vitórias consecutivas
int territoriosZerados = 0; // Quantos territórios inimigos foram reduzidos a zero tropas

// ---------- PROTÓTIPOS ----------
void limparBuffer();
void pausar();
void lerTexto(char *destino, int tamanho);
int lerInteiro(int minimo, int maximo);
void paraMaiusculas(char *texto);
void exibirMapa(Territorio *mapa, int tamanho);
int atacar(Territorio *atacante, Territorio *defensor);
void sortearCorMissao(Territorio *mapa, int tamanho);
int atribuirMissao(char *destino, char *missoes[], int totalMissoes, Territorio *mapa, int tamanho);
void exibirMissao(char *missao);
int verificarMissao(int idMissao, Territorio *mapa, int tamanho);
void exibirProgresso(int idMissao, char *missao, Territorio *mapa, int tamanho);
int mundoDominado(Territorio *mapa, int tamanho);
void resetarJogo(Territorio *mapa, Territorio *mapaOriginal, int tamanho);
void liberarMemoria(Territorio *mapa, Territorio *mapaOriginal, char *missaoJogador);

// ---------- LIMPA BUFFER ----------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------- PAUSA ----------
void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
}

// ---------- LÊ TEXTO COM SEGURANÇA ----------
// Lê uma linha, remove o '\n' e não aceita campo vazio.
// Se a pessoa digitar mais caracteres do que cabem, o excesso é descartado
// (senão ele "vazaria" para a próxima leitura).
void lerTexto(char *destino, int tamanho) {
    while (1) {
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        if (strchr(destino, '\n') == NULL)
            limparBuffer();

        destino[strcspn(destino, "\n")] = '\0';

        if (strlen(destino) > 0)
            return;

        printf("Campo obrigatório, digite novamente: ");
    }
}

// ---------- LÊ NÚMERO COM SEGURANÇA ----------
// Repete a pergunta até receber um número inteiro entre minimo e maximo.
// Sem isso, digitar uma letra no scanf travava o jogo em loop infinito.
int lerInteiro(int minimo, int maximo) {
    int valor;
    int lido;

    while (1) {
        lido = scanf("%d", &valor);

        if (lido == EOF) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        limparBuffer();

        if (lido == 1 && valor >= minimo && valor <= maximo)
            return valor;

        printf("Valor inválido! Digite um número entre %d e %d: ", minimo, maximo);
    }
}

// ---------- CONVERTE PARA MAIÚSCULAS ----------
// Assim "azul", "Azul" e "AZUL" contam como o mesmo exército.
void paraMaiusculas(char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper((unsigned char) texto[i]);
    }
}

// ---------- EXIBE MAPA ----------
void exibirMapa(Territorio *mapa, int tamanho) {
    printf("\n===== MAPA DO MUNDO =====\n");

    for (int i = 0; i < tamanho; i++) {
        printf("%d - %-20s | Exército: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// ---------- FUNÇÃO DE ATAQUE ----------
// Retorna: 1 = vitória do ataque | 0 = vitória da defesa | -1 = empate
int atacar(Territorio *atacante, Territorio *defensor) {

    // Sorteio dos dados (1 a 6)
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O Atacante %s rolou o dado e tirou: %d | O Defensor %s rolou o dado e tirou: %d\n",
           atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    // Empate: ninguém perde tropas
    if (dadoAtacante == dadoDefensor) {
        printf("Empate nos dados! Ninguém venceu esta batalha.\n");
        return -1;
    }

    // Caso o atacante vença
    if (dadoAtacante > dadoDefensor) {

        printf("Vitória do ataque! %s perdeu 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        if (defensor->tropas == 0) {
            territoriosZerados++;

            printf("\nTerritório %s conquistado!\n", defensor->nome);

            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = TROPAS_INICIAIS;

            printf("O território agora pertence ao exército %s!\n", defensor->cor);
        }

        return 1;
    }

    // Caso a defesa vença
    printf("Vitória da defesa! %s perdeu 1 tropa.\n", atacante->nome);
    atacante->tropas--;

    // Se o atacante ficar sem tropas, a defesa conquista o território dele
    if (atacante->tropas == 0) {
        printf("\nA defesa contra-atacou! Território %s conquistado!\n", atacante->nome);

        strcpy(atacante->cor, defensor->cor);
        atacante->tropas = TROPAS_INICIAIS;

        printf("O território agora pertence ao exército %s!\n", atacante->cor);
    }

    return 0;
}

// ---------- SORTEIA COR DA MISSÃO ----------
void sortearCorMissao(Territorio *mapa, int tamanho) {
    int indice = rand() % tamanho;        // Escolhe índice aleatório
    strcpy(corMissao, mapa[indice].cor);  // Copia a cor do território
}

// ---------- ATRIBUI MISSÃO ----------
// Copia o texto da missão para "destino" e devolve o número da missão sorteada.
int atribuirMissao(char *destino, char *missoes[], int totalMissoes, Territorio *mapa, int tamanho) {

    int sorteio = rand() % totalMissoes;  // Sorteia uma missão

    strcpy(destino, missoes[sorteio]);    // Copia a missão para o jogador

    // Se a missão for eliminar uma cor, sorteia qual cor
    if (sorteio == MISSAO_ELIMINAR_COR) {
        sortearCorMissao(mapa, tamanho);
        strcat(destino, " ");
        strcat(destino, corMissao);
    }

    // Toda missão nova começa do zero
    vitoriasSeguidas = 0;
    territoriosZerados = 0;

    return sorteio;
}

// ---------- EXIBE MISSÃO ----------
void exibirMissao(char *missao) {
    printf("\n===== SUA MISSÃO =====\n");
    printf("%s\n", missao);
    printf("======================\n");
}

// ---------- VERIFICA MISSÃO (silenciosa) ----------
// Retorna 1 se a missão foi cumprida, 0 se ainda não.
int verificarMissao(int idMissao, Territorio *mapa, int tamanho) {

    switch (idMissao) {

        case MISSAO_ELIMINAR_COR:
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, corMissao) == 0 && mapa[i].tropas > 0)
                    return 0;
            }
            return 1;

        case MISSAO_VENCER_TRES:
            return vitoriasSeguidas >= 3;

        case MISSAO_ZERAR_TERRITORIO:
            return territoriosZerados >= 1;
    }

    return 0;
}

// ---------- EXIBE PROGRESSO DA MISSÃO ----------
void exibirProgresso(int idMissao, char *missao, Territorio *mapa, int tamanho) {

    exibirMissao(missao);

    if (idMissao == MISSAO_ELIMINAR_COR) {
        int restantes = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corMissao) == 0)
                restantes++;
        }
        printf("Territórios do exército %s ainda no mapa: %d\n", corMissao, restantes);
    }
    else if (idMissao == MISSAO_VENCER_TRES) {
        printf("Vitórias seguidas: %d de 3\n", vitoriasSeguidas);
    }
    else if (idMissao == MISSAO_ZERAR_TERRITORIO) {
        printf("Nenhum território inimigo foi conquistado ainda.\n");
    }
}

// ---------- VERIFICA SE UM EXÉRCITO DOMINOU O MUNDO ----------
// Se todos os territórios têm a mesma cor, não sobra ninguém para atacar.
int mundoDominado(Territorio *mapa, int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, mapa[0].cor) != 0)
            return 0;
    }
    return 1;
}

// ---------- RESETAR JOGO ----------
// Volta o mapa exatamente como foi cadastrado (nomes, cores e tropas).
void resetarJogo(Territorio *mapa, Territorio *mapaOriginal, int tamanho) {

    printf("\nResetando o jogo...\n");

    memcpy(mapa, mapaOriginal, tamanho * sizeof(Territorio));

    vitoriasSeguidas = 0;
    territoriosZerados = 0;
}

// ---------- LIBERA MEMÓRIA ----------
void liberarMemoria(Territorio *mapa, Territorio *mapaOriginal, char *missaoJogador) {
    free(mapa);
    free(mapaOriginal);
    free(missaoJogador);
}

// ---------- MAIN ----------
int main() {

    srand(time(NULL));

    Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof(Territorio));
    Territorio *mapaOriginal = malloc(MAX_TERRITORIOS * sizeof(Territorio)); // Cópia para o reset
    char *missaoJogador = malloc(TAM_MISSAO * sizeof(char));

    // Sempre conferir se a memória foi alocada
    if (mapa == NULL || mapaOriginal == NULL || missaoJogador == NULL) {
        printf("Erro ao alocar memória!\n");
        liberarMemoria(mapa, mapaOriginal, missaoJogador); // free(NULL) é seguro
        return 1;
    }

    // A ordem aqui precisa bater com os números MISSAO_... lá em cima
    char *missoes[TOTAL_MISSOES] = {
        "ELIMINE TODAS AS TROPAS DA COR",
        "VENÇA 3 BATALHAS SEGUIDAS!",
        "REDUZA UM TERRITÓRIO INIMIGO A ZERO TROPAS!"
    };

    // ---------- CADASTRO ----------
    printf("--- Vamos cadastrar os %d territórios iniciais do nosso mundo! ---\n\n", MAX_TERRITORIOS);

    for (int i = 0; i < MAX_TERRITORIOS; i++) {

        printf("== Cadastrando Território %d ==\n", i + 1);

        printf("Nome do território: ");
        lerTexto(mapa[i].nome, TAM_NOME);

        printf("Cor do exército: ");
        lerTexto(mapa[i].cor, TAM_COR);
        paraMaiusculas(mapa[i].cor);

        printf("Número de Tropas (1 a %d): ", MAX_TROPAS_CADASTRO);
        mapa[i].tropas = lerInteiro(1, MAX_TROPAS_CADASTRO);

        printf("-------------------------------------------\n\n");
    }

    // Guarda o mapa inicial para poder resetar depois
    memcpy(mapaOriginal, mapa, MAX_TERRITORIOS * sizeof(Territorio));

    // Atribui e exibe missão
    int missaoAtual = atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
    exibirMissao(missaoJogador);
    pausar();

    // ---------- LOOP PRINCIPAL ----------
    int jogando = 1;

    while (jogando) {

        // Se só sobrou um exército, não há mais batalhas possíveis
        if (mundoDominado(mapa, MAX_TERRITORIOS)) {
            exibirMapa(mapa, MAX_TERRITORIOS);
            printf("\nO exército %s dominou o mundo! Não há mais territórios para atacar.\n", mapa[0].cor);
            break;
        }

        // Exibe o mapa atualizado
        exibirMapa(mapa, MAX_TERRITORIOS);

        // ---------- MENU DE AÇÕES ----------
        printf("\n1 - Atacar\n2 - Verificar missão\n0 - Sair\nEscolha: ");
        int opcao = lerInteiro(0, 2);

        if (opcao == 0) {
            jogando = 0;
        }
        else if (opcao == 2) {
            exibirProgresso(missaoAtual, missaoJogador, mapa, MAX_TERRITORIOS);
            pausar();
        }
        else if (opcao == 1) {

            printf("\n--- FASE DE ATAQUE ---\n");

            printf("Escolha o território atacante (1 a %d): ", MAX_TERRITORIOS);
            int atacante = lerInteiro(1, MAX_TERRITORIOS) - 1; // Ajusta para índice do vetor (0 a 4)

            printf("Escolha o território defensor (1 a %d): ", MAX_TERRITORIOS);
            int defensor = lerInteiro(1, MAX_TERRITORIOS) - 1;

            // Validação das escolhas
            if (atacante == defensor) {
                printf("Jogada inválida! Um território não pode atacar a si mesmo.\n");
                pausar();
                continue;
            }

            if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0) {
                printf("Jogada inválida! Os dois territórios são do exército %s.\n", mapa[atacante].cor);
                pausar();
                continue;
            }

            int resultado = atacar(&mapa[atacante], &mapa[defensor]);

            if (resultado == 1) {
                vitoriasSeguidas++;     // Vitória do atacante
            } else if (resultado == 0) {
                vitoriasSeguidas = 0;   // Derrota do atacante
            }
            // Empate (-1): não altera vitoriasSeguidas

            pausar();

            // Verificação silenciosa da missão
            if (verificarMissao(missaoAtual, mapa, MAX_TERRITORIOS)) {

                printf("\n🎉 MISSÃO CONCLUÍDA! 🎉\n");
                printf("%s\n", missaoJogador);

                // Missão "Reduzir território a zero": gera nova missão e continua
                if (missaoAtual == MISSAO_ZERAR_TERRITORIO) {

                    printf("\nVocê conquistou o território inimigo! Será gerada uma nova missão.\n");
                    pausar();

                    missaoAtual = atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
                    exibirMissao(missaoJogador);
                    pausar();
                }

                // Missão "Vencer 3 batalhas seguidas": menu pós-missão
                else if (missaoAtual == MISSAO_VENCER_TRES) {

                    printf("\n===== MENU PÓS-MISSÃO =====\n");
                    printf("1 - Continuar jogo com nova missão\n");
                    printf("2 - Resetar jogo\n");
                    printf("0 - Sair do jogo\n");
                    printf("Escolha uma opção: ");
                    int opcaoMissao = lerInteiro(0, 2);

                    switch (opcaoMissao) {
                        case 1:
                            missaoAtual = atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
                            exibirMissao(missaoJogador);
                            pausar();
                            break;

                        case 2:
                            resetarJogo(mapa, mapaOriginal, MAX_TERRITORIOS);
                            missaoAtual = atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES, mapa, MAX_TERRITORIOS);
                            exibirMissao(missaoJogador);
                            pausar();
                            break;

                        case 0:
                            jogando = 0;
                            break;
                    }
                }

                // Missão "Eliminar uma cor": vitória final
                else {
                    printf("\nO exército %s foi eliminado do mapa. VOCÊ VENCEU O JOGO!\n", corMissao);
                    jogando = 0;
                }
            }
        }
    }

    // Único ponto de saída: a memória é sempre liberada
    printf("\nJogo encerrado. Obrigada por jogar!\n");
    liberarMemoria(mapa, mapaOriginal, missaoJogador);

    return 0;
}
