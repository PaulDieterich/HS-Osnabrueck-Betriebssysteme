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
	char* eing;
	scanf("%d", &eing);
	if(strlen(eing) == 0){
		return 0;
	}else{
		return 1;
	}
}

void printDirectory(char* username){

	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("[%s@ -%s ]\e" ,username,cwd);

}

void createProcess(char* a){
	/* TODO */
	pid_t pid = fork();
	if(pid < 0){
		printf(" Konnte nicht erstellt werden\n");
	} else if(pid == 0){
		printf(" Kind process wurde erzeugt\n");
		if(execvp(a[0], a) == <0)
			printf(" execvp ausgefuerung faild\n");
		exit(0);
	} else{
		wait(NULL);
	}
}

void help(){
	printf("nutze 'exit', 'ch', 'set' oder 'export' \n");
}

int handler(char** args){
	/* TODO */
	int switchArguments = 0;
	char* ListOfComands[] = {"exit", "cd", "set", "export"};
	switch(ListOfComands[i]):
			case 0:
				printf("exit");
				exit(0);
			case 1:
				chdir(args[1]);
				returnn 1;
			case 2:
				printf("set");
				return 1;
			case 3:
				printf("export");
				return 1;
			default:
				help();
				break;
return 0;
}
handelProcess(){
/*TODO: */
}
int main(int argc, char* argv[]){
	char input[1000], *paresedArgs[100];
	char* username = getenv("USER"); // erfassung des Nutzernames
	char eing;

	sayHello();
	while(1){
		printDirectory(username);
		if(getInput(input)) // wenn getInput 1 zuruek gibt gehts weiter
			continue;

	}



return 0;
}
