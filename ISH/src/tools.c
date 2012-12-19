/* tools.c  */

#include <stdio.h>
#include <string.h>

#include "tools.h"

static char Sepa[]=" \t\n";

int isSepa(char c)
{
char *W;
    W=Sepa;
    while (*W != '\0') {
       if (*W == c) return 1;
       W++;
    }
    return 0;
}

char * premierMot(char *s, char **Suite)
{
char *pmot, *reste;
    pmot = s;
    *Suite = s + strlen(s);
    while (isSepa(*pmot)) pmot++;
    if (*pmot == '\0') return NULL;
    reste = pmot;
    while (!isSepa(*reste) && (*reste != '\0')) reste++;
    if (*reste == '\0') return pmot;
    *reste='\0';
    reste++;
    while (isSepa(*reste)) reste++;
    *Suite=reste;
    return pmot;
}

int nbMots(char *s)
{
int n = 0; /* nb de mots */
char *d, *f;
   d=s;
   f=s+strlen(s);
   while( d < f) {
      while (isSepa(*d)) d++;
      if (*d == '\0') break;
      /* j'ai trouve un mot */
      n++;
      while (!isSepa(*d) && (*d != '\0')) d++;
   }
   return n;
}

/* fct qui teste si le dernier cvaractere non separateur est C.
  Si oui il le supprime de la fin de la chaine */
int isDerCar(char *s, char C)
{
char *f;
   if (strlen(s) == 0) return 0;
   f=s+strlen(s)-1;
   while ((s<f) && (isSepa(*f))) f--;
   if (*f != C) return 0;
   *f='\0';
   return 1;
}


int lireLigne(char *B, int MAX)
{
char *t, *f;
    t=B;
    f=B+MAX;
    while (t < f) {
      if ((*t = (char)getchar()) == '\n') break;
      t++;
    }
    if (t == f) {  /* buffer trop petit */
       while ((char)getchar() != '\n');
       return -1;
    }
    *t = '\0';
    return (t-B);
}

