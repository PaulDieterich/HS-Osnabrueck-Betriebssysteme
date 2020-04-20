/*
Paul Dieterich, Matrikelnummer: 883966
Jan Weimer, Matrikelnummer: 868487
*/


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

/*
getInput
Dient zur Nutzereingabe
Rueckgabewert: int
Uebergabewert: char* str
Bedingung: char* str muss laenger als 0 sein.
*/
int getInput(char* str){
	int max = 1000;
	fgets(str, max,stdin );
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
	pid_t pid = fork(); // erstellt Kindprozess.
	if(pid < 0){
		printf(" Konnte nicht erstellt werden\n");
	} else if(pid == 0){
//		printf(" Kind process wurde erzeugt\n");
		if(execvp(a[0], a) < 0) //execvp = wird ueberprueft ob uebergabe
					//	   ausfuerbar ist.
			printf(" execvp Ausfuerung faild\n");
		exit(0);
	} else{
		wait(NULL); // Elternprozess wartet, dass Kindprozess terminiert wird.
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
	char* ListOfComands[] = {"exit", "cd", "set", "export","help"};


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
			case 5:
				help();
				return 1;
			default:
				break;
     }
return 0;
}
/*
handelProcess

Verarbeitet die Nutzeringabe zu Argumenten, die als Befehle unserer Shell dienen.
Fuehrt Systemcall aus wenn notwendig.

Uebergabeparameter: char* str, char** args

*/
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


/*
main
Uebergabeparameter: int argc, char* argv[]
argc = Anzahl der Argumente
argv[] = Pointer Array, die auf die uebergegebenden Argumente zeigen.
Bedingung: wenn getInput(input) != 1 ist dann geht er wieder hoch zur while-schleife und
	   startet von vorn
*/
int main(int argc, char* argv[]){
	char input[1000], *parsedArgs[100];
	char* username = getenv("USER"); // Erfassung des Nutzernames

	sayHello();
	while(1){
		printDirectory(username);
		if(getInput(input)) // wenn getInput 1 zuruek gibt gehts weiter
			continue;
        handelProcess(input, parsedArgs);

	}



return 0;
}
