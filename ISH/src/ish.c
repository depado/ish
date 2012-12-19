/* ish.c : ingesup shell */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define VERSION 2.1

#include "tools.h"

int RUN=1;
#define LBUF 512
char buf[LBUF];

void interrupt(int S)
{
     switch(S) {
     case SIGTERM :
          printf("Quelqu'un m'a envoye une signal %d !!\n",S);
          break;
     default :
          printf("Recu signal %d ??\n",S);
     }
     return;
}

/* traitement des redirections */
int Redir[3], Savstd[3];

void init_Redir(void)
{
int i;
    for (i=0; i<3; i++) {
       Redir[i]=-1;
       Savstd[i]=-1;
    }
}

void make_Redir(int fd, int std)
{
   Redir[std]=fd;
   Savstd[std]=dup(std);
   dup2(Redir[std],std);
}

void traite_Redir(char *s)
{
int fd;
char *w;
   /* si on trouve >toto */
   if ((w=strstr(s,">toto")) != NULL) {
       strncpy(w,"     ",5);
       fd=open("toto",O_RDWR|O_CREAT,0644);
       make_Redir(fd,1);
   }
}

void restaure_Redir(void)
{
int i;
   for (i=0; i<3; i++) {
      if (Redir[i] != -1) {
         close(Redir[1]);
         dup2(Savstd[1],1);
         close(Savstd[1]);
      }
   }
}


void  commandeExt(char * com, char *suite)
{
pid_t p;
int n, i, bg;
char **V, *pm, *su, *su2;
   /* cas normal : le shell fork un fils et attend sa fin */
   bg = isDerCar(suite,'&');
   if ((p=fork()) < 0) perror(com);
   else { /* gestion des deux processus */
     if (p == 0) { /* fils */
        n = nbMots(suite);
        printf("Il y a %d parametres !!\n", n);
        V = (char **)malloc((n+2)*sizeof(char*)); /* nb de param + nom commande + NULL */
        if (V == (char**)NULL) perror("malloc");
        else {
            V[0] = com;
            su=suite;
            for (i=1; i<n+1; i++) {
                pm = premierMot(su, &su2);
                V[i] = pm;
                su = su2;
            }
            V[i]=NULL;
            /* execution */
            execvp(com, V);
            perror(com);
        }
        exit(1);
     } else { /* le shell */
         if (bg) return;
         waitpid(p, NULL, 0);
     }
   }
}


int commandeInt(char * com, char *suite)
{
char *b, *param1, *suite2;
     if (strcmp(com, "exit") == 0) {
        RUN=0;
        return 1;
     }
     if (strcmp(com, "vers") == 0) {
        printf("ish version %g\n",VERSION);
        return 1;
     }
     if (strcmp(com, "pwd") == 0) {
        b=getcwd(NULL,0);
        printf("%s\n",b);
        free((void*)b);
        return 1;
     }
     if (strcmp(com, "cd") == 0) {
        if (strlen(suite) > 0) {
            /* recuperation du 1er parametre */
            param1 = premierMot(suite, &suite2);
            if (strlen(suite2) > 0) param1=NULL;
        } else { /* il faut recuperer le contenu de la variable HOME */
               param1 = getenv("HOME");
        }
        if (param1 == NULL)
            fprintf(stderr," cd n'accepte qu'un seul parametre !\n");
        else
            if (chdir(param1)) perror(param1);
        return 1;
     }
     return 0;
}

void execute(char *b)
{
char *com, *suite;
   /* traitement des redirections */
   init_Redir();
   traite_Redir(b);

   /* printf("Execution de <%s> \n", b); */
   com = premierMot(b, &suite);
#ifdef DEBUG
   if (com == NULL) printf("Commande absente !\n");
   else printf("La commande est <%s> et la suite <%s> !!\n", com, suite);
#endif
   if (com != NULL) {
         if (!commandeInt(com, suite)) 
              commandeExt(com, suite);
   }
   restaure_Redir();
}

int main (int N, char *P[])
{
int n;
   signal(SIGINT,SIG_IGN);
   signal(SIGTERM,interrupt);
   while (RUN) {
     printf("ish> ");  /* affichage du prompt */
     if ((n = lireLigne(buf, LBUF)) == -1) {
        fprintf(stderr,"La ligne est trop grande !!\n");
        continue;
     } 
     if (n) execute(buf);
   }
   printf("Au revoir !\n");
   return 0;
}


