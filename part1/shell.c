#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>

void runProg(char *);
void removeNL(char *);
void errPrint(int );
char *progName(char *);
void validInput(char *);
int min(int, int);
void myExit(char *);
void cd(char *);

static void die(const char *err){
    errPrint(1);
    exit(-1);
}

int main(){
    size_t size = 1;
    char *inpt = malloc(size);
    printf("$");
    while((getline(&inpt, &size, stdin)) != -1){
        if(strcmp(inpt, "\n"))
            validInput(inpt);
        printf("$");
    }
}

//something was entered as input (no blank line)
//checks if it is valid and sends it to appropriate function
void validInput(char *inpt){

    //means it is trying to go to a folder and run executable
    if(inpt[0]=='/'){
        runProg(inpt);
        return ;
    }

    //Manipulate input now that we know its not trying to go to folder
    char myInpt[strlen(inpt) + 1];
    strcpy(myInpt, inpt);

    //divide into tokens
    char *spcmd = strtok(myInpt, " ");

    if(strchr(spcmd, '\n') != NULL)
        removeNL(spcmd);
    //If you are exiting, you pass original pointer
    if(!strcmp(spcmd, "exit"))
        myExit(inpt);
    else if(!strcmp(spcmd, "cd"))
        cd(strtok(NULL, " "));
    else
        errPrint(0);
}
void runProg(char *inpt){
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
            errPrint(1);
        }
    }

}

//removes next line from a string
//Note that the only case this will happen is if one presses
//enter at the very end due to how fgets works
void removeNL(char *str){
    str[strlen(str)-1] = 0;
}

//useful function in this case to print errors as required
void errPrint(int realErr){
    if(realErr)
        fprintf(stderr, "error: %s\n", strerror(errno));
    else
        fprintf(stderr, "error: unknown command\n");
}

//Returns pointer to only the program name given a path
char *progName(char *path){
    char *name;
    char *delim = "/";
    char *tmp = strtok(path, delim);
    while((tmp=strtok(NULL, delim)) != NULL){
        name = tmp;
    }
    //removes next line if necessary
    if(strchr(name, '\n') != NULL)
        removeNL(name);
    return name;
}

int min(int a, int b){
    if(a>b){
        return b;
    }
    return a;
}


void myExit(char *inpt){
    free(inpt);
    exit(0);
}

void cd(char *inpt){
    
    if(strchr(inpt, '\n') != NULL)
        removeNL(inpt);

    if(chdir(inpt))
        errPrint(1);

}
