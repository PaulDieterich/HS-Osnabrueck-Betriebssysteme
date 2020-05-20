//
// Created by pjd-vm on 19.05.20.
//
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>


using namespace std;

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