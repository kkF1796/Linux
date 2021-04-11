/*
-----------------------------------------
Elham Amin Mansour
Kathleen Favre

Utilisation: 
	- ./cuisinier
	- avec ./serveur dans une autre fenêtre
-----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

// ces constantes sont modulables par l'utilisateur
const int MAX_PIZZA=10;
const int MAX_SIZE_ETAGERE=3;
const int MAX_TEMPS_ATTENTE=3;

void die(char *issue) {
  perror(issue); 
  exit(EXIT_FAILURE); 
}

int main(int argc, char *argv[]){
	int i;	
	
	// définition des noms de la mémoire partagée et des 3 sémaphores
	const char *name="comptoir";
	const char *sem1="TropDePizzas";
	const char *sem2="NoPizza";
	const char *sem3="aMonTour";

	int shared_mmry;
	int *etagere;
	sem_t *NoPizza, *TropDePizza, *aMonTour;

	//création d'une zone pour la mémoire partagée
	shared_mmry=shm_open(name,O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(shared_mmry == -1){
		die("Erreur dans la définition de la zone de la mémoire partagée. \n");
	}

	// définition de la taille de l'espace de la mémoire partagée
	if(ftruncate(shared_mmry, sizeof(int)) == -1){
		die("Erreur dans la définition de la taille de la mémoire partagée. \n");
	}

	// relier la mémoire partagée et l'adresse mémoire virtuelle
	etagere=mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_mmry, 0);
	if(etagere == MAP_FAILED){
		die("Erreur dans mapping.\n");
	}

	//définition des sémaphores
	//sémaphore qui bloque le cuisinier lorsqu'il 3 pizzas sur etagère
	TropDePizza=sem_open(sem1, O_CREAT, S_IRUSR | S_IWUSR,MAX_SIZE_ETAGERE);
	//sémaphore qui bloque lle serveur tant qu'il n'y a pas de pizza sur l'étagère
	NoPizza=sem_open(sem2, O_CREAT, S_IRUSR | S_IWUSR,0);
	//sémaphore qui gère l'exclusions mutuelle, alternance entre les 2, éviter la collision
	//valeur initiale à 1 pour permettre le cusinier de commencer
	aMonTour=sem_open(sem3, O_CREAT, S_IRUSR | S_IWUSR,1);

	// boucle pour cuisiner les pizzas
	for(i=0;i<MAX_PIZZA;i++){
		sem_wait(TropDePizza); //vérifie si l'on n'a pas dépassé le max de 3 pizzas 
		sleep(rand()%MAX_TEMPS_ATTENTE+1);//un temps d'attente aléatoire pour la cuisine d'une pizza
		sem_wait(aMonTour);//bloquer le serveur pendant le temps qu'il veut changer le nombre de pizza sur étagère
		(*etagere)++;
		printf("Cuisinier: Une nouvelle pizza cuisinée, il y a %d pizzas sur l'étagère.\n", *etagere);
		sem_post(aMonTour);//debloquer serveur
		sem_post(NoPizza);//une pizza de plus à servir
	
	}

	printf("Cuisinier: j'ai cuisiné mes 10 pizzas, il y a %d pizzas à vendre.\n", *etagere);

	// fermeture et libération des sémaphores
	if(sem_close(NoPizza) == -1 || sem_close(TropDePizza) == -1 || sem_close(aMonTour) == -1){
		die("Mauvaise fermeture des sémaphores. \n");
	}
	
	/*if(sem_unlink(sem1) == -1 || sem_unlink(sem2) == -1 || sem_unlink(sem3) == -1){
		die("Mauvaise libération des sémaphores. \n");
	}*/

	// suppression du mapping, fermeture et libération de la mémoire partagée
	if(munmap(etagere, sizeof(int)) == -1){
		die("Erreur de la suppression du mapping.\n");
	}
	
	if(close(shared_mmry) == -1){
		die("Erreur dans la fermeture de mémoire partagée.\n");
	}

	/*if(shm_unlink(name) == -1){
		die("Erreur lors de la destruction du segment de mémoire partagé.\n");
	}*/

	return 0;
	
}
