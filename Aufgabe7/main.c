#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
void myLs(int aFlag ,int lFlag,  int gFlag,  int oFlag,char *dirPath) {
    DIR* dir;
    struct dirent *entry; struct stat info; char *ctime();
    char* fullpath = realpath(dirPath,NULL);
    printf("%s",fullpath );
    printf("dirpath: %s\n ", dirPath);
    if ((dir = opendir(dirPath)) == NULL) {
        fprintf(stderr, "ls: can not open %s\n", dirPath);
        exit(EXIT_FAILURE);
    } else {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.' && !aFlag) {
                continue;
            }
            stat(fullpath, &info);
            if (lFlag || oFlag || gFlag) {
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
                printf("%4d",(int)info.st_nlink);
                if(!gFlag) {
                    struct passwd *usr = getpwuid(info.st_uid);
                    printf(" %8s", usr->pw_name);
                }
                if(!oFlag) {
                    struct group *gr = getgrgid(info.st_gid);
                    printf(" %8s", gr->gr_name);
                }

                printf("%8d",(int)info.st_size);
                printf(" %.12s ", 4+ctime(&info.st_mtime));
            }
            printf("  %s", entry->d_name);
            printf("  ");
            if (lFlag || oFlag || gFlag) {
                printf("\n");
            }
        }
    }
    printf("\n");
    closedir(dir);
}
int main(int argc, char* argv[]){
        char* dirname; //= "."; //verweisst auf das aktuelle verzeichnis wo das programm ausgefuehrt wird
        int opt; int count;
        int aFlag = 0;  int lFlag = 0;
        int gFlag = 0;  int oFlag = 0;
        while ((opt = getopt(argc, argv, "algo")) != -1) {
            switch (opt) {
                case 'a':
                    aFlag = 1;
                    break;
                case 'l':
                    lFlag = 1;
                    break;
                case 'g':
                    gFlag = 1;
                    break;
                case 'o':
                    oFlag = 1;
                    break;
            }
        }

        printf("aFlag=%d; lFlag=%d;  gFlag=%d; oFlag=%d; optind=%d\n",aFlag,lFlag,gFlag,oFlag, optind);
        printf("%d,%d\n",argc,optind);
        if (optind == argc) {
            dirname=".";
        }else{
            dirname = argv[optind];
        }
        printf("dirname: %s\n",dirname);
        myLs(aFlag,lFlag,gFlag,oFlag,dirname);
        return 0;

}
