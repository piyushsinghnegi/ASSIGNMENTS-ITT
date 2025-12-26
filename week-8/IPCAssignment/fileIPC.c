#include <stdio.h>
#include <stdlib.h>
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
    int data[MAX_ARRAY_SIZE], size;
    
    printf("Enter size of array: ");
    size = takeIntInput();
    
    if(size > MAX_ARRAY_SIZE){
        printf("Size exceeds limit\n");
        exit(1);
    }

    takeInput(data, size);
    
    pid_t pid = fork();

    if (pid == 0) {
        sortArray(data, size);
        
        FILE *filePtr = fopen("data.txt", "w");
        if (filePtr == NULL) { 
            perror("File error");
            exit(1);
        }
        
        for(int i=0; i<size; i++){
            fprintf(filePtr, "%d ", data[i]);
        } 
        
        fclose(filePtr);
        exit(0);

    } else {
        wait(NULL);
        
        FILE *filePtr = fopen("data.txt", "r");
        if (filePtr == NULL) {
            perror("File error");
            exit(1);
        }
        
        int val;
        while(fscanf(filePtr, "%d", &val) != EOF) {
            printf("%d ", val);
        }

        printf("\n");
        fclose(filePtr);
    }
    return 0;
}