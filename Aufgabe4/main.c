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
//		printf(" Kind process wurde erzeugt\n");
		if(execvp(a[0], a) < 0)
			printf(" execvp ausgefuerung faild\n");
		exit(0);
	} else{
		wait(NULL);
	}
}

void help(){
//	printf("nutze 'exit', 'cd', 'set' oder 'export' \n");
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
				return 1;
			case 3:
				printf("set\n");
				return 1;
			case 4:
				printf("export\n");
				return 1;
			default:
				help();
				break;
     }
return 0;
}

void handelProcess(char* str, char** args){
    int process,i;
    strtok(str,"\n");
    //innerhalb der Schleife wird der Input als Argumente verarbeitet
    for(i = 0; i < 100; i++){
        args[i] = strsep(&str," ");

        if(args[i] == NULL)
            break;
    }

    process = handler(args);
    //wenn process 0 ist wird createProcess aufgerufen (Kindprozess erstellen)
    if(process == 0){
        createProcess(args);
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
