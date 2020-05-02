/**
 * Betriebssysteme Praktikum Aufgabe6
 * Threading
 * Dieterich,Paul und Weimer,Jan
 *
 * Todo:
 *
  *
 */
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>



//Liste an URL's zum auslesen
typedef struct URLList{
    char* url;
    struct URLList* next;
} URLList;

//GLOBALE VARIABLEN
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
URLList* urlList = NULL;
URLList* currList = NULL;

//aus Beispieldatei Web_Request
//FILE* file, URLList* urlList, URLList* currList,char url[256]
void* readFile(void* arg) {
    char url[256];
    FILE* file = (FILE*)arg;
    if (!(file = fopen(file, "r"))) {
        perror("Steuerdatei konnte nicht geoeffnet werden");
    } else {
        while (fgets(url, sizeof(url),file)) { // erster parameter, wo es reinspeichern soll, zweiter parameter wie lang, dritter parameter die datei.
            if (strtok(url, "\r\n") == NULL) {
                continue;
            }
            if (urlList == NULL) {
                //urlList ist Leer
                urlList = malloc(sizeof(URLList));
                currList = urlList;
            } else {
                //ein neuer eintrag in der Liste
                currList->url = malloc(sizeof(URLList));
                currList->next = NULL;
            }
            currList->url = strdup(url);
            currList->next = NULL;
        }
        fclose(file);
    }

}
//TODO: Einlesen von Steuerdatei aus den Übergabeparametern
/*TODO: Ein Thread liest die Zeilen aus der Seuerdatei und schreibt einen entsprechenden Job
 *      in eine Queue mit einer konfoguierbaren Größe.
 */
/*TODO: Eine Konfigurierbare Anzahl von Threads liest
 * die Jobs aus der Queue und kontakiert die jeweiligen Web-Server
*/
void createThreadArray(pthread_t threadArray[]){
    for(int i = 0; i < sizeof(threadArray); i++){
        pthread_create(&threadArray[i], NULL,NULL,NULL);
    }
}
void joinThreadArray(pthread_t threadArray[]){
    for(int i = 0; i < sizeof(threadArray); i++){
        pthread_join(threadArray[i], NULL);
    }
}

int main(int argc, char *argv[]) {
    FILE* file = argv[1];
    int searchThreadAnzahl = argv[2];

    pthread_t threadArray[searchThreadAnzahl]; // create array of pthread's
    pthread_t readThread; //create readThread

    createThreadArray(searchThreadAnzahl, threadArray[]); //create vorbestimmte Anzahl an SearchThreads
    pthread_create(&readThread ,NULL,readFile,file);



    pthread_join(&readThread, NULL);
    joinThreadArray(threadArray[],NULL);


    return 0;
}
