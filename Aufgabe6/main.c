/**************************************************************
*Aufgabe 6 - Praktikum Betriebsysteme
* Paul Dieterich, Jan Weimer
*
*
* To Do:
* - ...
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void *entry_point(void *value) {
    printf("hello from the second thread :)\n");

    int* num = (int*) value;

    printf("the value of value is %d\n", *num);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thread;

    printf("hello from the first thread :D\n");

    int num = 1;

    pthread_create(&thread, NULL, entry_point, &num);

    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}
