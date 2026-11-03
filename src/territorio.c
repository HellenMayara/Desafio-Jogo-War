// territorio.c
// Cadastro, exibição do mapa e regras de batalha.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "territorio.h"
#include "entrada.h"

// ---------- CADASTRO ----------
void cadastrarTerritorios(Territorio *mapa, int tamanho) {

    printf("--- Vamos cadastrar os %d territórios iniciais do nosso mundo! ---\n\n", tamanho);

    for (int i = 0; i < tamanho; i++) {

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
// Devolve um dos RESULTADO_... definidos em territorio.h
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
        return RESULTADO_EMPATE;
    }

    // Caso o atacante vença
    if (dadoAtacante > dadoDefensor) {

        printf("Vitória do ataque! %s perdeu 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        if (defensor->tropas == 0) {
            printf("\nTerritório %s conquistado!\n", defensor->nome);

            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = TROPAS_INICIAIS;

            printf("O território agora pertence ao exército %s!\n", defensor->cor);
            return RESULTADO_CONQUISTA;
        }

        return RESULTADO_ATAQUE;
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

    return RESULTADO_DEFESA;
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

// ---------- RESETA O MAPA ----------
// Volta o mapa exatamente como foi cadastrado (nomes, cores e tropas).
void resetarMapa(Territorio *mapa, Territorio *mapaOriginal, int tamanho) {
    printf("\nResetando o jogo...\n");
    memcpy(mapa, mapaOriginal, tamanho * sizeof(Territorio));
}
