#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** environment;
void sayHello(){
    printf("Welcome to my own shell\n"); //Begruessung
}

/*
getInput
Dient zur Nutzereingabe
Rueckgabewert: int
Uebergabewert: char* str
Bedingung: char* str muss laenger als 0 sein.
*/
int getInput(char* str){
    int max = 1000;
    fgets(str, max,stdin);
    if(strlen(str) != 0){
        return 0;
    }else{
        return 1;
    }
}
/*
printDirectory
Dient zur Ausgabe vom PS1
Uebergabewert: char* username
username wird ermittelt aus getenv("USER") in Main(){}
*/
void printDirectory(char* username){

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s@ -%s ]" ,username,cwd);

}
/*
createProcess
Dient zur Kindprozesserstellung
Uebergabewert: char** a

*/
void createProcess(char** a){
    /* TODO */
    pid_t pid = fork();
    if(pid < 0){
        printf(" Konnte nicht erstellt werden\n");
    } else if(pid == 0){
        printf(" Kind process wurde erzeugt\n");
        if(execvp(a[0], a) < 0)
            printf(" execvp ausgefuerung faild\n");
        exit(0);
    } else{
        wait(NULL);
    }
}

void createProcessPiped(char** args, char** argsPiped){

    int pipefd[2];
    pid_t p;
    //hier werden die beiden Deskriptoren mit pipe() erzeugt
    if(pipe(pipefd) < 0){
        printf("\nPipe konnte nicht erstellt werden");
        return;
    }
    p = fork();
    if(p < 0){
        printf("\nKindprozess konnte nicht erstellt werden");
        return;
    }

    if(p == 0){
        //erstes Kindprozess executing
        //wird in das zweite Argument eingelesen (dient als Eingabestrom
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if(execvp(args[0], args) < 0) {
            printf("\nKonnte das erste Command nichts ausführen");
            exit(0);
        }
    } else {
        //Elternprozess executing
        p = fork();

        if(p < 0){
            printf("\nKindprozess konnte nicht erstellt werden");
            return;
        }

        //Kind 2 executing
        if(p == 0){
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if(execvp(argsPiped[0],argsPiped) < 0){
                printf("\nKonnte das zweite Command nicht ausführen");
                exit(0);
            }
        } else {
            // Elternprozess ausgeführt, wartet für zwei Kindprozessen
            close(pipefd[0]); // Beendet Prozess 1
            close(pipefd[1]); // Beendet Prozess 2
            int j;
            waitpid(p,&j,0);
        }
    }
}

void help(){
    printf("nutze 'exit', 'cd', 'set' oder 'export' \n");
}

        /*
       handler
       Dient zur Befehlsauswahl eigener Befehle
       Uebergabewerte: char** args
       Rueckgabewert: int
       */
        int handler(char** args){
            int switchArguments = 0 , i , comands = 4;
            char* ListOfComands[] = {"exit", "cd", "set", "export"};


            //in der Schleife wird das erste Argument mit den gegebenen Befehlen verglichen und bestimmt
            for(i = 0; i < comands; i++){
                if(strcmp(args[0], ListOfComands[i]) == 0){
                    switchArguments = i + 1;
                    break;
                }
            }

            switch(switchArguments){
                case 1:
                    printf("exit");
                    exit(0);
                case 2:
                    chdir(args[1]);
                    return 0;
                case 3:
                    //printf("set\n");
                    if(args[1] != NULL){
                        char* v;
                        int i = 1;
                        while(args[i] != NULL) {
                            v = getenv(args[i]);
                            if (v != NULL) {
                                printf("%s\n", v);
                            } else {
                                printf("Nicht gefunden\n");
                            }
                            i++;
                        }
                    } else {
                        for (char **env = environment; *env; ++env) { //https://stackoverflow.com/questions/2085302/printing-all-environment-variables-in-c-c
                            printf("%s", *env);
                        }
                    }
                    printf("\n");
                    return 0;
                case 4:
                    //printf("export\n");
                    if(args[1]== NULL){
                        return 0;
                    }
                    int j = 1;
                    while(args[j]!=NULL){
                        char* export[2];
                        export[0]=strtok(args[j],"=");
                        export[1]=strtok(NULL,"=");
                        if(export[0]!=NULL && export[1]!=NULL){
                            setenv(export[0],export[1],1);
                        } else {
                            printf("Angabe inkorrekt\n");
                        }
                        j++;
                    }
                    return 0;
                default:
                    help();
                    break;
            }
            return 1;
        }

        void argsHandler(char* str,char** args){
            int i;
            strtok(str,"\n");
            //innerhalb der Schleife wird der Input als Argumente verarbeitet
            for(i = 0; i < 100; i++){
                args[i] = strsep(&str," ");

                if(args[i] == NULL)
                    break;
            }
        }
