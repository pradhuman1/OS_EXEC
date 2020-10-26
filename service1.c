#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

typedef struct{
    int res[5];
}result_buffer;

void my_handler() {}
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int array[], int low, int high) {
  
  int pivot = array[high];
  int i = (low - 1);

  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }

  swap(&array[i + 1], &array[high]);
  return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {
    
    int pi = partition(array, low, high);
    
    quickSort(array, low, pi - 1);
    
    quickSort(array, pi + 1, high);
  }
}


int main(int argc, char *argv[]){
  
    int data[5],k=0,i;
    int clientID,key1,shmid;

    key1 = atoi(argv[7]);
    clientID = atoi(argv[6]);

    for (i=1; i<argc; i++){
        data[k++]=atoi(argv[i]);
    }
    quickSort(data, 0, 4);
    result_buffer *output;    
    shmid = shmget(key1, sizeof(result_buffer), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("error1 : ");
        exit(1);
    }
    output = (result_buffer *)shmat(shmid,NULL,0);
    if(output == (void *)-1){
        perror("error 2 : ");
        exit(0);
    }
    for(i=0;i<5;i++){
        output->res[i] = data[i];
    }
    kill(clientID, SIGUSR1);
}