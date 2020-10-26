#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

// Maximum number of digits in output
#define MAX 500
typedef struct
{
	int res[5];
} result_buffer;

void my_handler() {}

int multiply(int x, int res[], int res_size);

int factorial(int n)
{
	int res[MAX];
	res[0] = 1;
	int res_size = 1;
	for (int x = 2; x <= n; x++)
		res_size = multiply(x, res, res_size);

	for (int i = res_size - 1; i >= 0; i--)
		return res[i];
	return 0;
}

int multiply(int x, int res[], int res_size)
{
	int carry = 0;

	for (int i = 0; i < res_size; i++)
	{
		int prod = res[i] * x + carry;
		res[i] = prod % 10;
		carry = prod / 10;
	}

	while (carry)
	{
		res[res_size] = carry % 10;
		carry = carry / 10;
		res_size++;
	}
	return res_size;
}

int main(int argc, char *argv[])
{
	int ans, clientID, key1, shmid;
	signal(SIGUSR1, my_handler);
	key1 = atoi(argv[7]);
	clientID = atoi(argv[6]);
	printf("ClientID : %d\n", clientID);
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