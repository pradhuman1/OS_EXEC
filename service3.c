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

int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int main(int argc, char *argv[])
{

    int a = atoi(argv[1]), b = atoi(argv[2]);
    int ans, clientID, key1, shmid;

    key1 = atoi(argv[7]);
    clientID = atoi(argv[6]);
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
    ans = gcd(a, b);
    output->res[0] = ans;
    kill(clientID, SIGUSR1);
    return 0;
}