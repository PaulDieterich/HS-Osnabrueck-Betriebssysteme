#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * t - list content
 * v - Verbosely show the .tar file progress
 * f - filename type of the archive file
 *
 * - rechte  - owner/group - groeße - datum urhzeit - datei pfad
 *
 */

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
void* readByteWise(int filename, int from, int bytes){
    lseek(filename, from, SEEK_SET);
    void* buf = malloc(bytes*sizeof(char));
    return read(filename, buf,bytes); //return buf
}

int getSize(char* string){
    char* c;
    for(c = string; *c != '\0';c++);
    return c - string;
}
int stdOutput(char* text){ //standat output zur text ausgabe
    write(STDOUT_FILENO,text,getSize(text));
}
int errOutput(char* text){ //error output zur error ausgabe
    write(STDERR_FILENO,text,getSize(text));
}
int check_Ustar(char* filename, void *buf, long offset) {

    int fdes = open(filename, O_RDONLY);
    if (lseek(fdes, 257, SEEK_SET) < 0) {


        buf = malloc(6 * sizeof(char));
        read(fdes, buf, 6);
        if (*((int *) buf) != *((int *) "ustar")) {
            stdOutput(filename);// << endl;
            stdOutput(" is not an Ustar File");// << endl;
            return 0;
        }
        return 1;
    }
}


/*   FILE* f = fdopen(fd,buffer);
    lseek(fd,0,SEEK_END);
    length = ftell(f);
    lseek(fd,0,SEEK_SET);
    buffer = (char*) malloc(length);
    return read(fd,buffer,length);
    }
*/
int main(int argc, char* argv[]){
    void* buf;
    long offset = 0;
  for(int i = 1; i < argc; i++){
      if(check_Ustar(argv[i],buf,offset)){ // check_Ustart gibt 1(True) und 0(False)
          continue;
      }
  }

    return 0;
}