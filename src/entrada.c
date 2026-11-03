// entrada.c
// Leitura segura do teclado: nada que a pessoa digitar trava o jogo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "entrada.h"

// ---------- LIMPA BUFFER ----------
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------- PAUSA ----------
void pausar(void) {
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