/*
trimwhithspace
Uebergabeparameter: char* str
Rueckgabeparameter: char*
Es wird ein char* uebergeben. er wird durchlaufen und geprueft, ob
es am Anfang oder Ende Lehrzeichen gibt. Wenn dann werden sie geloescht.
Quelle: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way  [2020.04.20 - 17:00uhr]
*/
        char *trimwhitespace(char *str){
            char *end;
            // Trim leading space
            while(isspace((unsigned char)*str)) str++;
            if(*str == 0)  // All spaces?
                return str;
            // Trim trailing space
            end = str + strlen(str) - 1;
            while(end > str && isspace((unsigned char)*end)) end--;
            // Write new null terminator character
            end[1] = '\0';
            return str;
        }

//Diese Funktion dient dazu Pipes zu erkennen
//Das erste Argument wird in pipe[0] und das Zweite in pipe[1] gespeichert
//Wenn pipe[1] = NULL ist, dann ist kein pipe vorhanden
        int pipeHandler(char* str, char** pipe){
            int i;
            for (i=0; i < 2; i++){
                pipe[i] = strsep(&str, "|");
                if(pipe[i] == NULL)
                    break;
            }

            if(pipe[1] == NULL)

                return 0;
            else {
                pipe[0] = trimwhitespace(pipe[0]);
                pipe[1] = trimwhitespace(pipe[1]);
                return 1;
            }
        }
/*
handelProcess

Verarbeitet die Nutzeringabe zu Argumenten, die als Befehle unserer Shell dienen.
Fuehrt Systemcall aus wenn notwendig.

Uebergabeparameter: char* str, char** args

*/
        void handelProcess(char* str, char** args){
            char* strPiped[2];
            char* argsPiped[100];
            int process, piped = 0;

            piped = pipeHandler(str, strPiped);
            // wenn eine Pipe erkannt wird, dann haben wir 2 ArgsArrays, die wir verarbeiten
            if(piped){
                argsHandler(strPiped[0], args);

                argsHandler(strPiped[1], argsPiped);
                //printf(strPiped[1]);
            }
            else{
                argsHandler(str,args);
            }
            process = handler(args);
            if(process){
                process += piped;
            }
            //wenn handler 1 ist wird createProcess aufgerufen (Kindprozess erstellen)
            if(process == 1){
                createProcess(args);
            }
            else if(process == 2){
                createProcessPiped(args,argsPiped);
            }
        }


/*
main
Uebergabeparameter: int argc, char* argv[]
argc = Anzahl der Argumente
argv[] = Pointer Array, die auf die uebergegebenden Argumente zeigen.
Bedingung: wenn getInput(input) != 1 ist dann geht er wieder hoch zur while-schleife und
	   startet von vorn
*/
        int main(int argc, char* argv[], char **envp){
            char input[1000], *parsedArgs[100];
            char* username = getenv("USER"); // erfassung des Nutzernames
            environment = envp;

            sayHello();
            while(1){
                printDirectory(username);
                if(getInput(input)) // wenn getInput 1 zuruek gibt gehts weiter
                    continue;
                handelProcess(input, parsedArgs);

            }



            return 0;
        }