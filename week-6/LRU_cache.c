#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STRING_LENGTH 100
#define MAX_CAPACITY 1000

struct Node{
    int key;
    char value[MAX_STRING_LENGTH];
    struct Node *next;
    struct Node *previous;
};

struct Node *front = NULL;
struct Node *rear = NULL;

struct HashNode {
    int key;
    struct Node *linklistNode;
    struct HashNode *next;
};

struct HashNode *hashTable[MAX_CAPACITY];
int cacheCapacity = 0;
int currentSize = 0;


void addToFront(struct Node *node) {
    node->next = front;
    node->previous = NULL;
    if(front != NULL)
        front->previous = node;

    front = node;

    if(rear == NULL)
        rear = node;
}

void removeNode(struct Node *node) {
    if(node->previous != NULL)
        node->previous->next = node->next;
    else
        front = node->next;

    if(node->next != NULL)
        node->next->previous = node->previous;
    else
        rear = node->previous;
}

void insertToHashMap(int key, struct Node *linklistNode){
    int index = key % cacheCapacity;
    struct HashNode *newHash = malloc(sizeof(struct HashNode));
    
    newHash->key = key;
    newHash->linklistNode = linklistNode;
    newHash->next = hashTable[index];
    
    hashTable[index] = newHash;
}

void deleteFromHashMap(int key){
    int index = key % cacheCapacity;
    struct HashNode *nodeAddress = hashTable[index];
    struct HashNode *prev = NULL;

    while(nodeAddress){
        if(nodeAddress->key == key){
            if(prev == NULL)
                hashTable[index] = nodeAddress->next;
            else
                prev->next = nodeAddress->next;

            free(nodeAddress);
            return;
        }
        prev = nodeAddress;
        nodeAddress = nodeAddress->next;
    }
}

struct Node* searchKey(int key){
    int index = key % cacheCapacity;
    struct HashNode *node = hashTable[index];

    while(node){
        if(node->key == key)
            return node->linklistNode;

        node = node->next;
    }
    return NULL;
}


void removeLRU(){
    if(rear == NULL) return;

    struct Node *deleteNode = rear;

    removeNode(deleteNode);
    deleteFromHashMap(deleteNode->key);

    free(deleteNode);
    currentSize--;
}


void putInHashMap(int key, char *value){
    struct Node *nodeAddress = searchKey(key);

    if(nodeAddress != NULL){
        strcpy(nodeAddress->value, value);
        removeNode(nodeAddress);
        addToFront(nodeAddress);
        return;
    }

    if(currentSize == cacheCapacity){
        removeLRU();
    }

    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->next = newNode->previous = NULL;

    addToFront(newNode);
    insertToHashMap(key, newNode);

    currentSize++;
}

void getFromHashMap(int key){
    struct Node *nodeAddress = searchKey(key);

    if(nodeAddress == NULL){
        printf("NULL\n");
        return;
    }

    printf("%s\n", nodeAddress->value);

    removeNode(nodeAddress);
    addToFront(nodeAddress);
}

void freeMemory(){
    struct Node *deleteNode = front;
    while(deleteNode) {
        struct Node *next = deleteNode->next;
        free(deleteNode);
        deleteNode = next;
    }
    front = rear = NULL;
    
    for (int node = 0; node < cacheCapacity; node++) {
        struct HashNode *deleteHash = hashTable[node];
        while(deleteHash) {
            struct HashNode *next = deleteHash->next;
            free(deleteHash);
            deleteHash = next;
        }
        hashTable[node] = NULL;
    }
}

int main() {
    char inputCommand[MAX_STRING_LENGTH];
    char keyword[MAX_STRING_LENGTH];
    int inputCachesize;
    int inputKey;
    char inputValue[MAX_STRING_LENGTH];

    while(1){
        strcpy(inputCommand, "");
        printf("> ");
        fgets(inputCommand, sizeof(inputCommand), stdin);
        
        if(sscanf(inputCommand, "%s", keyword) != 1){
            continue;
        }
        
        if(strcmp(keyword, "createCache") == 0){
            if(sscanf(inputCommand, "%*s %d", &inputCachesize) == 1){
                if(inputCachesize < MAX_CAPACITY && inputCachesize > 0){
                    cacheCapacity = inputCachesize;
                    printf("Cache created with size: %d\n", inputCachesize);
                } else {
                    printf("Should be in range: [%d,%d]\n",0,MAX_CAPACITY);
                }
            } else {
                printf("Invalid Input!\n");
            }
        } 
        
        else if(strcmp(keyword, "put") == 0){
            if(sscanf(inputCommand, "%*s %d %s", &inputKey, inputValue) == 2 && inputKey > 0){
                if(cacheCapacity == 0){
                    printf("Cache not created!\n");
                } else {
                    putInHashMap(inputKey, inputValue);
                }
            } else {
                printf("Invalid Input!\n");
            }
        } 
        
        else if(strcmp(keyword, "get") == 0){
            if(sscanf(inputCommand, "%*s %d", &inputKey) == 1 && inputKey > 0){
                if(cacheCapacity == 0){
                    printf("Cache not created!\n");
                } else {
                    getFromHashMap(inputKey);
                }
            } else {
                printf("Invalid Input\n");
            }
        } 
        
        else if(strcmp(keyword, "exit") == 0){
            printf("Freeing memory...\n");
            freeMemory();
            printf("Exiting ...\n");
            exit(0);
        } 
        
        else {
            printf("Invalid keyword!\n");
        }
    }

    return 0;
}