#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 42

union semun 
{
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO                               (Linux-specific) */
} data;


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
		printf("%d\n",semctl(semid, 0, SETVAL, val));
		printf("Tada, you have a semaphore now. It's at %d\n",semid);
	}
	return semid;
}

int semview()
{
	int semval = semget(KEY, 1, 0600);
	int semcut = semctl(semval, 0, GETVAL);
	printf("%d\n",semcut);
	if (semcut<0)
	{
		printf("You need to create a semaphore first\n");
		return 0;
	}
	else
	{
		printf("The semaphore value is: %d\n",semcut);
	}
	return data.val+0;
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
			int val = atoi(argv[2]);
			semcreate(val);
		}
	}
}