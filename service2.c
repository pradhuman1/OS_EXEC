#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

typedef struct
{
	int res[5];
} result_buffer;

void my_handler() {}

int factorial(int n)
{
	int i, f = 1;
	for (i = 1; i <= n; i++)
	{
		f = f * i;
	}
	return f;
}

int main(int argc, char *argv[])
{
	int ans, clientID, key1, shmid;
	signal(SIGUSR1, my_handler);
	key1 = atoi(argv[7]);
	clientID = atoi(argv[6]);
	ans = factorial(atoi(argv[1]));
	result_buffer *output;
	shmid = shmget(key1, sizeof(result_buffer), IPC_CREAT | 0666);
	if (shmid < 0)
	{
		perror("error1 : ");
		exit(1);
	}
	output = (result_buffer *)shmat(shmid, NULL, 0);
	if (output == (void *)-1)
	{
		perror("error 2 : ");
		exit(0);
	}
	output->res[0] = ans;
	kill(clientID, SIGUSR1);
	return 0;
}