// territorio.h
// Estrutura do território e funções do mapa e das batalhas.

#ifndef TERRITORIO_H
#define TERRITORIO_H

// ---------- CONSTANTES ----------
#define MAX_TERRITORIOS 5      // Quantidade total de territórios
#define TROPAS_INICIAIS 5      // Tropas que um território recebe ao ser conquistado
#define MAX_TROPAS_CADASTRO 99
#define TAM_NOME 30
#define TAM_COR 10

// Possíveis resultados de uma batalha
#define RESULTADO_EMPATE   -1
#define RESULTADO_DEFESA    0  // Defesa venceu
#define RESULTADO_ATAQUE    1  // Ataque venceu
#define RESULTADO_CONQUISTA 2  // Ataque venceu e conquistou o território

// ---------- ESTRUTURA ----------
typedef struct {
    char nome[TAM_NOME];  // Nome do território
    char cor[TAM_COR];    // Cor do exército
    int tropas;           // Quantidade de tropas
} Territorio;

// ---------- PROTÓTIPOS ----------
void cadastrarTerritorios(Territorio *mapa, int tamanho);
void exibirMapa(Territorio *mapa, int tamanho);
int atacar(Territorio *atacante, Territorio *defensor);
int mundoDominado(Territorio *mapa, int tamanho);
void resetarMapa(Territorio *mapa, Territorio *mapaOriginal, int tamanho);

#endif
