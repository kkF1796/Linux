
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

void listing(char rep[]);
int is_regular_file(const char *path);
void afficheTime(struct tm *tm);
void permission(struct stat fileStat);
void affichage(char rep[]);

