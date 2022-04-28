#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int dir() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("Error");
    }
    return 1;
}

char historyQueue[10][100];
int order = 1;
void keepHistory(char *args){
    if(order <= 10){
        strcpy(historyQueue[order-1],args);
        order++;
    }
    else{
        for (int i = 1; i <= 10 ; i++) {
            if(i != 10){
                strcpy(historyQueue[i-1],historyQueue[i]);
            }
            else{
                break;
            }
        }
        strcpy(historyQueue[9],args);
    }

}

void showHistory(){
    for (int i = 0; i < 10; i++){
        printf("[%d] %s\n",(i+1),historyQueue[i]);
    }
}

int bye() {
    exit(0);
}


int main(){

    printf("./myShell\n ");

    while(1){

        char input[100];
        char *refreshedInput;
        char *args[10] = {NULL};
        char *args2[10]= {NULL};
        char *splitedInput;
        int index = 0;
        bool lock = true;
        bool lockProcess = false;


        printf("myShell> ");
        fgets(input,100,stdin);
        refreshedInput = strtok(input,"\n");
        if (strlen(input)==1){
                    continue;
        }
        strcpy(input, refreshedInput);


        splitedInput = strtok(input, " ");


        while (splitedInput != NULL) {
            if (lock && strcmp(splitedInput, "|") == 0) {
            	index = -1;
                lock = false;
            }
            else if(strcmp(splitedInput, "&") == 0){
            	lockProcess = true;
                splitedInput = NULL;
                break;
            }
            else {
                if (lock == true) {
                    args[index] = splitedInput;
                } else {
                    args2[index] = splitedInput;
                }
            }

            splitedInput = strtok(NULL, " ");
            index++;
        }

        keepHistory(input);

        if(strcmp(args[0],"cd") == 0){
            if(args[1] == NULL){
                chdir(getenv("HOME"));
            }
            else {
                if (chdir(args[1]) != 0) {
                    perror("Error");
                }
            }
        }
        else if(strcmp(input,"dir") == 0){
            dir();
        }
        else if(strcmp(input,"history") == 0){
            showHistory();
        }
        else if(strcmp(input,"bye") == 0){
            bye();
        }
        else{
            if(lock == true){
                pid_t process;
                process = fork();

                if (process == 0) {
                    if (execvp(args[0], args) == -1) {
                        perror("Error");
                    }
                    exit(EXIT_FAILURE);
                } else if (process < 0) {
                    perror("Error");
                    return 1;
                } else {
                    if(lockProcess == false){	
                    	wait(NULL);
            	    }
                }
            }
            else{
                int fd[2];
                pid_t process1, process2;
                process1 = fork();
                int pipedProcess;
                pipedProcess = pipe(fd);

                if(pipedProcess == -1){
                    perror("Error");
                    return 1;
                } 
                
                
	    	if (process1 == 0) {
	        	close(fd[0]);
	        	dup2(fd[1], 1);
	        	close(fd[1]);
	        	if (execvp(args[0], args) == -1) {
	            		perror("Error");
	        	}
	        	return 0;
	    	}
	    	else if(process1 == -1){
	    		perror("Error");
	    		return 1;
	    	}
	    	else{
	    		process2 = fork();
	    		if(process2 == 0){
	    			close(fd[1]);
	        		dup2(fd[0], 0);
	        		close(fd[0]);
	        		if (execvp(args2[0], args2) == -1) {
	        	    		perror("Error");
	        		}
	        		return 0;
	    		}
	    		else if(process2 == -1){
		    		perror("Error");
		    		return 1;
	    		}
	    		else{
	    			close(fd[0]);
	        		close(fd[1]);
	        		wait(NULL);
	        		wait(NULL);
	    		}
	        	
	    	}
	    	  
            }
        }

    }

}
