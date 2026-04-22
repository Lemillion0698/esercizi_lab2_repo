COMPILATORE = gcc
ATTRIBUTI = -Wall -pedantic
ATTRIBUTI_PRELIMINARI = -g $(ATTRIBUTI)
ATTRIBUTI_DEFINITIVI = -O3 $(ATTRIBUTI)
FILE_OGGETTO = main.o

# Regola per il file oggetto (creato separatamente)
$(FILE_OGGETTO) : main.c
	$(COMPILATORE) $(ATTRIBUTI) -c main.c -o $(FILE_OGGETTO)

# Linking per la versione con debug
preliminare : $(FILE_OGGETTO)
	$(COMPILATORE) $(ATTRIBUTI_PRELIMINARI) $(FILE_OGGETTO) -o preliminare

# Linking per la versione ottimizzata
definitiva : $(FILE_OGGETTO)
	$(COMPILATORE) $(ATTRIBUTI_DEFINITIVI) $(FILE_OGGETTO) -o definitiva

esecuzione : definitiva
	./definitiva

.PHONY : pulizia preliminare definitiva esecuzione

pulizia :
	rm -f $(FILE_OGGETTO) definitiva preliminare