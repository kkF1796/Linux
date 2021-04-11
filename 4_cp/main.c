//include"option.h"
#include"listing.h"
#include"copie.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>


void main(int argc, char *argv[]){
	int i;     
	int flag = 0; 
	int opt;  
	// on regarde si -a fait parti des paramètres
	while((opt = getopt(argc, argv, "a:")) != -1) {
	switch(opt){
		case 'a':
			flag = 1;	
	}

	}
	// on traite le cas listing
	if (argc == 2){
		listing(argv[1]);
		
	}else{
		// on traite le cas copie
		// on copie pour un dossier
		if ((exists(argv[argc - 1]) && !is_regular_file(argv[argc - 1])) || 
			(!exists(argv[argc - 1]) && (argc > (3 + flag))) ||
  			( !exists(argv[argc - 1]) && (argc == (3 + flag)) && (!is_regular_file(argv[1 + flag])) ) ){//dossier

			if(!exists(argv[argc - 1])){
				//si le dossier de dest n'existe pas on le crée
				mkdir(argv[argc - 1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			}
			// copie des fichiers d'un répertoire à l'autre(de source dans destination)
			for (i=1 + flag; i<argc-1;i++){

				int file = 0;
				
				
				if((file=open(argv[i],O_RDONLY)) < -1)
					printf("can't read file");
	 
				struct stat fileStat;
				if(fstat(file,&fileStat) < 0)    
					printf("\n n'importe quoi,erreur de l'ouverture");
				char path[14000]="";
				strcpy(path,argv[argc-1]);
				strcat(path,"/");
				strcat(path,basename(argv[i]));				
				
				if( !exists(path)){	
					mkdir(path,fileStat.st_mode);
				}
				copie(argv[i],path,flag);
		
				
			}
		}
		// on copie pour le cas fichier vers un fichier
		else if((exists(argv[argc - 1]) && is_regular_file(argv[argc -1] ) && (argc == (3 + flag)) && is_regular_file(argv[1 + flag] )) || (!exists(argv[argc - 1]) && (argc == (3 + flag)) && is_regular_file(argv[1 + flag] ))){
			if(exists(argv[argc - 1])){//si le fichier existe déjà ,on regarde s'il faut l'ecraser ou pas
				if(ecraser(argv[1 + flag], argv[argc - 1])){
					fich_copie(argv[1+flag],argv[2 +flag]);
				}
				else if(flag){
					int file = 0;
					if((file=open(argv[1 + flag],O_RDONLY)) < -1)
					printf("\n Can't read file");
	 
					struct stat fileStat;
					if(fstat(file,&fileStat) < 0)    
						printf("\n n'importe quoi,erreur de l'ouverture");
					chmod(argv[argc - 1],fileStat.st_mode);//passage des droits s'il ne faut pas ecraser et -a est donné
				}
			}
			else{
				open(argv[2 + flag], O_RDWR | O_CREAT, S_IRUSR| S_IWUSR | S_IROTH);
				fich_copie(argv[1+flag],argv[2 +flag]);
			}
		}
		else{
			printf("\n ERREUR, FORMAT NON VALIDE !\n");
		}		
	}

        //return 0;
}
