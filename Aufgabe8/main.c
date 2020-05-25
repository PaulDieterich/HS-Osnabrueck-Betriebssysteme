#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>



struct posix_header
{
    /* byte offset */
    char name[100];               /*   0 */
    char mode[8];                 /* 100 */
    char uid[8];                  /* 108 */
    char gid[8];                  /* 116 */
    char size[12];                /* 124 */
    char mtime[12];               /* 136 */
    char chksum[8];               /* 148 */
    char typeflag;                /* 156 */
    char linkname[100];           /* 157 */
    char magic[6];                /* 257 */
    char version[2];              /* 263 */
    char uname[32];               /* 265 */
    char gname[32];               /* 297 */
    char devmajor[8];             /* 329 */
    char devminor[8];             /* 337 */
    char prefix[155];             /* 345 */
    /* 500 */
};


int check_Ustar(int fd,char* buffer,long length){
    FILE* f = fdopen(fd,buffer);
    lseek(fd,0,SEEK_END);
    length = ftell(f);
    lseek(fd,0,SEEK_SET);
    buffer = (char*) malloc(length);
    return read(fd,buffer,length);
}

int main(int argc, char* argv[])
{
    char* buffer = 0;
    long length;
    //FILE* f
    int fd = open(argv[1], O_RDWR);
    if(fd != -1){
        if(check_Ustar(fd,buffer,length) != -1){
            printf("genug bytes ausgelesen.");
        }
    }
    else{
        fprintf(stderr,"File nicht gefunden");
    }

    return 0;
}