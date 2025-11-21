#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_COMMAND 256
#define MAX_NAME_LENGTH 50
#define BLOCK_SIZE 512
#define NUM_BLOCKS 500


typedef enum {
    NODE_FILE,
    NODE_DIRECTORY
} NodeType;

struct freeBlockNode {
    int index;
    struct freeBlockNode *next;
    struct freeBlockNode *previous;
};

typedef struct FileNode {
    char name[MAX_NAME_LENGTH + 1];
    NodeType type;
    long contentSize;
    int blockPointers[NUM_BLOCKS];
    int blocksUsed;
    
    struct FileNode* parent;
    struct FileNode* children;
    struct FileNode* next; 
} FileNode;

unsigned char virtualMemory[NUM_BLOCKS][BLOCK_SIZE];

struct freeBlockNode* freeBlockHead = NULL;
struct freeBlockNode* freeBlockTail = NULL;

FileNode* root = NULL;
FileNode* cwd = NULL;

int freeBlockCount = 0;


FileNode* createNode(const char* name, NodeType type, FileNode* parent) {
    FileNode* newNode = (FileNode*)malloc(sizeof(FileNode));
    if (newNode == NULL) {
        printf("Memory is full!\n");
        exit(1);
    }
    
    strncpy(newNode->name, name, MAX_NAME_LENGTH);
    newNode->name[MAX_NAME_LENGTH] = '\0';
    
    newNode->type = type;
    newNode->contentSize = 0;
    newNode->blocksUsed = 0;
    newNode->parent = parent;
    newNode->children = NULL;
    
    newNode->next = NULL; 
    
    return newNode;
}

void initializeFreeBlocks() {
    struct freeBlockNode *prev = NULL;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        struct freeBlockNode *newNode = (struct freeBlockNode *)malloc(sizeof(struct freeBlockNode));
        newNode->index = i;
        newNode->next = NULL;
        newNode->previous = prev;
        
        if (prev != NULL) {
            prev->next = newNode;
        } else {
            freeBlockHead = newNode;
        }
        prev = newNode;
    }
    freeBlockTail = prev;
    freeBlockCount = NUM_BLOCKS;
    printf("Initialized memory.\n", freeBlockCount);
}

int allocateBlock() {
    if (freeBlockHead == NULL) {
        return -1;
    }

    struct freeBlockNode* blockToAllocate = freeBlockHead;
    int blockIndex = blockToAllocate->index;

    freeBlockHead = blockToAllocate->next;

    if (freeBlockHead != NULL) {
        freeBlockHead->previous = NULL;
    } else {
        freeBlockTail = NULL;
    }
    
    free(blockToAllocate);
    freeBlockCount--;
    
    return blockIndex;
}

void freeBlock(int blockIndex) {
    struct freeBlockNode *newNode = (struct freeBlockNode *)malloc(sizeof(struct freeBlockNode));
    if (newNode == NULL) {
        printf("Memory is full!\n");
        return;
    }
    newNode->index = blockIndex;
    newNode->next = NULL;
    newNode->previous = freeBlockTail;

    if (freeBlockTail != NULL) {
        freeBlockTail->next = newNode;
    } else {
        freeBlockHead = newNode;
    }
    
    freeBlockTail = newNode;
    freeBlockCount++;
}

void removeFreeBlocks() {
    struct freeBlockNode* current = freeBlockHead;
    struct freeBlockNode* next;
    
    printf("Freeing blocks ...\n");
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    freeBlockHead = NULL;
    freeBlockTail = NULL;
}

void freeNodesRecursive(FileNode* node) {
    if (node == NULL) {
        return;
    }

    if (node->children != NULL) {
        FileNode* child = node->children;
        FileNode* nextChild;
        
        FileNode* lastChild = child;
        while (lastChild->next != child) {
            lastChild = lastChild->next;
        }
        lastChild->next = NULL;
        
        while (child != NULL) {
            nextChild = child->next;
            freeNodesRecursive(child);
            child = nextChild;
        }
    }
    free(node);
}


FileNode* findNodeByName(FileNode* parentDir, const char* name) {
    if (parentDir->children == NULL) {
        return NULL;
    }

    FileNode* current = parentDir->children;

    do {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    } while (current != parentDir->children);

    return NULL;
}

void removeNodeFromParent(FileNode* nodeToRemove) {
    FileNode* parentDir = nodeToRemove->parent;

    if (parentDir == NULL || parentDir->children == NULL) {
        return;
    }

    FileNode* current = parentDir->children;
    FileNode* prev = NULL;

    do {
        if (current == nodeToRemove) {
            break;
        }
        prev = current;
        current = current->next;
    } while (current != parentDir->children);

    if (current != nodeToRemove) {
        return;
    }

    if (current->next == current) {
        parentDir->children = NULL;
    }
    else if (current == parentDir->children) {
        FileNode* lastNode = current;
        while (lastNode->next != parentDir->children) {
            lastNode = lastNode->next;
        }
        parentDir->children = current->next;
        lastNode->next = parentDir->children;
    }
    else {
        prev->next = current->next;
    }

    nodeToRemove->next = NULL;
    nodeToRemove->parent = NULL;
}

