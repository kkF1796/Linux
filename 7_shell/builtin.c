#include"builtin.h"
//fonction qui ser à changer le repertoire courant
void changeDir(char *path){
	int des;
	des = chdir(path);
	//verification de changement de repertoire
	if(des != 0){
	   die("Erreur dans le changement de répertoire.\n");
	}


}

