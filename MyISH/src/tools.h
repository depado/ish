/* tools.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ncurses.h>

/* defines */
#define VERSION "1.0"
#define blue "\033[34m"
#define boldblue "\033[1;34m"
#define none "\033[0m"

/* prototypes */ 
extern int readLine(char *buffer, int max);
extern int isSeparator(char c);
extern char *firstWord(char *s, char **suite);
extern void vers();
extern void pwd();
extern void listRec(const char *name, int level); // ls -r implementation
extern void list(const char *name); // ls 
extern int changeDirectory(char *path);