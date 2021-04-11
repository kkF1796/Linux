#include"task.h"
#include <fcntl.h>
//boolean qui indique si une tache de fond est en cours
int flag=0;

//pid de la tache de fond et principale
pid_t Background; 
pid_t foreground; 

//fonction qui indique si une tache de fond est en cours
int exists_background(){
	return flag;
}

//fonction pour la commande exit sur ligne de commande
void exit_process(){
	printf("Fermeture de mybash \n");
	kill(Background,SIGKILL);// tuer la tache de fond
	kill(foreground,SIGKILL);// tuer la tache principale
	exit(EXIT_SUCCESS);//sortir de mybash
}

//fonction handler de sigaction pour controle C
void  kill_foreground(int signal ){
		kill(foreground,SIGKILL);// tuer la tache principale
		printf("Unexpected termination\n");
}

//fonction handler pour sigaction de SIGHUP
void closeAll(int sig){
	int status;
	//redirection de sighup a tout processus
	kill(Background,SIGHUP);
	kill(foreground,SIGHUP);
	//nettoyage des zombies
	waitpid(Background,&status, WNOHANG);
	waitpid(foreground,&status, WNOHANG);
	//tuer mybash 
	kill(getpid(),SIGHUP);
	kill(getpid(),SIGKILL);

}

//fonction qui gere la tache principale
void taskManager(char *myargv[], int myargc){
	char filepath[10000];
	int status; 
	sprintf(filepath,"/bin/%s",myargv[0]);
	//création de fils pour gérer ...
	pid_t pid = fork();

	if(pid > 0){
		//Processus pere en cours
		foreground = pid;

		//gestion du sigal Control C(SIGINT)
		struct sigaction signalCrtlC;  
		sigemptyset(&signalCrtlC.sa_mask);
		signalCrtlC.sa_flags = SA_RESTART;
		signalCrtlC.sa_handler = kill_foreground;
		sigaction(SIGINT, &signalCrtlC, 0);
		//le pere(mybash) attend la terminaison de l'enfant et donc le terminal est bloqué 
		waitpid(pid,&status, 0);
	
		if (!status){
			printf("foreground job exited with code %d\n", status);
		}
		else{
			printf("foreground job exited \n");
		}
		
	}
	else if(pid == 0){
		//Processus fils en cours
		//le fils execute la commande
		if(execv(filepath, myargv) == -1){
			die("ERREUR DANS L'EXECUTION DU PROCESSUS\n");
		}
	}
	else{
		die("Erreur fork d'un processus\n");
	}


	
}



//fonction handler pour gestion de signal SIGCHILD
void  kill_background(int signal, siginfo_t *siginfo, void *context ){
	
		int status;
		//si le sigchild envoyé coresspond au pid du fils background alors on s'occupe de sa destruction avec waitpis
		if(Background == siginfo->si_pid){
			flag=0;//il n' y a plus de tache de fond
			waitpid(Background,&status, WNOHANG);
			if (!status){
				printf("Fin de tache de fond\n");
			}
			else{
				printf("tache de fond pas bien terminé \n");
			}
			
		}
		
	
	
}

//Fonction qui gère la tache de fond
void backgroundTask(char *myargv[], int myargc){
	char filepath[10000]; 
	sprintf(filepath,"/bin/%s",myargv[0]);

	pid_t pid = fork();
	flag=1;//une tache de fond commence à tourner

			if(pid == 0){
			//cas du processus fils
				int sortie = open("/dev/null",O_RDWR); // gère la redirection
				dup2(sortie, 0);
				dup2(sortie, 1);
				close(sortie);
		
				if(execvp(filepath, myargv) == -1){
					die("ERREUR DANS L'EXECUTION DU PROCESSUS\n");
				
				}
				
				
			}else if(pid >0){
			// cas du processus père
				
				Background = pid;
				
				//mise en place du signal sigchild
				struct sigaction signalfils; 
				sigfillset(&signalfils.sa_mask);
				signalfils.sa_flags = SA_SIGINFO ;
				signalfils.sa_handler = kill_background;
				sigaction(SIGCHLD, &signalfils, NULL);
				

			}



}



