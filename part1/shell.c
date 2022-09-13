#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

static void die(const char *err){
    perror(err);
    exit(-1);
}

void processInput(char *);
void removeNL(char *);
int main(){
    char inpt[1000];
    printf("$");
    while(fgets(inpt, sizeof(inpt), stdin) != NULL){
        processInput(inpt);
        printf("$");
    }
}

void processInput(char *inpt){
    int st; 
    char *tok = " ";
    char *cmd = strtok(inpt, tok);    

    if(strchr(cmd, '\n') != NULL)
        removeNL(cmd);

    pid_t pid = fork();

    if(pid < 0){
        die("ERROR fork");
    }
    else if(pid == 0){ //child process
        execl(cmd, cmd, (char *) 0);
        die("ERROR");
    }
    else{ //parent process
        if(waitpid(pid, &st, 0) != pid){
            perror("ERROR: waitpid");
        }
    }

}

void removeNL(char *str){
    str[strlen(str)-1] = 0;
}
