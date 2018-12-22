#include "head.h"
#include <grp.h>
#include <pwd.h>
#include <time.h>


char** split(char* chaine){
    char ** res  = NULL;
    char *  p    = strtok (chaine, " ");
    int n = 0;
    while (p) {
        res = realloc (res, sizeof (char*) * ++n);
        res[n-1] = p;
        p = strtok (NULL, " ");
    }
    res = realloc (res, sizeof (char*) * (n+1));
    res[n] = 0;
    return res;

}

int nom_correspond(const char *chemin, const char *nom){
    char *fileName;
    fileName = strrchr(chemin, '/')+1;
    if (strstr(nom,"*") != NULL){
        char *fileExtension; char *other;
        if (strstr(fileName,".") != NULL && strcmp(fileName,".") != 0){
            other = strrchr(fileName, '.');
            fileExtension =strchr(nom, '*')+1;
            if (strstr(fileExtension,other) != NULL) return 1;
        }
    }else return !strcmp(fileName,nom);
    return 0;
}

void execCmd(char ** cmd){
    pid_t pid;

    if(!(pid =fork())){
        execvp(cmd[0], cmd);
    }else{
        int statut;
        waitpid(pid, &statut, 0);
    }
}
pid_t execu(char ** cmd) {
    int fd[2];
    char character;
    pid_t pid;
    pipe(fd);
    int statut;

    if ((pid =fork())) {
        close(fd[0]);
        dup2(fd[1],1);
        //printf("%s\n",cmd[0]);
        execvp(cmd[0], cmd);

    } else {

        close(fd[1]);
        while(read(fd[0],&character,1)){
            printf("%c",character);
        }
        close(fd[0]);
        waitpid(execu(cmd), &statut, 0);
    }
    return pid;
}
void executer(char* commande){
    char** cmd = split(commande);
    int k = 0;
    for (; cmd[k] != NULL; ++k) {}
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            cmd[k] = files[i];
            cmd = realloc (cmd, sizeof (char*) * (k+2));
            cmd[k+1] = 0;
            execCmd(cmd);
        }
    }

}


void lireAffichage (){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL) printf("%s\n",affichages[i]);
    }
}

void print(){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            char* fichier = malloc(strlen(files[i]) + strlen(affichages[i])+1);
            strncpy(fichier,affichages[i],strlen(affichages[i]));
            strcpy(fichier+strlen(affichages[i]),"\n");
            strcpy(fichier + strlen(affichages[i])+1 ,files[i]);
            affichages[i] = fichier;
        }
    }
}


void search(char* name){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            if(strcmp(files[i],"."))
                if(!nom_correspond(files[i],name)) files[i] = NULL;
        }
    }

}

int expressDansFichier(char* fichier,char* express){
    int fp = open(fichier, O_RDONLY);
    int n = strlen(express);
    char buf[n];
    while (read(fp,buf,n)==n){
        lseek(fp,1-n,SEEK_CUR);
        if (!strcmp(buf,express)) return 1;
    }
    close(fp);
    return 0;

}


void chercheExpress(char* express){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            if(!expressDansFichier(files[i],express)) files[i] = NULL;
        }
    }
}

int isImage(char* fichier){
    void *handle;
    void* (*magic_open)(int);
    void (*magic_close)(void*);
    char* (*magic_file)(void*,char*);
    int (*magic_load)(void*,char*);
    handle = dlopen("libmagic.so", RTLD_LAZY | RTLD_DEEPBIND);
    magic_open = (void* (*)(int)) dlsym(handle, "magic_open");
    magic_close = (void (*)(void*)) dlsym(handle, "magic_close");
    magic_file = (char* (*)(void*,char*)) dlsym(handle, "magic_file");
    magic_load = (int (*)(void*,char*)) dlsym(handle, "magic_load");
    dlsym(handle,"MAGIC_MIME_TYPE");
    void* cookie = (*magic_open)(16);
    (*magic_load)(cookie,NULL);
    char* info = (*magic_file)(cookie, fichier);
    (*magic_close)(0);
    dlclose(handle);
    return strncmp(info,"image/",6);
}

void image(){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            if(isImage(files[i])) files[i] = NULL;
        }
    }

}

/*
struct stat fileStat;
char* ls_l(const char *chemin){
    char* fichier = malloc(200);
    struct group *grp;
    struct passwd *pwd;
    int i = 0;
    stat(chemin,&fileStat);
    strcpy(fichier,(S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IRUSR) ? "r" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IWUSR) ? "w" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IXUSR) ? "x" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IRGRP) ? "r" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IWGRP) ? "w" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IXGRP) ? "x" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IROTH) ? "r" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IWOTH) ? "w" : "-");
    strcpy(fichier + (++i),(fileStat.st_mode & S_IXOTH) ? "x" : "-");
    strcpy(fichier + (++i),".");
    sprintf(fichier+i+1," %ld",fileStat.st_nlink);
    i = i +3 +(int) fileStat.st_nlink / 10;
    //printf("%ld\t%d\n",fileStat.st_nlink,(int) fileStat.st_nlink / 10);
    pwd = getpwuid(fileStat.st_uid);
    sprintf(fichier+i," %s",pwd->pw_name);
    i = i + 1+strlen(pwd->pw_name);
    grp = getgrgid(fileStat.st_gid);
    sprintf(fichier + i," %s",grp->gr_name);i = i + strlen(grp->gr_name)+1;
    //strcpy(fichier + (i = i + strlen(grp->gr_name)),grp->gr_name);
    sprintf(fichier + i," %ld",fileStat.st_size);i = i +2 + (int)(fileStat.st_size/10.0);
    //strcpy(fichier + (i = i + 1+ (int)fileStat.st_size/10),fileStat.st_size);
    sprintf(fichier + i," %.12s",4+ctime(&fileStat.st_mtime)); i = i + 13;
    //printf(" %s",ctime(&fileStat.st_ctimespec.tv_nsec));//owner name group name
    //printf(" %.12s",4+ctime(&fileStat.st_mtime));
    sprintf(fichier + i," %s",chemin);
    //strcpy(fichier +i,chemin);
    return fichier;

}
*/
struct stat fileStat;
void ls_l(const char *chemin){
    if(stat(chemin,&fileStat) < 0)
        return;
    struct group *grp;
    struct passwd *pwd;
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    //printf("\n\n");

    printf(". %ld",fileStat.st_nlink);

    pwd = getpwuid(fileStat.st_uid);
    printf(" %s",pwd->pw_name);
    grp = getgrgid(fileStat.st_gid);
    printf(" %s",grp->gr_name);

    printf(" %ld",fileStat.st_size);
    //printf(" %s",ctime(&fileStat.st_ctimespec.tv_nsec));//owner name group name
    printf(" %.12s",4+ctime(&fileStat.st_mtime));
    printf("%s\n",chemin);

}

void listing(){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            ls_l(files[i]);
        }
    }
}
