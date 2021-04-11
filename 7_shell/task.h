#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

void taskManager(char *myargv[], int myargc);
void  kill_foreground(int signal);
void  kill_background(int signal, siginfo_t *siginfo, void *context );
void closeAll(int sig);
void exit_process();
int exists_background();
