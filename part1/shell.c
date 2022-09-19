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

extern int errno;

static void die(char *inpt){
    errPrint(1);
    myExit(inpt);
}

int main(){
    size_t size = 100;
    int k;
    char *inpt = malloc(size);

    //In case malloc failed
    if(inpt == NULL){
        perror("malloc failed\n");
        exit(-1);
    }

    printf("$");
    while((k = getline(&inpt, &size, stdin)) > 0){
        //in case getline used realloc
        if(inpt == NULL){
            perror("malloc failed\n");
            exit(-1);
        }

        if(strcmp(inpt, "\n"))
            validInput(inpt);
        printf("$");
    }

    if(k == -1 && errno != 0){
        errPrint(1);
    }
    //if errno is 0 then no error occured
    myExit(inpt);
    
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
    char *args[5000];
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

    //checks if fork failed
    if(pid < 0){
        perror("fork failed");
        myExit(inpt);
    }
    else if(pid == 0){ //child process
        execv(prgrm, args);
        die(inpt);
    }
    else{ //parent process
        if(waitpid(pid, &st, 0) != pid){
            //checks if waitpid failed
            perror("waitpid failed");
            myExit(inpt);
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
    char *delim = "/";
    char *name= strtok(path, delim);
    char *tmp; 
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
    
    if(inpt == NULL){
        errPrint(0);
        return ;
    }
    if(strchr(inpt, '\n') != NULL)
        removeNL(inpt);

    if(chdir(inpt))
        errPrint(1);

}
