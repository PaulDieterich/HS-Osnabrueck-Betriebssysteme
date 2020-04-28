#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
void* myTurn(void * arg){
    int *intr = (int*)arg;
    for(int i = 0; i < 8; i++) {
        sleep(1);
        printf("My Turn! %d %d\n",i,*intr);
        (*intr)++;
    }
    return NULL;
}
void yourTurn(){
    for(int i = 0; i < 5; i++) {
        sleep(1);
        printf("Your Turn! %d\n",i);
    }
}
int main() {
    int v = 5;
    pthread_t newtread;
    pthread_create(&newtread, NULL,myTurn,&v);
    yourTurn();
    pthread_join(&newtread,NULL);
    printf("thread's done: v=%d", v);
    return 0;
}
