#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define KEY 42

int semcreate(int val)
{
	int semid;
	semid = semget(KEY, 1, IPC_EXCL | IPC_CREAT | 0600);
	if (semid==-1)
	{
		printf("Oh no this semaphore already exists :(\n");
		return 0;
	}
	else
	{
		semctl(KEY, 0, SETVAL, val);
		printf("Tada, you have a semaphore now. It's at %d\n",semid);
	}
	return semid;
}

int semview()
{
	int semval = semget(KEY,1,0666);
	semctl(semval,1,GETVAL);
	if (semval==-1)
	{
		printf("You need to create a semaphore first\n");
		return 0;
	}
	else
	{
		printf("The semaphore value is: %d\n",semval);
	}
	return semval;
}

int semclose()
{
	int semloc = semget(KEY,1,0600);
	printf("Semaphore eliminated: %d\n", semctl(semloc,0,IPC_RMID));
}


int main(int argc, char **argv)
{
	if (argc<2)
	{
		printf("You seem to be missing some arguments.\n");
		return 0;
	}
	else
	{
		if (!strcmp(argv[1],"-v"))
		{
			semview();
		}
		if (!strcmp(argv[1],"-r"))
		{
			semclose();
		}
		if ((!strcmp(argv[1],"-c")&&(argc==3)))
		{
			int val = argv[2]-'0';
			semcreate(val);
		}
	}
}