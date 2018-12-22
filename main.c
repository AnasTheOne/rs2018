#include "head.h"

char* path;
int type;

void treat(char* chemin, int type){

    switch(type){
        case SPECIAL:
            files = realloc (files, sizeof (char*) * ++numFichier);
            char* fichier = malloc(strlen(chemin)+1);
            strcpy(fichier,chemin);
            files[numFichier-1] = fichier;
            break;
        case PRINT:
            if(*(chemin + strlen(chemin)-1) != 46){
                files = realloc (files, sizeof (char*) * ++numFichier);
                char* fichier = malloc(strlen(chemin)+1);
                strcpy(fichier,chemin);
                files[numFichier-1] = fichier;
            }
            break;
    }
}

int main(int argc, char **argv){
    numFichier = 0;
    files = NULL;
    affichages = NULL;
    path = malloc(1024);
    if(argc < 2) path =".";
    if(argc > 1) path = argv[1];
    type = SPECIAL;
    treat(path,type);
    if(*(path + strlen(path)-1) == '/' && strlen(path)>1) *(path + strlen(path)-1) = 0;
    type = PRINT;
    parcourir_repertoire(path, type, 0);
    files = realloc (files, sizeof (char*) * ++numFichier);
    files[numFichier-1] = NULL;
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            affichages = realloc (affichages, sizeof (char*) * (i+1));

            char* fichier = malloc(strlen(files[i])+1);
            strcpy(fichier,files[i]);
            affichages[i] = fichier;
        }
    }
    if (argc > 2){
        int i = 2;
        while(i < argc){
            if(argv[i][0] == '-' ){
                if(strcmp(argv[i]+1, "-print") == 0){
                    print();
                }
                else if(strcmp(argv[i]+1, "-name") == 0){
                    type = NAME;
                    char* name = argv[++i];
                    search(name);
                }
                else if(strcmp(argv[i]+1, "t") == 0){
                    type = T;
                    char* name = argv[++i];
                    chercheExpress(name);
                }
                else if (strcmp(argv[i]+1, "l") == 0){
                    type = L;
                    listing();
                }
                else if (strcmp(argv[i]+1, "i") == 0){
                    type = I;
                    image();
                }
                else if(strcmp(argv[i]+1, "-exec") == 0){
                    char* commande = argv[++i];
                    type = EXEC;
                    if(strlen(commande)>0){
                        executer(commande);
                    } else {
                        lireAffichage();
                    }
                }
                i++;
            }
        }
    }

    if(type != L && type != EXEC) lireAffichage();
    return 0;
}
