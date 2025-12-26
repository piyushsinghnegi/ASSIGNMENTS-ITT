#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARRAY_SIZE 1000

void sortArray(int *dataArray, int size) {
    for (int outerIndex = 0; outerIndex < size - 1; outerIndex++) {
        for (int innerIndex = 0; innerIndex < size - outerIndex - 1; innerIndex++) {
            if (dataArray[innerIndex] > dataArray[innerIndex + 1]) {
                int temp = dataArray[innerIndex];
                dataArray[innerIndex] = dataArray[innerIndex + 1];
                dataArray[innerIndex + 1] = temp;
            }
        }
    }
}

int takeIntInput(){
    int value;
    while(1){
        if(scanf("%d", &value) == 1 && value > 0){
            return value; 
        }
        while(getchar() != '\n');
        printf("Invalid input\n");
    }
}

void takeInput(int *data, int size){
    printf("Enter %d integers:\n", size);
    for(int i = 0; i < size; i++){
        data[i] = takeIntInput();
    }
}

int main() {
    key_t key = ftok(".", 65);
    int shmid = shmget(key, sizeof(int) * (MAX_ARRAY_SIZE + 1), 0666 | IPC_CREAT);
    
    int *arr = (int*) shmat(shmid, (void*)0, 0);

    printf("Enter size of array: ");
    int size = takeIntInput();

    if(size > MAX_ARRAY_SIZE){
        printf("Size exceeds limit\n");
        exit(1);
    }

    arr[0] = size;

    takeInput(&arr[1], size);

    if (fork() == 0) {
        sleep(1);
        
        int childSize = arr[0];
        
        sortArray(&arr[1], childSize);
        
        shmdt(arr);
        exit(0);
        
    } else {
        wait(NULL);
        
        for(int i=0; i<size; i++){
            printf("%d ", arr[i+1]);
        }
        printf("\n");
        
        shmdt(arr);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}