#include "head.h"

void parcourir_repertoire(const char* chemin,int type, int suivre_lien){
    DIR *dir = opendir(chemin);
    struct dirent *entry;
    struct stat dir_stat;
    if (! dir)
    {
        perror(chemin);
        return;
    }
    //treat(chemin, exp);
    while((entry = readdir(dir)) != NULL){//traiter_fichier(buff, exp, suivre_lien);
        char buff[strlen(chemin)+strlen(entry->d_name)+2];
        sprintf(buff,"%s/%s",chemin,entry->d_name);
        suivre_lien ? stat(buff, &dir_stat) : lstat(buff,&dir_stat);//pourquoii???????
        //printf("%s\t%s\n",chemin,path);

        if(strcmp(entry->d_name,"..") != 0){
            if(((dir_stat.st_mode & S_IFMT) == S_IFDIR)  && strcmp(entry->d_name, ".") != 0){

                treat(buff, type);
                parcourir_repertoire(buff, type, suivre_lien);
            }
            else
                treat(buff, type);

        }
    }
    closedir(dir);
}