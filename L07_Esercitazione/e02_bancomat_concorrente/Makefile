COMPILATORE = gcc
CFLAGS = -Wall -pedantic -std=c11
NAME = bancomat_atomics
LIBS = -lpthread -lrt

.PHONY: default clean run

default: $(NAME) #serve perché quando digito "make" semplicemente, sarà il file creato per difetto

# target (principale) per l'eseguibile
$(NAME): $(NAME).o
	$(COMPILATORE) $(CFLAGS) $(NAME).o -o $(NAME) $(LIBS)

#target (intermedio) per il file oggetto
$(NAME).o: $(NAME).c
	$(COMPILATORE) -c $(CFLAGS) $(NAME).c -o $(NAME).o

# target di utilità (run e clean)
run: $(NAME)
	./$(NAME)

clean: 
	rm -f $(NAME) $(NAME).o