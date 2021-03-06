#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
typedef struct
{
    int res[5];
} result_buffer;

struct request_info
{
    int serviceNo;
    int clientPID;
    int neededArg[5];
    int result_ref_key;
};

typedef struct
{
    int lock;
    int tempIN;
    int in;
    struct request_info queue[100];
} Q;

void my_handler()
{
}

int printOutput(result_buffer *item, int i)
{

    printf("Output : ");
    for (int count = 0; count < i; count++)
    {
        printf("%d ", item->res[count]);
    }
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{
    printf("Client Program Started. Process id: %d\n", getpid());
    int choice, client, arr[5], shmid, i, curr, shmid1;
    signal(SIGUSR1, my_handler);
    client = getpid();
    printf("Available Services : \n\n");
    printf("Service 1 : Sorting an integer array of 5 items\n");
    printf("Service 2 : Factorial of an integer\n");
    printf("Service 3 : Calculate the GCD (greatest common divisor) of two given integers\n");
    printf("\nWhich service do you want(1/2/3) :  \n");
    scanf("%d", &choice);

    //

    int key1, key;

    Q *infoItem;
    key = ftok("empty.txt", 11);

    shmid = shmget(key, sizeof(Q), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("error1 info : ");
        exit(1);
    }
    infoItem = (Q *)shmat(shmid, NULL, 0);
    if (infoItem == (void *)-1)
    {
        perror("error 2 : ");
        exit(0);
    }

    (infoItem->tempIN)++;
    curr = infoItem->tempIN;

    result_buffer *output;
    key1 = ftok("empty.txt", curr);

    shmid1 = shmget(key1, sizeof(result_buffer), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("error1 result buffer: ");
        exit(1);
    }
    output = (result_buffer *)shmat(shmid1, NULL, 0);
    if (output == (void *)-1)
    {
        perror("error 2 : ");
        exit(0);
    }

    //sharedMemory

    (infoItem->queue[curr]).clientPID = client;

    if (choice == 1)
    {
        printf("Enter 5 integers : ");
        for (i = 0; i < 5; i++)
        {
            scanf("%d", &(infoItem->queue[curr]).neededArg[i]);
        }
    }
    else if (choice == 2)
    {
        printf("Enter a integer : ");
        scanf("%d", &(infoItem->queue[curr]).neededArg[0]);
        i = 1;
    }
    else if (choice == 3)
    {
        printf("Enter two numbers : ");
        scanf("%d", &(infoItem->queue[curr]).neededArg[0]);
        scanf("%d", &(infoItem->queue[curr]).neededArg[1]);
        i = 1;
    }
    else
    {
        printf("Sorry! Service not available.Try Again...\n");
        shmctl(shmid1, IPC_RMID, NULL);
        exit(0);
    }
    (infoItem->queue[curr]).result_ref_key = key1;
    (infoItem->queue[curr]).serviceNo = choice;
    printf("Getting Result... \n");
    infoItem->in = infoItem->tempIN;
    pause();
    printOutput(output, i);
    shmctl(shmid1, IPC_RMID, NULL);
    printf("Exiting ...\n");
    return 0;
}