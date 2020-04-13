#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>



void sayHello(){
	printf("Welcome to my own shell\n"); //Begruessung
}

int getInput(char* str){
	int max = 1000;
	fgets(str, max,stdin );
	if(strlen(str) != 0){
		return 0;
	}else{
		return 1;
	}
}

void printDirectory(char* username){

	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("[%s@ -%s ]" ,username,cwd);

}

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
    pid_t p1, p2;
    //hier werden die beiden Deskriptoren mit pipe() erzeugt
    if(pipe(pipefd) < 0){
        printf("\nPipe konnte nicht erstellt werden");
        return;
    }
    p1 = fork();
    if(p1 < 0){
        printf("\nKindprozess konnte nicht erstellt werden");
        return;
    }

    if(p1 == 0){
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
        p2 = fork();

        if(p2 < 0){
            printf("\nKindprozess konnte nicht erstellt werden");
            return;
        }

        //Kind 2 executing
        if(p2 == 0){
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if(execvp(argsPiped[0],argsPiped) < 0){
                printf("\nKonnte das zweite Command nicht ausführen");
                exit(0);
            }
        } else {
            // Elternprozess ausgeführt, wartet für zwei Kindprozessen
            wait(NULL);
            wait(NULL);
        }
    }
}

void help(){
	printf("nutze 'exit', 'cd', 'set' oder 'export' \n");
}

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
				printf("set\n");
				return 0;
			case 4:
				printf("export\n");
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
        return 1;
    }
}

void handelProcess(char* str, char** args){
    char* strPiped[2];
    char* argsPiped[100];
    int process, piped = 0;

    piped = pipeHandler(str, strPiped);
    if(piped){
        argsHandler(strPiped[0], args);
        argsHandler(strPiped[1], argsPiped);
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



int main(int argc, char* argv[]){
	char input[1000], *parsedArgs[100];
	char* username = getenv("USER"); // erfassung des Nutzernames


	sayHello();
	while(1){
		printDirectory(username);
		if(getInput(input)) // wenn getInput 1 zuruek gibt gehts weiter
			continue;
        handelProcess(input, parsedArgs);

	}



return 0;
}
