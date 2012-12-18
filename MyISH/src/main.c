/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "tools.h"
#define LBUF 50
#define VERSION "1.0"
#define ARGLIMIT 10
#define DEBUG // Enable debug mode

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");
}


int RUN=1;
char buf[LBUF];

int commandeInt(char *com, char *suite) {

	int argnbr = 0;
	if (strcmp(suite, "")) {
		char *pch;
		pch = strtok (suite," ");
		while (pch != NULL)
	  	{
	    	argnbr++;
	    	pch = strtok (NULL, " ");
	  	}
  	}
	char **argarray=(char **)malloc(((argnbr+1)*sizeof(char*)));
	char *pch;
	int i=0;
	pch = strtok (suite," ");
	while (pch != NULL) {
		argarray[i]=pch;
		i++;
		pch = strtok (NULL, " ");
	}

	if (strcmp(com, "exit") == 0) {
		if (argnbr > 0) {
			printf("No arguments needed : Exiting.\n");
		}
		RUN=0;
		return 1;
	}
	if (strcmp(com, "vers") == 0) {
		vers();
		return 1;
	}
	if (strcmp(com, "pwd") == 0) {
		pwd();
		return 1;
	}
	if (strcmp(com, "cd") == 0) {
		if(changeDirectory(suite)) {
			printf("Can't change directory.\n");
		}
		return 1;
	}
	if (strcmp(com, "ls") == 0) {
		char *arg;
		if (!strcmp(suite,"")) {
			arg=".";
		}
		else arg=suite;
		if (!strcmp(argarray[0], "-r")) {
			if (argnbr == 2) {
				listRec(argarray[1],0);
			}
			else listRec(".",0);
		}
		else list(arg);
		return 1;
	}
	if (strcmp(com, "extern") == 0) {
		if (!strcmp(suite,"")) {
			printf("Please specify an external command\n");
			return 0;
		}
		system(suite);
		return 1;
	}
	#ifdef DEBUG
	printf("Can't find internal command.\n");
	printf("Executing external command.\n");
	#endif
	execvp(com, argarray);
	return 0;
}

void execute(char *b) {
	char *com, *suite;
	com = firstWord(b, &suite);
	#ifdef DEBUG
	printf("Execution de <%s>\n", b);
	printf("La commande est <%s> et la suite <%s>\n", com, suite);
	#endif
	if (com == NULL) return;
	if (commandeInt(com, suite)) return;
	// execvp(com, suite);
	return;
}

int main(int argc, char const *argv[])
{
	//initscr(); // For ncurses implementation
	int n;
	char *hostname = malloc(10 * sizeof(char));
	gethostname(hostname, 10);
	
	while(RUN) {
		char *cd=getcwd(NULL, 0);
		signal(SIGINT, SIG_IGN);
		signal(4, SIG_IGN);
		printf("[%s][%s]\n> ",hostname,cd);
		if ((n = readLine(buf, LBUF)) == -1) {
			fprintf(stderr, "La ligne est trop grande.\n");
			continue;
		}
		execute(buf);
		if (NULL != cd) {
    		free((void*)cd);
    		cd = NULL; 
 		}
	}
	if (NULL != hostname) {
    	free((void*)hostname);
    	hostname = NULL; 
 	}
	return 0;
}