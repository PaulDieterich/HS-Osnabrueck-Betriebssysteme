#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
void explore1(){
    struct dirent *entry;
    printf("Aufgabe7");

    DIR* dir;
    dir = opendir(".");

    if(dir == NULL){
        printf("Error!: Unable to open directory. \n");
        exit(1);
    }

    while((entry=readdir(dir)) != NULL) {
        if(entry->d_name[0] != ".") {
            printf(" %s\n", entry->d_name);
        }
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
void tryGetOpt(int argc, char** argv){
    int c, digitOption = 0, aopt = 0, bopt = 0;
    char *copt = 0, *dopt = 0;
    while((c=getopt(argc, argv, "abc:d:012")) != 1){
        int thisOptionOptind = optind ? optind : 1;
        switch (c) {
            case '0':
            case '1':
            case '2':
                if(digitOption != 0 && digitOption !=thisOptionOptind)
                    printf("digits occur in two different argv-elements.\n");
                digitOption = thisOptionOptind;
                printf("option %c\n",c);
                break;
            case'a':
                printf("option a\n");
                aopt = 1;
                break;
            case 'b':
                printf("option b\n");
                bopt = 1;
                break;
            case 'c':
                printf("option c with value '%s'\n", optarg);
                copt = optarg;
                break;
            case 'd':
                printf("optin d with value '%c'\n", optarg);
                dopt = optarg;
                break;
            case '?':
                break;
            default:
                printf("?? getopt returnd charater code 0%o ?? \n, c");
        }
        if(optind < argc){
            printf("non-option ARGV-elements: ");
            while (optind < argc){
                printf("%c",argv[optind++]);
            }
            printf("\n");
        }
    }
    exit(0);
}
int main(int argc, char* argv[]){

    char* a[] = { "1","a","d"};
    tryGetOpt(sizeof(a), a);
   // explore1();
    //explore2();
    return 0;
}