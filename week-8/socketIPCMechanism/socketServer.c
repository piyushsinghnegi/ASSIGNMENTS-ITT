#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define DB_FILE "../resource/accountDB.txt" 

pthread_mutex_t lock;

void *client_handler(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);
    
    char buffer[1024] = {0};
    char response[1024] = {0};
    int balance = 0;
    
    int bytes_read = read(client_sock, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0';
    
    if (bytes_read <= 0) {
        close(client_sock);
        return NULL;
    }

    char command[50];
    int amount = 0;
    sscanf(buffer, "%s %d", command, &amount);

    pthread_mutex_lock(&lock);

    FILE *fp = fopen(DB_FILE, "r");
    if (fp == NULL) {
        balance = 0;
    } else {
        fscanf(fp, "%d", &balance);
        fclose(fp);
    }

    if (strcmp(command, "BALANCE") == 0) {
        sprintf(response, "Current Balance: %d", balance);
    } 
    else if (strcmp(command, "DEPOSIT") == 0) {
        balance += amount;
        fp = fopen(DB_FILE, "w");
        fprintf(fp, "%d", balance);
        fclose(fp);
        sprintf(response, "Deposited %d. New Balance: %d", amount, balance);
    } 
    else if (strcmp(command, "WITHDRAW") == 0) {
        if (amount > balance) {
            sprintf(response, "Insufficient Funds! Balance: %d", balance);
        } else {
            balance -= amount;
            fp = fopen(DB_FILE, "w");
            fprintf(fp, "%d", balance);
            fclose(fp);
            sprintf(response, "Withdrawn %d. New Balance: %d", amount, balance);
        }
    } else {
        sprintf(response, "Unknown Command");
    }

    pthread_mutex_unlock(&lock);

    write(client_sock, response, strlen(response));
    close(client_sock);
    
    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) { 
        perror("Socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("ATM Server running on Port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New Client Connected (ID: %d)\n", client_fd);

        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_fd;
        
        if (pthread_create(&thread_id, NULL, client_handler, (void*)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
        }
        
        pthread_detach(thread_id);
    }

    return 0;
}