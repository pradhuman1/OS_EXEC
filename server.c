#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int shmid;
pid_t pid;
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
    int serID;
    int in;
    struct request_info queue[100];
} Q;

void sighandler()
{
    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
    printf("\nExiting....\n");
    exit(0);
};
void my_handler() {}

int main(int argc, char *argv[])
{
    printf("Server Program Started. Process id: %d\n", getpid());
    int curr = -1, i, servise;
    signal(SIGINT, sighandler);
    signal(SIGUSR1, my_handler);
    int key;
    Q *infoItem;
    key = ftok("empty.txt", 11);
    shmid = shmget(key, sizeof(Q), IPC_CREAT | 0666);

    if (shmid < 0)
    {
        perror("error1 : ");
        exit(1);
    }
    infoItem = (Q *)shmat(shmid, NULL, 0);
    if (infoItem == (void *)-1)
    {
        perror("error 2 : ");
        exit(0);
    }
    infoItem->serID = getpid();
    infoItem->in = -1;
    while (1)
    {
        i = infoItem->in;
        if (curr < i)
        {
            curr++;

            // printf("SER : %d \n", (infoItem->queue[curr]).serviceNo);
            // printf("client : %d\n", (infoItem->queue[curr]).clientPID);
            // printf("H");

            // printf("key : %d\n", (infoItem->queue[curr]).result_ref_key);

            // for (int j = 0; j < 5; j++)
            // {
            //     printf("%d\n", (infoItem->queue[curr]).neededArg[j]);
            // }
            servise = (infoItem->queue[curr]).serviceNo;

            char buff1[40], buff2[40], buff3[40], buff4[40], buff5[40], buff6[40], buff7[40];
            sprintf(buff1, "\n%d", (infoItem->queue[curr]).neededArg[0]);
            sprintf(buff2, "\n%d", (infoItem->queue[curr]).neededArg[1]);
            sprintf(buff3, "\n%d", (infoItem->queue[curr]).neededArg[2]);
            sprintf(buff4, "\n%d", (infoItem->queue[curr]).neededArg[3]);
            sprintf(buff5, "\n%d", (infoItem->queue[curr]).neededArg[4]);
            sprintf(buff6, "\n%d", (infoItem->queue[curr]).clientPID);
            sprintf(buff7, "\n%d", (infoItem->queue[curr]).result_ref_key);

            pid = fork();
            if (pid == 0)
            {
                if (servise == 2)
                {
                    execl("./service2", "./service2", buff1, buff2, buff3, buff4, buff5, buff6, buff7, NULL);
                }
                else if (servise == 1)
                {
                    execl("./service1", "./service1", buff1, buff2, buff3, buff4, buff5, buff6, buff7, NULL);
                }
                else if (servise == 3)
                {
                    execl("./service3", "./service3", buff1, buff2, buff3, buff4, buff5, buff6, buff7, NULL);
                }
            }
        }
    }

    printf("Exiting ...\n");
    return 0;
}