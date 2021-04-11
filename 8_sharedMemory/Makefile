CC=gcc
CFLAGS= -Wall -g 
EXEC1 = cuisinier
EXEC2 = serveur
OPT= -lrt -lpthread

all: cuisinier serveur

cuisinier:cuisinier.c 
	$(CC) $(CFLAGS) cuisinier.c -o $(EXEC1) $(OPT) 

serveur:serveur.c
	$(CC) $(CFLAGS) serveur.c -o $(EXEC2) $(OPT) 


clean:
	rm -rf *.o cuisinier serveur
