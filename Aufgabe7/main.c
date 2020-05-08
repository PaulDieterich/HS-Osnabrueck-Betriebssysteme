#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void explore1(){
    struct dirent *sd;
    printf("Aufgabe7");

    DIR* dir;
    dir = opendir(".");

    if(dir == NULL){
        printf("Error!: Unable to open directory. \n");
        exit(1);
    }

    while((sd=readdir(dir)) != NULL) {
        printf(" %s\n", sd->d_name);
    }
    closedir(dir);
}
void explore2(){
    char *dir_name;
    DIR* dir;
    struct dirent *entry;
    struct stat info;
    dir_name = ".";
    dir = opendir(dir_name);
    if(!dir){ printf("Directory not found\n"); exit(1); }
    while((entry=readdir(dir)) != NULL){
        if(entry->d_name[0] != "."){
            char* path;
            snprintf(path, sizeof path, "%s/%s",dir_name,entry->d_name);
            printf("Entry = %s", path);
            stat(path, &info);
            if(S_ISDIR(info.st_mode)){
                explore2(path);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
    explore1();
    explore2();
    return 0;
}