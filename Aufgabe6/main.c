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
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

//Liste an URL's zum auslesen
typedef struct URLList{
    char* url;
    struct URLList* next;
} URLList;

int checkIfFileReadabill(char* datei, FILE* file){
    //modes: "r" = read: Open file for input operations. The file must exit.
    if(!(file = fopen(datei, "r"))){
     perror("Steuerdatei konnte nicht geoeffnet werden");
     return 0;
    }
    return 1;
}
//aus Beispieldatei Web_Request
void readFile(FILE* file, URLList* urlList, URLList* currList,char url[256]){
    while(fgets(url, sizeof(url), file)){

        if(strtok(url, "\r\n") == NULL){
            continue;
        }
        if(urlList == NULL){
            //urlList ist Leer
            urlList = malloc(sizeof(URLList));
            currList = urlList;
        }
        else{
            //ein neuer eintrag in der Liste
            currList->url = malloc(sizeof(URLList));
            currList->next = NULL;
        }
        currList->url = strdup(url);
        currList->next = NULL;
    }
}
void* search(void *arg){
    URLList* list = (URLList*) arg;
    
}
//TODO: Einlesen von Steuerdatei aus den Übergabeparametern
/*TODO: Ein Thread liest die Zeilen aus der Seuerdatei und schreibt einen entsprechenden Job
 *      in eine Queue mit einer konfoguierbaren Größe.
 */
/*TODO: Eine Konfigurierbare Anzahl von Threads liest
 * die Jobs aus der Queue und kontakiert die jeweiligen Web-Server
 */

int main(int argc, char *argv[]) {
    char* steuerdatei = argv[1];
    int threadAnzahl = argv[2]; //fuer variable anzahl an threads
    FILE* file; char url[256];
    URLList* urlList = NULL;
    URLList* currList = NULL;
    pthread_t searchThread1, searchThread2;
    if(checkIfFileReadabill(steuerdatei,file)){
        readFile(file,urlList,currList,url);
        fclose(file); // datei wird geschlossen, sie wurde in checkIfFileReadebill geöffnet
    }



    return 0;
}
