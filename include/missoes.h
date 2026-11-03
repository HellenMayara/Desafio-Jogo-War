// missoes.h
// Sistema de missões: sorteio, progresso e verificação.

#ifndef MISSOES_H
#define MISSOES_H

#include "territorio.h"

// ---------- CONSTANTES ----------
#define TOTAL_MISSOES 3
#define TAM_MISSAO 100

// Cada missão tem um número fixo. Comparar números é mais seguro do que
// comparar textos com strstr: se o texto da missão mudar, o jogo continua funcionando.
#define MISSAO_ELIMINAR_COR 0
#define MISSAO_VENCER_TRES 1
#define MISSAO_ZERAR_TERRITORIO 2

// ---------- ESTRUTURA ----------
// Tudo que o jogo precisa saber sobre a missão atual fica junto aqui,
// em vez de espalhado em variáveis globais.
typedef struct {
    int id;                   // Qual missão (MISSAO_...)
    char *texto;              // Texto exibido ao jogador (alocado com malloc)
    char cor[TAM_COR];        // Cor sorteada (só na missão de eliminar)
    int vitoriasSeguidas;     // Contador de vitórias consecutivas
    int territoriosZerados;   // Territórios inimigos reduzidos a zero tropas
} Missao;

// ---------- PROTÓTIPOS ----------
void atribuirMissao(Missao *missao, Territorio *mapa, int tamanho);
void exibirMissao(Missao *missao);
void registrarBatalha(Missao *missao, int resultado);
int verificarMissao(Missao *missao, Territorio *mapa, int tamanho);
void exibirProgresso(Missao *missao, Territorio *mapa, int tamanho);

#endif
