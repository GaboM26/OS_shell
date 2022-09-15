#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>

void processInput(char *);
void removeNL(char *);
void errPrint();
char *progName(char *);

static void die(const char *err){
    errPrint();
    exit(-1);
}

int main(){
    char inpt[2000];
    printf("$");
    while(fgets(inpt, sizeof(inpt), stdin) != NULL){
        if(strcmp(inpt, "\n"))
            processInput(inpt);
        printf("$");
    }
}

void processInput(char *inpt){
    //get command and save a copy to execute (manipulated pointers)
    int st; 
    char *tok = " ";
    char *args[1000];
    char *cmd = strtok(inpt, tok);    
    char prgrm[strlen(cmd) + 1]; 
    strcpy(prgrm, cmd);

    //parse and get all command line args
    int i = 1;
    while((args[i] = strtok(NULL, tok)) != NULL)
        i++;
    args[0] = progName(cmd); //make first arg program name
    
    // if input is one command, take out the \n from it
    if(strchr(prgrm, '\n') != NULL)
        removeNL(prgrm);

    //Do the same for final input
    if(strchr(args[i-1], '\n') != NULL)
        removeNL(args[i-1]);

    pid_t pid = fork();

    if(pid < 0){
        die("ERROR fork");
    }
    else if(pid == 0){ //child process
        execv(prgrm, args);
        die("ERROR");
    }
    else{ //parent process
        if(waitpid(pid, &st, 0) != pid){
            errPrint();
        }
    }

}

void removeNL(char *str){
    str[strlen(str)-1] = 0;
}

void errPrint(){
    fprintf(stderr, "error: %s\n", strerror(errno));
}

char *progName(char *path){
    char *name;
    char *delim = "/";
    char *tmp = strtok(path, delim);
    while((tmp=strtok(NULL, delim)) != NULL){
        name = tmp;
    }
    return name;
}
