/*
-----------------------------------------------
Favre Kathleen,  Amin Mansour Elham
-----------------------------------------------
*/

#include "builtin.h"
#include "task.h"

#define BUFFER_SIZE 10000
#define ARGV_SIZE 100

// Fonction d'erreur
void die(char *issue) {
  perror(issue); 
  exit(EXIT_FAILURE); 
}


int main(int argc, char *argv[]){

	char buffer[BUFFER_SIZE];
	char *ptr;
	char *(*myargv);
	int myargc=0;	
	
	// vérifie que la seule commande est mybash
	if(argc != 1){
		die("Commande introuvable. \n");
	
	}

	pid_t pid_new;
	int stat_new;
	
	
	while(1){
		
		myargc=0;

		// Ignorer SIGQUIT et SIGTERM
		struct sigaction signalIgn;  
		sigemptyset(&signalIgn.sa_mask);
		signalIgn.sa_flags = SA_RESTART ;
		signalIgn.sa_handler = SIG_IGN;// action faite lorsque le signal est reçu
		sigaction(SIGQUIT, &signalIgn, 0); 
		sigaction(SIGTERM, &signalIgn, 0);
	
		// Gestion du signal SIGHUP
		struct sigaction signalClose;  
		sigemptyset(&signalClose.sa_mask);
		signalClose.sa_flags = SA_RESTART ;
		signalClose.sa_handler = closeAll;//fonction closeall qui ferme tout processus
		sigaction(SIGHUP, &signalClose, NULL);
	
		// Saisie de la commande
		printf("Your wish is my command: \n");

		if(fgets(buffer,BUFFER_SIZE,stdin) != NULL){ 
		
		//utilisation de strtok pour separer les arguments de ligne de commande
		char bufferCopie[10000];
		strcpy(bufferCopie, buffer);
		ptr=strtok(buffer," \t");

		while(ptr != NULL){
			
			ptr = strtok(NULL," \t");
			myargc++;
			
		}
	
		myargv = malloc(sizeof(char*) * myargc);
		if(!argv){
			die("Erreur d'allocation.\n");
		}
		int i = 0;
		ptr=NULL;
		ptr=strtok(bufferCopie," \t");
		
		// remplissage de myargv (équivalent de argv pour notre shell)
		while(ptr != NULL){
			myargv[i] = malloc((strlen(ptr) + 1) * sizeof(char));
			if(!myargv[i]){
				die("Erreur d'allocation.\n");
			}
			sprintf(myargv[i],"%s",ptr);
			ptr = strtok(NULL," \t");
			
			i++;
		}
		
		myargv[myargc-1][strlen(myargv[myargc-1]) - 1] = '\0';
		
		// gestion des fonctions builtin
		if(!strcmp("exit",myargv[0])){
			exit_process();
			
		}
		else if(!strcmp("cd",myargv[0])){
			changeDir(myargv[1]);
		}
		else{
			// cas de la tache de fond
			if((myargv[myargc-1][strlen(myargv[myargc - 1])-1] == '&' )|| !strcmp(myargv[myargc - 1], "&")){
				myargv[myargc-1][strlen(myargv[myargc-1]) - 1] = '\0';

				//gestion d'une unique tache de fond			
				if(exists_background()){
					printf("not possible, there is already a background task\n");
				}
				else{				
					printf("authorisation de faire tache de fond\n");
				
					backgroundTask(myargv,myargc);
			
				}
				
			}
			else{
					//gestion de la tache principale					
					taskManager(myargv,myargc);
					
					
				
				
			}
		}
		
 	
	// libération de la mémoire pour tableau myargv, remise à 0 des variables et on vide le buffer
		for(int j = 0; j < myargc; j++){
			free(myargv[j]);
			myargv[j] = NULL;
		}
		free(myargv);
		myargv = NULL;
		ptr=NULL;
		bzero(buffer,BUFFER_SIZE);
	} 
	}

	return EXIT_SUCCESS;
}
