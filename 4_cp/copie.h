#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include"listing.h"

void fich_copie(char source[], char destination[]);
int exists(const char * filename);
void copie(char source[],char destination[],int flag);
int ecraser(char srcPath[], char destPath[]);

