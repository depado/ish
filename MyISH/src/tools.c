#include "tools.h"
/*
	Un processus = Une instance d'execution d'un programme
		→ Un segment TEXT (Code)
		→ Un segment DATA 
			→ Fonction du programme (Variables globales du programme)
			→ Fonction du système
				→ Environnement (Comme PATH)
				→ Table des fichiers ouverts (TFO)(Accès aux fichiers qu'on a ouvert)
				  Trois premiers éléments : [0]Stdin, [1]Stdout, [2]Stderr
				→ Table des signaux 

	Différences entres commandes internes et externes :
	→ Quand on execute une commande interne, pas besoin de faire un fork (pas de création de process)
*/

// IFS : Internal Field Separator
static char sepa[]=" \t \n";

int changeDirectory(char *path) {
	return chdir(path);
}

int isSeparator(char c) {
	char * w;
	w = sepa;
	while (*w != '\0') {
		if (*w == c) return 1;
		w++;
	}
	return 0;
}

void listRec(const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) {
        return;
    }
    if (!(entry = readdir(dir))) {
        return;
    }
    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            printf("%s%*s[%s]\n%s", blue, level*2, "", entry->d_name, none);
            listRec(path, level + 1);
        }
        else
            printf("%*s- %s\n", level*2, "", entry->d_name);
    } while ((entry = readdir(dir)));
    closedir(dir);
}

void list(const char *name)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) {
        return;
    }
    if (!(entry = readdir(dir))) {
        return;
    }
    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            printf("%s%s %s", boldblue, entry->d_name, none);
        }
        else
            printf("%s ", entry->d_name);
    } while ((entry = readdir(dir)));
    printf("\n");
    closedir(dir);
}

void vers() {
	printf("Version actuelle : %s\n", VERSION);
}

void pwd() {
	char *cd=getcwd(NULL, 0);
	printf("%s\n",cd);
	free((void*)cd);
}

char *firstWord(char *s, char **suite) {
	char *first, *reste;
	first = s;
	*suite = s + strlen(s);
	while (isSeparator(*first)) first++;
	if (*first == '\0') return NULL;
	reste = first;
	while (!isSeparator(*reste) && (*reste != '\0')) reste++;
	if (*reste == '\0') return first;
	*reste='\0';
	reste++;
	while (isSeparator(*reste)) reste++;
	*suite=reste;
	return first;

}


int readLine(char *buffer, int max) {
	char *t, *f;
	t = buffer;
	f = buffer+max;
	while (t < f) {
		if ((*t=(char)getchar()) == '\n') break;
		t++;
	}
	if (t == f) {
		while ((char)getchar() != '\n');
		return -1;
	}
	*t = '\0';
	return 0;
}