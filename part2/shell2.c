#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int min(int a, int b)
{
	if (a > b)
		return b;
	return a;
}

/*removes next line from a string
 * Note that the only case this will happen is if one presses
 * enter at the very end due to how getline works
 */
void removeNL(char *str) { str[strlen(str) - 1] = '\0'; }

void myExit(char *inpt, size_t size)
{
	munmap(inpt, size);
	exit(EXIT_SUCCESS);
}

void errPrint(int realErr)
{
	char err[1000];
	char start[] = "error: ";

	strcpy(err, start);
	if (!realErr) {
		strcat(err, "unknown command\n");
	} else {
		strcat(err, strerror(errno));
		strcat(err, "\n");
	}

	if (write(2, err, strlen(err)) != strlen(err))
		exit(EXIT_FAILURE);

	if (ferror(stderr))
		exit(EXIT_FAILURE);
}

/*Returns pointer to only the program name given a path*/
char *progName(char *path)
{
	char *delim = "/";
	char *name = strtok(path, delim);
	char *tmp;

	while ((tmp = strtok(NULL, delim)) != NULL)
		name = tmp;

	if (strchr(name, '\n') != NULL)
		removeNL(name);
	return name;
}

static void die(char *inpt, size_t sz)
{
	errPrint(1);
	myExit(inpt, sz);
}


void cd(char *inpt)
{

	if (inpt == NULL) {
		errPrint(0);
		return;
	}
	if (strchr(inpt, '\n') != NULL)
		removeNL(inpt);

	if (chdir(inpt))
		errPrint(1);
}

void *myMalloc(size_t size)
{
	void *hp;

	hp = mmap(NULL, size, PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE, -1,
			0);
	if (__builtin_expect(hp == MAP_FAILED, 0))
		return NULL;

	return hp;
}

void runProg(char *inpt, size_t size)
{
	/*get command and save a copy to execute (manipulated pointers)*/
	int st;
	char *tok = " ";
	char *args[5000];
	char *cmd = strtok(inpt, tok);
	char prgrm[strlen(cmd) + 1];

	strcpy(prgrm, cmd);

	/*parse and get all command line args*/
	int i = 1;

	while ((args[i] = strtok(NULL, tok)) != NULL)
		i++;
	args[0] = progName(cmd);

	if (strchr(prgrm, '\n') != NULL)
		removeNL(prgrm);

	if (strchr(args[i - 1], '\n') != NULL)
		removeNL(args[i - 1]);

	pid_t pid = fork();

	/*checks if fork failed*/
	if (pid < 0) {
		errPrint(1);
		myExit(inpt, size);
	} else if (pid == 0) { /*child process*/
		execv(prgrm, args);
		die(inpt, size);
	} else { /*parent process*/
		if (waitpid(pid, &st, 0) != pid) {
			errPrint(1);
			myExit(inpt, size);
		}
	}
}

/*something was entered as input (no blank line)
 *checks if it is valid and sends it to appropriate function
 */
void validInput(char *inpt, size_t sz)
{

	if (inpt[0] == '/') {
		runProg(inpt, sz);
		return;
	} else if (inpt[0] == '.' && inpt[1] == '/') {
		runProg(inpt, sz);
		return;
	} else if (inpt[0] == '.' && inpt[1] == '.' && inpt[2] == '/') {
		runProg(inpt, sz);
		return;
	}

	/*Manipulate input now that we know its not trying to go to folder*/
	char myInpt[strlen(inpt) + 1];

	strcpy(myInpt, inpt);

	char *spcmd = strtok(myInpt, " ");

	if (strchr(spcmd, '\n') != NULL)
		removeNL(spcmd);

	if (!strcmp(spcmd, "exit"))
		myExit(inpt, sz);
	else if (!strcmp(spcmd, "cd"))
		cd(strtok(NULL, " "));
	else
		errPrint(0);
}

void myPrint(char *msg)
{
	if (write(1, msg, strlen(msg)) == -1)
		exit(EXIT_FAILURE);
}

int myGetLine(char **buff, size_t *size)
{
	int bytrd;
	char *wrkBuff = *buff;
	/* EOF  */
	bytrd = read(0, *buff, *size - 1);
	if (bytrd == 0) {
		return 0;
	} else if (bytrd < 0) {
		/* read error */
		return -1;
	}
	wrkBuff[bytrd] = '\0';
	if (strchr(*buff, '\n'))
		return bytrd;

	/* buffer overflowed */
	int newSize = *size + 1000;
	char tempBuff[1000];
	char *newBuff = myMalloc(newSize);

	memcpy(newBuff, *buff, *size);
	munmap(*buff, *size);

	while ((bytrd = read(0, tempBuff, sizeof(tempBuff) - 1)) ==
			sizeof(tempBuff) - 1) {
		if (strchr(tempBuff, '\n'))
			break;
		tempBuff[bytrd] = '\0';
		strcat(newBuff, tempBuff);
		char *oldBuff = newBuff;

		newSize = newSize + 1000;
		newBuff = myMalloc(newSize);
		memcpy(newBuff, oldBuff, newSize - 1000);
		munmap(oldBuff, newSize - 1000);
		if (strchr(tempBuff, '\n'))
			break;
	}
	tempBuff[bytrd] = '\0';
	strcat(newBuff, tempBuff);
	*buff = newBuff;
	*size = newSize;
	return *size;
}

int main(void)
{
	size_t size = 100;
	int k;
	char *inpt = (char *)myMalloc(size);

	if (inpt == NULL) {
		errPrint(1);
		exit(EXIT_FAILURE);
	}

	myPrint("$");
	if (ferror(stdout))
		errPrint(1);
	while ((k = myGetLine(&inpt, &size)) > 0) {

		if (inpt == NULL) {
			errPrint(1);
			exit(EXIT_FAILURE);
		}

		if (strcmp(inpt, "\n"))
			validInput(inpt, size);
		myPrint("$");
		if (ferror(stdout))
			errPrint(1);
	}

	if (k == -1 && errno != 0) {
		errPrint(1);
		exit(EXIT_FAILURE);
	}
	/*if errno is 0 then no error occured*/
	myExit(inpt, size);
}