void makeDirectory(char args1[], char args2[], FileNode* currentDir) {
    if (strlen(args1) > MAX_NAME_LENGTH) {
        printf("Cannot have more than %d characters.\n", MAX_NAME_LENGTH);
        return;
    }

    if (findNodeByName(currentDir, args1) != NULL) {
        printf("Directory already exists.\n", args1);
        return;
    }

    FileNode* newDir = createNode(args1, NODE_DIRECTORY, currentDir);

    if (currentDir->children == NULL) {
        currentDir->children = newDir;
        newDir->next = newDir;
    } 
    else {
        FileNode* current = currentDir->children;
        while (current->next != currentDir->children) {
            current = current->next;
        }
        
        current->next = newDir;
        newDir->next = currentDir->children;
    }

    printf("Directory created.\n", args1);
}

void makeFile(char args1[], char args2[], FileNode* currentDir) {
    if (strlen(args1) > MAX_NAME_LENGTH) {
        printf("Cannot have more then %d characters.\n", MAX_NAME_LENGTH);
        return;
    }

    if (findNodeByName(currentDir, args1) != NULL) {
        printf("File already exists.\n", args1);
        return;
    }

    FileNode* newFile = createNode(args1, NODE_FILE, currentDir);

    if (currentDir->children == NULL) {
        currentDir->children = newFile;
        newFile->next = newFile;
    } 
    else {
        FileNode* current = currentDir->children;
        while (current->next != currentDir->children) {
            current = current->next;
        }
        
        current->next = newFile;
        newFile->next = currentDir->children;
    }

    printf("File created.\n", args1);
}

void listContents(char args1[], char args2[], FileNode* currentDir) {
    if (currentDir->children == NULL) {
        printf("No files exist\n");
        return;
    }

    FileNode* current = currentDir->children;
    
    do {
        printf("%s", current->name);
        if (current->type == NODE_DIRECTORY) {
            printf("/");
        }
        printf("\n");
        
        current = current->next;
    } while (current != currentDir->children);
}

void changeDirectory(char args1[], char args2[]) {
    if (strcmp(args1, "..") == 0) {
        if (cwd->parent != NULL) {
            cwd = cwd->parent;
        }
    } else if (strcmp(args1, "/") == 0) {
        cwd = root;
    } else {
        FileNode* target = findNodeByName(cwd, args1);
        
        if (target == NULL) {
            printf("Directory not found.\n", args1);
        } else if (target->type != NODE_DIRECTORY) {
            printf("Not a directory.\n", args1);
        } else {
            cwd = target;
        }
    }
}

void printPathRecursive(FileNode* node) {
    if (node == NULL || node->parent == NULL) {
        printf("/");
        return;
    }
    
    printPathRecursive(node->parent);
    
    if (node->parent != root) {
        printf("/");
    }
    printf("%s", node->name);
}

void displayCurrentPath(char args1[], char args2[]) {
    printPathRecursive(cwd);
    printf("\n");
}

void showDiskUsage(char args1[], char args2[], FileNode* currentDir) {
    int usedBlocks = NUM_BLOCKS - freeBlockCount;
    double usagePercent = 0.0;
    
    if (NUM_BLOCKS > 0) {
        usagePercent = ((double)usedBlocks / NUM_BLOCKS) * 100.0;
    }
    
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks:  %d\n", usedBlocks);
    printf("Free Blocks:  %d\n", freeBlockCount);
    printf("Disk Usage:   %.2f%%\n", usagePercent);
}

void removeDirectory(char args1[], char args2[], FileNode* currentDir) {
    FileNode* dir = findNodeByName(currentDir, args1);

    if (dir == NULL) {
        printf("Directory not found.\n", args1);
        return;
    }
    if (dir->type != NODE_DIRECTORY) {
        return;
    }
    if (dir->children != NULL) {
        printf("Error: Directory is not empty.\n", args1);
        return;
    }

    removeNodeFromParent(dir);
    free(dir);

    printf("Directory removed.\n");
}

void removeFile(char args1[], char args2[], FileNode* currentDir) {
    FileNode* file = findNodeByName(currentDir, args1);

    if (file == NULL) {
        printf("File  not found.\n", args1);
        return;
    }
    if (file->type != NODE_FILE) {
        printf("Use rmdir.\n", args1);
        return;
    }

    for (int i = 0; i < file->blocksUsed; i++) {
        freeBlock(file->blockPointers[i]);
    }

    removeNodeFromParent(file);
    free(file);

    printf("File deleted successfully.\n");
}

