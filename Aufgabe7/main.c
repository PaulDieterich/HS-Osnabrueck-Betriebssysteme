#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>

void myLs(int aFlag ,int lFlag,  int gFlag,  int oFlag){
    char *dir_name;
    DIR* dir;
    struct dirent *entry;
    struct stat info;
    dir_name = ".";
    dir = opendir(dir_name);
    if(!dir){ printf("Directory not found\n"); exit(1); }

    while((entry=readdir(dir)) != NULL){
        if(entry->d_name[0] != "." || aFlag){
            stat(entry->d_name, &info);
            if(gFlag || lFlag || oFlag){
                printf(S_ISDIR(info.st_mode) ? "d" : "-");
                printf(info.st_mode & S_IRUSR ? "r" : "-");
                printf(info.st_mode & S_IWUSR ? "w" : "-");
                printf(info.st_mode & S_IXUSR ? "x" : "-");
                printf(info.st_mode & S_IRGRP ? "r" : "-");
                printf(info.st_mode & S_IWGRP ? "w" : "-");
                printf(info.st_mode & S_IXGRP ? "x" : "-");
                printf(info.st_mode & S_IROTH ? "r" : "-");
                printf(info.st_mode & S_IWOTH ? "w" : "-");
                printf(info.st_mode & S_IXOTH ? "x" : "-");
            }
            printf(" %lu",info.st_nlink);
            if(!oFlag) {
                struct passwd* usr  = getpwuid(info.st_uid);
                printf(" %s", usr->pw_name);
            }
            if(!gFlag) {
                struct group *gr = getgrgid(info.st_gid);
                printf(" %s", gr->gr_name);
            }
            printf(" %ld", info.st_size);

            //timestamp output
            char* outstr[64];struct tm *tmp;
            //strftime(timestamp,sizeof(timestamp),"%b %d %H:%M",tmp);
            //printf(" %s", *timestamp);
            
            
              if (strftime(outstr, sizeof(outstr), entry->d_name, tmp) == 0) {
                    fprintf(stderr, "strftime returned 0");
                   // exit(EXIT_FAILURE);
                }
            printf(" %sc", outstr);



            printf(" %s\n", entry->d_name);
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
        char* dirname = "."; //verweisst auf das aktuelle verzeichnis wo das programm ausgefuehrt wird
        int flags, optopt, opt,optind;
        int nsecs, aFlag, lFlag, gFlag, oFlag;

        //  -a, --all   do not ignore entries starting with .
        //  -g          like -l, but do not list owner
        //  -l          use a long listing format
        //  -o          like -l, but do not list group information

    nsecs = 0; aFlag = 0; 
        lFlag = 0; gFlag = 0;
        oFlag = 0; flags = 0;
        while ((opt = getopt(argc, argv, "algo:")) != -1) {
            switch (opt) {
                case 'a':
                    aFlag = 1;
                    break;
                case 'l':
                    lFlag = 1;
                    break;
                case 'p':
                    gFlag = 1;
                    break;
                case 'o':
                    nsecs = atoi(optarg);
                    oFlag = 1;
                    break;
                default:


                    fprintf(stderr, "Unkonwn option -%c\n",optopt);
                    exit(EXIT_FAILURE);

            }
        }

        printf("flags=%d; aFlag=%d; lFlag=%d;  gFlag=%d; oFlag=%d; nsecs=%d; optind=%d\n",
               flags, aFlag,lFlag,gFlag,oFlag,nsecs, optind);

        if (optind >= argc) {
            fprintf(stderr, "Expected argument after options\n");
            exit(EXIT_FAILURE);
        }

        printf("name argument = %s\n", argv[optind]);

        /* Other code omitted */


        myLs(aFlag,lFlag,gFlag,oFlag);

        return 0;

}