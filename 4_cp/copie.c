#include"copie.h"

// fonction qui vérifie l'existence d'un fichier ou dossier
int exists(const char * filename){
	FILE *file;
	if (file = fopen(filename, "r")){
		fclose(file);
		return 1;	
	}
	return 0;
}

// fonction pour copier le contenu d'un fichier dans un autre
void fich_copie(char source[], char destination[]){
	FILE *src,*dest;

	int Nblus;
	char buffer[1000] = ""; 
	src = fopen(source, "rb");
	dest=fopen(destination, "wb");
	
	if (/*src &&*/ exists(destination))
	{
		
		while( (Nblus = fread(buffer,1,512, src))!= 0){		
			fwrite(buffer, 1 , Nblus, dest);
		}

		fclose(dest);
		fclose(src);
	}else{
		printf("erreur dans l'ouverture des fichiers\n");
	}

}

// fonction récursive pour la copie du contenu d'un dossier
void copie(char source[],char destination[],int flag){

		int file=0;
		DIR * src = opendir(source);
			struct dirent * f;
			//on parcourt les fichiers à l'interieur de notre dossier
			while((f = readdir(src)) != NULL ){
				if(!(f->d_name[0] == '.')){		
				char srcPath[1000];
				strcpy(srcPath,source);
				strcat(srcPath,"/");
				strcat(srcPath,f->d_name);
				
				char destPath[1000];
				strcpy(destPath,destination);
				strcat(destPath,"/");
				strcat(destPath,f->d_name);//création du chemin du dossier destination
				
				
				if((file=open(srcPath,O_RDONLY)) < -1)
					printf("can't read file");
	 
				struct stat fileStat;
				if(fstat(file,&fileStat) < 0)    
					printf("\n n'importe quoi,erreur de l'ouverture");

			
				if(exists(destPath) ){
					if(is_regular_file(srcPath)){
						
						if(ecraser(srcPath, destPath)){
							fich_copie(srcPath,destPath);
						}
						else if(flag){
							chmod(destPath,fileStat.st_mode);
						}
					}
					else{
						if(flag){
							chmod(destPath,fileStat.st_mode);
						}
						copie(srcPath,destPath,flag);
					}
					
				}
				else{
					if(is_regular_file(srcPath)){
						
						open(destPath, O_RDWR | O_CREAT, fileStat.st_mode );
						fich_copie(srcPath, destPath);
					}
					else{
						mkdir(destPath, fileStat.st_mode);
						copie(srcPath,destPath,flag);
						
					}
		
					
					
				}
			
			
				}

			}
			closedir(src);

}

// fonction qui retourne un bool pour savoir s'il faut écraser un fichier ou non
int ecraser(char srcPath[], char destPath[]){
	int file=0;


	if((file=open(destPath,O_RDONLY)) < -1)
			printf("can't read file");
	 
	struct stat fileStatD;
	if(fstat(file,&fileStatD) < 0)    
		printf("\n n'importe quoi,erreur de l'ouverture");



	if((file=open(srcPath,O_RDONLY)) < -1)
			printf("can't read file");
	 
	struct stat fileStatS;
	if(fstat(file,&fileStatS) < 0)    
		printf("\n n'importe quoi,erreur de l'ouverture");
	
	

	struct tm *tm; 
	tm = localtime(&fileStatS.st_mtime);
	tm = localtime(&fileStatD.st_mtime);
	// les deux conditions à verifier
	if((fileStatS.st_size != fileStatD.st_size) || (fileStatS.st_mtime > fileStatD.st_mtime))
		return 1;
	return 0;

}










