#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARRAY_SIZE 100

struct msg_buffer {
    long msg_type;
    int size;
    int data[MAX_ARRAY_SIZE];
} message;

void sortArray(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
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
    int msgid = msgget(key, 0666 | IPC_CREAT);
    
    int size;
    printf("Enter size of array: ");
    size = takeIntInput();

    if(size > MAX_ARRAY_SIZE){
        printf("Size exceeds limit\n");
        exit(1);
    }

    int inputData[MAX_ARRAY_SIZE];
    takeInput(inputData, size);

    for(int i=0; i<size; i++){ 
        printf("%d ", inputData[i]);
    }
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) {
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        
        sortArray(message.data, message.size);

        message.msg_type = 2;
        msgsnd(msgid, &message, sizeof(message), 0);
        
        exit(0);

    } else {
        message.msg_type = 1;
        message.size = size;
        for(int i=0; i<size; i++){
            message.data[i] = inputData[i]; 
        }
        
        msgsnd(msgid, &message, sizeof(message), 0);
        
        wait(NULL);
        
        msgrcv(msgid, &message, sizeof(message), 2, 0);

        for(int i=0; i<message.size; i++){ 
            printf("%d ", message.data[i]);
        }
        printf("\n");
        
        msgctl(msgid, IPC_RMID, NULL);
    }
    return 0;
}