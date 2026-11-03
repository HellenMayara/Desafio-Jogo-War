// entrada.h
// Funções para ler dados do teclado com segurança.

#ifndef ENTRADA_H
#define ENTRADA_H

void limparBuffer(void);
void pausar(void);
void lerTexto(char *destino, int tamanho);
int lerInteiro(int minimo, int maximo);
void paraMaiusculas(char *texto);

#endif
