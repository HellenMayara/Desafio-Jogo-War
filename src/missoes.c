// missoes.c
// Sorteio, acompanhamento e verificação das missões.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "missoes.h"

// A ordem aqui precisa bater com os números MISSAO_... em missoes.h
static const char *textosMissoes[TOTAL_MISSOES] = {
    "ELIMINE TODAS AS TROPAS DA COR",
    "VENÇA 3 BATALHAS SEGUIDAS!",
    "REDUZA UM TERRITÓRIO INIMIGO A ZERO TROPAS!"
};

// ---------- ATRIBUI MISSÃO ----------
// Sorteia uma missão e zera os contadores: toda missão nova começa do zero.
void atribuirMissao(Missao *missao, Territorio *mapa, int tamanho) {

    missao->id = rand() % TOTAL_MISSOES;
    strcpy(missao->texto, textosMissoes[missao->id]);

    // Se a missão for eliminar uma cor, sorteia qual cor do mapa
    if (missao->id == MISSAO_ELIMINAR_COR) {
        int indice = rand() % tamanho;
        strcpy(missao->cor, mapa[indice].cor);

        strcat(missao->texto, " ");
        strcat(missao->texto, missao->cor);
    }

    missao->vitoriasSeguidas = 0;
    missao->territoriosZerados = 0;
}

// ---------- EXIBE MISSÃO ----------
void exibirMissao(Missao *missao) {
    printf("\n===== SUA MISSÃO =====\n");
    printf("%s\n", missao->texto);
    printf("======================\n");
}

// ---------- REGISTRA O RESULTADO DE UMA BATALHA ----------
void registrarBatalha(Missao *missao, int resultado) {

    if (resultado == RESULTADO_ATAQUE || resultado == RESULTADO_CONQUISTA) {
        missao->vitoriasSeguidas++;
    } else if (resultado == RESULTADO_DEFESA) {
        missao->vitoriasSeguidas = 0;
    }
    // Empate: não altera o contador

    if (resultado == RESULTADO_CONQUISTA) {
        missao->territoriosZerados++;
    }
}

// ---------- VERIFICA MISSÃO (silenciosa) ----------
// Retorna 1 se a missão foi cumprida, 0 se ainda não.
int verificarMissao(Missao *missao, Territorio *mapa, int tamanho) {

    switch (missao->id) {

        case MISSAO_ELIMINAR_COR:
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, missao->cor) == 0 && mapa[i].tropas > 0)
                    return 0;
            }
            return 1;

        case MISSAO_VENCER_TRES:
            return missao->vitoriasSeguidas >= 3;

        case MISSAO_ZERAR_TERRITORIO:
            return missao->territoriosZerados >= 1;
    }

    return 0;
}

// ---------- EXIBE PROGRESSO DA MISSÃO ----------
void exibirProgresso(Missao *missao, Territorio *mapa, int tamanho) {

    exibirMissao(missao);

    if (missao->id == MISSAO_ELIMINAR_COR) {
        int restantes = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, missao->cor) == 0)
                restantes++;
        }
        printf("Territórios do exército %s ainda no mapa: %d\n", missao->cor, restantes);
    }
    else if (missao->id == MISSAO_VENCER_TRES) {
        printf("Vitórias seguidas: %d de 3\n", missao->vitoriasSeguidas);
    }
    else if (missao->id == MISSAO_ZERAR_TERRITORIO) {
        printf("Nenhum território inimigo foi conquistado ainda.\n");
    }
}
