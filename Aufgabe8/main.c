#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define TIME_SIZE 32
#define NUMBER_BYTES 512
#define CHECK_USTAR "ustar"
#define USTAR_REGTYPE '0'            /* regular file */
#define USTAR_AREGTYPE '\0'          /* regular file */
#define USTAR_LNKTYPE '1'            /* Hard link */
#define USTAR_SYMTYPE '2'            /* Symbolic link */
#define USTAR_CHRTYPE '3'            /* character special */
#define USTAR_BLKTYPE '4'            /* block special */
#define USTAR_DIRTYPE '5'            /* directory */
#define USTAR_FIFOTYPE '6'           /* FIFO special */
#define USTAR_CONTTYPE '7'           /* Contiguous file  */

struct posix_header {
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
} buffer;

int getSize (char * s){
    char * t;
    for (t = s; *t != '\0'; t++);
    return t - s;
}
long power(long a, long n){
  long  x=1;
  for(int i = 0; i < n; i++){
    x *= a;
  }
  return x;
}

long octalToDecimal(char* num){
  long octalnum = atol(num);
  long decimalnum = 0, i = 0;

  while(octalnum != 0){
    decimalnum += (octalnum%10) * power(8,i);
    ++i;
    octalnum /= 10;
  }

  i = 1;
  return decimalnum;
}
//https://stackoverflow.com/questions/18858115/c-long-long-to-char-conversion-function-in-embedded-system
//von Charlie Burns
// wurde von mir erweitert
char* itoa(int val, int base){
    static char buf[8] = {0};
    int i = 6;

    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef "[val % base];

    while(i < sizeof(buf)){
      buf[i] = ' ';
      --i;
    }
    return &buf[i+1];
}
void stdNormalOutput(char* text){
    write(STDOUT_FILENO, text, getSize(text));
}
void stdErrorOutput(char* text){
   write(STDERR_FILENO, text, getSize(text));
}

int checkUstar() {
    for (int i = 0; i < sizeof(buffer.magic); i++) {
        if (buffer.magic[i] != CHECK_USTAR[i]) {
            return 0;
        }
    }
    return 1;
}

void ownerACL(char* strMode){
 if (buffer.mode[4] == '4' || buffer.mode[4] == '5' || buffer.mode[4] == '6' || buffer.mode[4] == '7') {
        strMode[1] = 'r';
    } else {
        strMode[1] = '-';
        }

    if (buffer.mode[4] == '2' || buffer.mode[4] == '3' || buffer.mode[4] == '6' || buffer.mode[4] == '7') {
        strMode[2] = 'w';
    } else {
        strMode[2] = '-';
    }

    if (buffer.mode[4] == '1' || buffer.mode[4] == '3' || buffer.mode[4] == '5' || buffer.mode[4] == '7') {
        strMode[3] = 'x';
    } else {
        strMode[3] = '-';
    }
}

void groupACL(char* strMode){
 if (buffer.mode[5] == '4' || buffer.mode[5] == '5' || buffer.mode[5] == '6' || buffer.mode[5] == '7') {
        strMode[4] = 'r';
    } else {
        strMode[4] = '-';
    }

    if (buffer.mode[5] == '2' || buffer.mode[5] == '3' || buffer.mode[5] == '6' || buffer.mode[5] == '7') {
        strMode[5] = 'w';
    } else {
        strMode[5] = '-';
    }

    if (buffer.mode[5] == '1' || buffer.mode[5] == '3' || buffer.mode[5] == '5' || buffer.mode[5] == '7') {
        strMode[6] = 'x';
    } else {
        strMode[6] = '-';
    }
}

void otherACL(char* strMode){
 if (buffer.mode[6] == '4' || buffer.mode[6] == '5' || buffer.mode[6] == '6' || buffer.mode[6] == '7') {
        strMode[7] = 'r';
    } else {
        strMode[7] = '-';
    }

    if (buffer.mode[6] == '2' || buffer.mode[6] == '3' || buffer.mode[6] == '6' || buffer.mode[6] == '7') {
        strMode[8] = 'w';
    } else {
        strMode[8] = '-';
    }

    if (buffer.mode[6] == '1' || buffer.mode[6] == '3' || buffer.mode[6] == '5' || buffer.mode[6] == '7') {
        strMode[9] = 'x';
    } else {
        strMode[9] = '-';
    }
}
void formatModeBits(char *strMode) {
    switch (buffer.typeflag){
    case USTAR_DIRTYPE: strMode[0]  = 'd'; break;
    case USTAR_BLKTYPE: strMode[0]  = 'b'; break;
    case USTAR_CHRTYPE: strMode[0]  = 'c'; break;
    case USTAR_LNKTYPE: strMode[0]  = 'l'; break;
    case USTAR_FIFOTYPE: strMode[0] = 'p'; break;
    case USTAR_REGTYPE: strMode[0] = '-'; break;
    default: strMode[0] = '?';
    }
    ownerACL(strMode);
    groupACL(strMode);
    ownerACL(strMode);
    strMode[10] = '\0';
}
int main(int argc, char *argv[]) {
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        stdErrorOutput("Datei kann nicht geöffnet werden.");
    } else  {
        read(fd, &buffer, NUMBER_BYTES);
    }
	  if (checkUstar() == 1){
      lseek(fd, 0, SEEK_SET);
      while (read(fd, &buffer, NUMBER_BYTES) > 0) {
          if (checkUstar() == 1) {
              char strMode[10];
              formatModeBits(strMode);
              stdNormalOutput(strMode);
              stdNormalOutput(" ");
              stdNormalOutput(buffer.uname);
              stdNormalOutput( "/");
              stdNormalOutput(buffer.gname);
              stdNormalOutput( " ");
              stdNormalOutput(itoa(octalToDecimal(buffer.size),10));
              stdNormalOutput( " ");
              time_t time = octalToDecimal(buffer.mtime);
              char timeStr[TIME_SIZE];
              strftime(timeStr, TIME_SIZE, "%Y-%m-%d %H:%M", localtime(&time));
              stdNormalOutput(timeStr);
              stdNormalOutput(" ");
              stdNormalOutput(buffer.name);
              stdNormalOutput("\n");
          }
      }
    }
    close(fd);
    return 0;
}
