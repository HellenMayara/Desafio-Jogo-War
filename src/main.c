// DESAFIO NÍVEL MESTRE
// JOGO WAR EM C COM SISTEMA DE MISSÕES
//
// main.c: controla o fluxo do jogo (menus e turnos).
// As regras ficam nos outros módulos:
//   entrada.c    -> leitura segura do teclado
//   territorio.c -> mapa e batalhas
//   missoes.c    -> sistema de missões

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "entrada.h"
#include "territorio.h"
#include "missoes.h"

// ---------- LIBERA MEMÓRIA ----------
static void liberarMemoria(Territorio *mapa, Territorio *mapaOriginal, char *textoMissao) {
    free(mapa);
    free(mapaOriginal);
    free(textoMissao);
}

// ---------- MENU PÓS-MISSÃO ----------
// Retorna 0 se o jogador escolheu sair, 1 se o jogo continua.
static int menuPosMissao(Missao *missao, Territorio *mapa, Territorio *mapaOriginal) {

    printf("\n===== MENU PÓS-MISSÃO =====\n");
    printf("1 - Continuar jogo com nova missão\n");
    printf("2 - Resetar jogo\n");
    printf("0 - Sair do jogo\n");
    printf("Escolha uma opção: ");
    int opcao = lerInteiro(0, 2);

    if (opcao == 0)
        return 0;

    if (opcao == 2)
        resetarMapa(mapa, mapaOriginal, MAX_TERRITORIOS);

    atribuirMissao(missao, mapa, MAX_TERRITORIOS);
    exibirMissao(missao);
    pausar();
    return 1;
}

// ---------- TURNO DE ATAQUE ----------
// Retorna 0 se o jogo deve terminar, 1 se continua.
static int turnoDeAtaque(Missao *missao, Territorio *mapa, Territorio *mapaOriginal) {

    printf("\n--- FASE DE ATAQUE ---\n");

    printf("Escolha o território atacante (1 a %d): ", MAX_TERRITORIOS);
    int atacante = lerInteiro(1, MAX_TERRITORIOS) - 1; // Ajusta para índice do vetor (0 a 4)

    printf("Escolha o território defensor (1 a %d): ", MAX_TERRITORIOS);
    int defensor = lerInteiro(1, MAX_TERRITORIOS) - 1;

    // Validação das escolhas
    if (atacante == defensor) {
        printf("Jogada inválida! Um território não pode atacar a si mesmo.\n");
        pausar();
        return 1;
    }

    if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0) {
        printf("Jogada inválida! Os dois territórios são do exército %s.\n", mapa[atacante].cor);
        pausar();
        return 1;
    }

    int resultado = atacar(&mapa[atacante], &mapa[defensor]);
    registrarBatalha(missao, resultado);
    pausar();

    // Verificação silenciosa da missão
    if (!verificarMissao(missao, mapa, MAX_TERRITORIOS))
        return 1;

    printf("\n🎉 MISSÃO CONCLUÍDA! 🎉\n");
    printf("%s\n", missao->texto);

    switch (missao->id) {

        // Reduzir território a zero: gera nova missão e continua
        case MISSAO_ZERAR_TERRITORIO:
            printf("\nVocê conquistou o território inimigo! Será gerada uma nova missão.\n");
            pausar();
            atribuirMissao(missao, mapa, MAX_TERRITORIOS);
            exibirMissao(missao);
            pausar();
            return 1;

        // Vencer 3 batalhas seguidas: menu pós-missão
        case MISSAO_VENCER_TRES:
            return menuPosMissao(missao, mapa, mapaOriginal);

        // Eliminar uma cor: vitória final
        default:
            printf("\nO exército %s foi eliminado do mapa. VOCÊ VENCEU O JOGO!\n", missao->cor);
            return 0;
    }
}

// ---------- MAIN ----------
int main(void) {

    srand(time(NULL));

    Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof(Territorio));
    Territorio *mapaOriginal = malloc(MAX_TERRITORIOS * sizeof(Territorio)); // Cópia para o reset
    Missao missao;
    missao.texto = malloc(TAM_MISSAO * sizeof(char));

    // Sempre conferir se a memória foi alocada
    if (mapa == NULL || mapaOriginal == NULL || missao.texto == NULL) {
        printf("Erro ao alocar memória!\n");
        liberarMemoria(mapa, mapaOriginal, missao.texto); // free(NULL) é seguro
        return 1;
    }

    cadastrarTerritorios(mapa, MAX_TERRITORIOS);

    // Guarda o mapa inicial para poder resetar depois
    memcpy(mapaOriginal, mapa, MAX_TERRITORIOS * sizeof(Territorio));

    // Atribui e exibe missão
    atribuirMissao(&missao, mapa, MAX_TERRITORIOS);
    exibirMissao(&missao);
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

        exibirMapa(mapa, MAX_TERRITORIOS);

        // ---------- MENU DE AÇÕES ----------
        printf("\n1 - Atacar\n2 - Verificar missão\n0 - Sair\nEscolha: ");
        int opcao = lerInteiro(0, 2);

        switch (opcao) {
            case 1:
                jogando = turnoDeAtaque(&missao, mapa, mapaOriginal);
                break;

            case 2:
                exibirProgresso(&missao, mapa, MAX_TERRITORIOS);
                pausar();
                break;

            case 0:
                jogando = 0;
                break;
        }
    }

    // Único ponto de saída: a memória é sempre liberada
    printf("\nJogo encerrado. Obrigada por jogar!\n");
    liberarMemoria(mapa, mapaOriginal, missao.texto);

    return 0;
}
