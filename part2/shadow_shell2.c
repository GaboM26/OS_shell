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
void myPrint(char *);

extern int errno;

static void die(char *inpt){
    errPrint(1);
    myExit(inpt);
}

int main(){
    size_t size = 100;
    int k;
    char *inpt = malloc(size);

    if(inpt == NULL){
        errPrint(1);
        exit(EXIT_FAILURE);
    }

    myPrint("$");
    if(ferror(stdout))
        errPrint(1);
    while((k = getline(&inpt, &size, stdin)) > 0){

        /*in case getline used realloc*/

        if(inpt == NULL){
            errPrint(1);
            exit(EXIT_FAILURE);
        }

        if(strcmp(inpt, "\n"))
            validInput(inpt);
        myPrint("$");
        if(ferror(stdout))
            errPrint(1);
    }

    if(k == -1 && errno != 0){
        errPrint(1);
        exit(EXIT_FAILURE);
    }
    /*if errno is 0 then no error occured*/
    myExit(inpt);

}

/*something was entered as input (no blank line)
  checks if it is valid and sends it to appropriate function */
void validInput(char *inpt){

    if(inpt[0]=='/'){
        runProg(inpt);
        return ;
    }
    else if(inpt[0] == '.' && inpt[1] == '/'){
        runProg(inpt);
        return ;
    }

    /*Manipulate input now that we know its not trying to go to folder*/
    char myInpt[strlen(inpt) + 1];
    strcpy(myInpt, inpt);

    char *spcmd = strtok(myInpt, " ");

    if(strchr(spcmd, '\n') != NULL)
        removeNL(spcmd);

    if(!strcmp(spcmd, "exit"))
        myExit(inpt);
    else if(!strcmp(spcmd, "cd"))
        cd(strtok(NULL, " "));
    else
        errPrint(0);
}
void runProg(char *inpt){
    /*get command and save a copy to execute (manipulated pointers)*/
    int st; 
    char *tok = " ";
    char *args[5000];
    char *cmd = strtok(inpt, tok);    
    char prgrm[strlen(cmd) + 1]; 
    strcpy(prgrm, cmd);

    /*parse and get all command line args*/
    int i = 1;
    while((args[i] = strtok(NULL, tok)) != NULL)
        i++;
    args[0] = progName(cmd); 

    if(strchr(prgrm, '\n') != NULL)
        removeNL(prgrm);

    if(strchr(args[i-1], '\n') != NULL)
        removeNL(args[i-1]);

    pid_t pid = fork();

    /*checks if fork failed*/
    if(pid < 0){
        errPrint(1);
        myExit(inpt);
    }
    else if(pid == 0){ /*child process*/
        execv(prgrm, args);
        die(inpt);
    }
    else{ /*parent process*/
        if(waitpid(pid, &st, 0) != pid){
            errPrint(1);
            myExit(inpt);
        }
    }

}

/*removes next line from a string
 * Note that the only case this will happen is if one presses
 * enter at the very end due to how fgets works*/
void removeNL(char *str){
    str[strlen(str)-1] = 0;
}

void errPrint(int realErr){
    char err[1000];
    char start[] = "error: ";
    strcpy(err, start);
    if(!realErr){
        strcat(err, "unknown command\n");
    }
    else{
        strcat(err, strerror(errno));
        strcat(err, "\n");
    }

    if(write(2, err, strlen(err)) != strlen(err)){
        exit(EXIT_FAILURE);
    }

    if(ferror(stderr)){
        exit(EXIT_FAILURE);
    }
}

/*Returns pointer to only the program name given a path*/
char *progName(char *path){
    char *delim = "/";
    char *name= strtok(path, delim);
    char *tmp; 
    while((tmp=strtok(NULL, delim)) != NULL){
        name = tmp;
    }

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
    exit(EXIT_SUCCESS);
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

void myPrint(char *msg){
    if(write(1, msg, strlen(msg)) == -1){
        exit(EXIT_FAILURE);
    }
}