void readFile(char args1[], char args2[], FileNode* currentDir) {
    FileNode* file = findNodeByName(currentDir, args1);
    
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", args1);
        return;
    }
    if (file->type != NODE_FILE) {
        printf("Error: '%s' is a directory.\n", args1);
        return;
    }
    
    if (file->contentSize == 0) {
        printf("\n");
        return;
    }
    
    long bytesRead = 0;
    for (int i = 0; i < file->blocksUsed; i++) {
        int blockIndex = file->blockPointers[i];
        int bytesToRead = (file->contentSize - bytesRead > BLOCK_SIZE) ? BLOCK_SIZE : (file->contentSize - bytesRead);
        
        for(int j = 0; j < bytesToRead; j++) {
            putchar(virtualMemory[blockIndex][j]);
        }
        bytesRead += bytesToRead;
    }
    printf("\n");
}

void writeFile(char args1[], char args2[], FileNode* currentDir) {
    FileNode* file = findNodeByName(currentDir, args1);

    if (file == NULL) {
        printf("Error: File '%s' not found.\n", args1);
        return;
    }
    if (file->type != NODE_FILE) {
        printf("Error: '%s' is a directory.\n", args1);
        return;
    }

    for (int i = 0; i < file->blocksUsed; i++) {
        freeBlock(file->blockPointers[i]);
    }
    file->blocksUsed = 0;
    file->contentSize = 0;

    long dataSize = (long)strlen(args2);
    if (dataSize == 0) {
        printf("Data written successfully (size=0 bytes).\n");
        return;
    }

    int blocksNeeded = (dataSize / BLOCK_SIZE) + (dataSize % BLOCK_SIZE != 0);

    if (blocksNeeded > freeBlockCount) {
        printf("Error: Not enough free space on disk.\n");
        return;
    }
    if (blocksNeeded > NUM_BLOCKS) {
         printf("Error: Data size exceeds maximum file size.\n");
         return;
    }
    
    long bytesWritten = 0;
    for (int i = 0; i < blocksNeeded; i++) {
        int blockIndex = allocateBlock();
        
        file->blockPointers[i] = blockIndex;
        file->blocksUsed++;
        
        int bytesToCopy = (dataSize - bytesWritten > BLOCK_SIZE) ? BLOCK_SIZE : (dataSize - bytesWritten);
        memcpy(virtualMemory[blockIndex], args2 + bytesWritten, bytesToCopy);
        bytesWritten += bytesToCopy;
    }
    
    file->contentSize = dataSize;
    printf("Data written successfully (size=%ld bytes).\n", dataSize);
}

int main() {
    char inputCommand[MAX_INPUT_COMMAND];
    char args1[MAX_INPUT_COMMAND];
    char args2[MAX_INPUT_COMMAND];

    printf("=== Virtual File System (VFS) ===\n");
    printf("Type 'exit' to quit.\n");
    
    initializeFreeBlocks();
    root = createNode("/", NODE_DIRECTORY, NULL); 
    cwd = root; 
    
    while (1) {
        printPathRecursive(cwd);
        printf("> "); 
        fflush(stdout);

        strcpy(args1, "");
        strcpy(args2, "");

        if (scanf("%s", inputCommand) == EOF) {
            strcpy(inputCommand, "exit");
            printf("exit\n");
        }
            
        if (strcmp(inputCommand, "exit") == 0) {
            printf("Freeing all memory...\n");
            removeFreeBlocks();
            freeNodesRecursive(root);
            printf("Exiting VFS!\n");
            break;
        } 
        else if (strcmp(inputCommand, "mkdir") == 0) {
            scanf("%s", args1);
            makeDirectory(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "cd") == 0) {
            scanf("%s", args1);
            changeDirectory(args1, args2);
        } 
        else if (strcmp(inputCommand, "ls") == 0) {
            listContents(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "create") == 0) {
            scanf("%s", args1);
            makeFile(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "write") == 0) {
            scanf("%s", args1);
            scanf(" %[^\n]", args2);
            writeFile(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "delete") == 0) {
            scanf("%s", args1);
            removeFile(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "read") == 0) {
            scanf("%s", args1);
            readFile(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "rmdir") == 0) {
            scanf("%s", args1);
            removeDirectory(args1, args2, cwd);
        } 
        else if (strcmp(inputCommand, "pwd") == 0) {
            displayCurrentPath(args1, args2);
        } 
        else if (strcmp(inputCommand, "df") == 0) {
            showDiskUsage(args1, args2, cwd);
        } 
        else {
            printf("Invalid command!\n");
        }

        while (getchar() != '\n');
    }

    return 0;
}