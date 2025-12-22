#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

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
    int fd1[2];
    int fd2[2];
    
    int data[MAX_ARRAY_SIZE];
    int size;

    if (pipe(fd1) == -1 || pipe(fd2) == -1) { 
        perror("Pipe Failed"); 
        return 1; 
    }

    printf("Enter size of array: ");
    size = takeIntInput();

    if(size > MAX_ARRAY_SIZE){
        printf("Size exceeds limit\n");
        exit(1);
    }

    takeInput(data, size);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        int received[MAX_ARRAY_SIZE];
        int receivedSize;

        close(fd1[1]);
        close(fd2[0]);

        read(fd1[0], &receivedSize, sizeof(int));

        read(fd1[0], received, receivedSize * sizeof(int));
        
        sortArray(received, receivedSize); 

        write(fd2[1], received, receivedSize * sizeof(int));
        
        close(fd1[0]);
        close(fd2[1]);
        exit(0);

    } else {
        close(fd1[0]);
        close(fd2[1]);

        write(fd1[1], &size, sizeof(int));

        write(fd1[1], data, size * sizeof(int));

        int final[MAX_ARRAY_SIZE];
        read(fd2[0], final, size * sizeof(int));

        for(int i=0; i<size; i++){ 
            printf("%d ", final[i]);
        }
        printf("\n");

        close(fd1[1]);
        close(fd2[0]);
        wait(NULL);
    }
    return 0;
}