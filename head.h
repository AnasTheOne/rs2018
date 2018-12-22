#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <wait.h>

enum
{
    NAME,
    EXEC,
    I,
    L,
    T,
    SPECIAL,
    PRINT,
};

char** files;
char** affichages;
int numFichier;

void treat(char* chemin, int type);
void parcourir_repertoire(const char* chemin,int type, int suivre_lien);
void executer(char* commande);
void lireAffichage();
void search(char* name);
void chercheExpress(char* name);
void print();
void image();
void listing();