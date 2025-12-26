#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int getAmount(){
    int value;
    while(1){
        if(scanf("%d", &value) == 1 && value > 0){
            return value; 
        }
        while(getchar() != '\n');
        printf("Invalid amount\n");
    }
}

void send_request(char *message) {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed. Is the server running?\n");
        close(sock);
        return;
    }

    send(sock, message, strlen(message), 0);
    read(sock, buffer, 1024);
    
    printf("\nServer Response: %s\n\n", buffer);

    close(sock);
}

int main() {
    int choice, amount;
    char buffer[100];

    while (1) {
        printf("--- ATM Menu ---\n1. Withdraw\n2. Deposit\n3. Balance\n4. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1){
            break;
        }

        if (choice == 4){
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter amount: ");
                amount = getAmount();
                sprintf(buffer, "WITHDRAW %d", amount);
                send_request(buffer);
                break;
            case 2:
                printf("Enter amount: ");
                amount = getAmount();
                sprintf(buffer, "DEPOSIT %d", amount);
                send_request(buffer);
                break;
            case 3:
                sprintf(buffer, "BALANCE 0");
                send_request(buffer);
                break;
            default:
                printf("Invalid option.\n");
        }
    }
    return 0;
}