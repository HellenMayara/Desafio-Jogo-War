# Compila o jogo com: make
# Executa com:        make run
# Apaga o executável: make clean

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRC = src/main.c src/entrada.c src/territorio.c src/missoes.c
EXE = war

$(EXE): $(SRC) include/*.h
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE) $(EXE).exe

.PHONY: run clean
