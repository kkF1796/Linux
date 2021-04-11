#include"listing.h"

// fonction qui détermine s'il s'agit d'un fichier 
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// fonction qui affiche la date dun fichier/répertoire
void afficheTime(struct tm *tm)
{
	char buf[200];
	printf(" ");
	strftime(buf, sizeof(buf), "%a", tm);
 	printf(" %s", buf);
	strftime(buf, sizeof(buf), "%b", tm);
 	printf(" %s", buf);
	strftime(buf, sizeof(buf), "%d", tm);
 	printf(" %s", buf);
	
	strftime(buf, sizeof(buf), "%H:%M:%S",tm);
   	printf(" %s", buf);
	strftime(buf, sizeof(buf), "%Y",tm);
   	printf(" %s", buf);   
	
}

// fonction qui affiche les permissions
void permission(struct stat fileStat)
{
	   printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	   printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	   printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	   printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	   printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	   printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	   printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	   printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	   printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	   printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");   
	
}

// fonction qui fait le listing d'un dossier
void listing(char rep[]) {

	affichage(rep);	
	
	if(!is_regular_file(rep)){
		DIR * dossier = opendir(rep);
		if(dossier != NULL){
			struct dirent * ent;
			//parcours des fichiers à l'interieur de rep
			while((ent = readdir(dossier)) != NULL ){
				if(!(ent->d_name[0] == '.')){		
				char inter[1000];
				strcpy(inter,rep);
				strcat(inter,"/");
				listing(strcat(inter,ent->d_name));
				
				
				}

			}
			closedir(dossier);
		
		}
	}

}

// fonction d'affichage pour les informations d'un dossier/fichier
void affichage(char rep[]){

	int file=0;
	struct tmp *tmp;
	if((file=open(rep,O_RDONLY)) < -1)
		printf("can't read file");
	 
	struct stat fileStat;
	if(fstat(file,&fileStat) < 0)    
		printf("\n n'importe quoi,erreur de l'ouverture");
		
	printf("\n");	
	permission(fileStat);
	printf("\t %ld",fileStat.st_size);

	struct tm *tm; 
	tm = localtime(&fileStat.st_mtime);
	afficheTime(tm);
	printf("  %s",rep);
}
