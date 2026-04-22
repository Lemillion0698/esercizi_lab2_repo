# Definizione delle variabili
CC = gcc 
FLAGS = -Wall -pedantic
OGGETTO = array_pun_a_fun.o 
SORGENTE = array_pun_a_fun.c

all: punt

# Fase di linking: ottengo l'eseguibile dal file .o
punt: $(OGGETTO)
	$(CC) $(FLAGS) $(OGGETTO) -o punt

# Fase di compilazione: ottengo il file .o dal sorgente
$(OGGETTO): $(SORGENTE)
	$(CC) $(FLAGS) -c $(SORGENTE) -o $(OGGETTO)

.PHONY: clean

clean: 
	rm -f $(OGGETTO) punt
