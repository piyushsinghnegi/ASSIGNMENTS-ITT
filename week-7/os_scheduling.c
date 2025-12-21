#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LENGTH 100
#define MAX_INPUT_LENGTH 200
#define HASH_SIZE 100

#define STATE_READY 0
#define STATE_RUNNING 1
#define STATE_WAITING 2
#define STATE_TERMINATED 3

struct process {
    int id;
    char name[MAX_LENGTH];
    int totalBurstTime;
    int remainingBurstTime;
    
    int ioStartTime;
    int ioDuration;
    int ioRemaining;
    
    int killTime;
    int completionTime;
    int arrivalTime;
    int state;
    
    struct process *next;
};

struct queue {
    struct process *front;
    struct process *rear;
};

struct queue *ready = NULL;
struct queue *waiting = NULL;
struct queue *terminated = NULL;

struct process *hashMap[HASH_SIZE]; 


void initQueues() {
    ready = (struct queue*)malloc(sizeof(struct queue));
    waiting = (struct queue*)malloc(sizeof(struct queue));
    terminated = (struct queue*)malloc(sizeof(struct queue));
    ready->front = ready->rear = NULL;
    waiting->front = waiting->rear = NULL;
    terminated->front = terminated->rear = NULL;
}

void enqueue(struct queue *q, struct process *p) {
    p->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = p;
        return;
    }
    q->rear->next = p;
    q->rear = p;
}

struct process* dequeue(struct queue *q) {
    if (q->front == NULL) return NULL;
    struct process *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    return temp;
}

int isEmpty(struct queue *q) {
    return (q->front == NULL);
}


void addInHashMap(struct process *p) {
    int index = p->id % HASH_SIZE;
    hashMap[index] = p;
}

struct process* getFromHashMap(int pid) {
    if (pid < 0 || pid >= HASH_SIZE) return NULL;
    return hashMap[pid];
}


void terminateProcess(int id, int killTime) {
    struct process *p = getFromHashMap(id);
    if (p != NULL) {
        p->killTime = killTime;
        printf("Scheduled KILL for PID %d at time %d\n", id, killTime);
    } else {
        printf("Id: %d dosent exist!\n", id);
    }
}

void startProcess(char *name, int id, int bTime, int ioStart, int ioDur) {
    struct process *newProc = (struct process*)malloc(sizeof(struct process));
    
    newProc->id = id;
    strcpy(newProc->name, name);
    newProc->totalBurstTime = bTime;
    newProc->remainingBurstTime = bTime;
    newProc->ioStartTime = ioStart;
    newProc->ioDuration = ioDur;
    newProc->ioRemaining = ioDur;
    newProc->killTime = -1; 
    newProc->arrivalTime = 0;
    newProc->state = STATE_READY;
    newProc->next = NULL;

    addInHashMap(newProc);
    enqueue(ready, newProc); 
}


void runSimulation() {
    printf("\n--- STARTING SIMULATION ---\n");
    int currentTime = 0;
    struct process *runningProcess = NULL;
    
    while (!isEmpty(ready) || !isEmpty(waiting) || runningProcess != NULL) {
        
        for(int i=0; i<HASH_SIZE; i++) {
            if(hashMap[i] != NULL && hashMap[i]->killTime == currentTime && hashMap[i]->state != STATE_TERMINATED) {
                 struct process *victim = hashMap[i];
                 
                 if (runningProcess == victim) {
                     runningProcess = NULL;
                 }
                 victim->state = STATE_TERMINATED;
                 victim->completionTime = currentTime; 
                 enqueue(terminated, victim);
                 printf("KILLED Process %d at %d\n", victim->id, currentTime);
            }
        }

        int waitingCount = 0;
        struct process *waitList[100];
        
        while(!isEmpty(waiting)) {
            waitList[waitingCount++] = dequeue(waiting);
        }

        for(int i=0; i<waitingCount; i++) {
            struct process *p = waitList[i];
            if(p->state == STATE_TERMINATED) continue;

            p->ioRemaining--; 
            
            if(p->ioRemaining <= 0) {
                p->state = STATE_READY;
                enqueue(ready, p);
            } else {
                enqueue(waiting, p);
            }
        }

        if (runningProcess == NULL && !isEmpty(ready)) {
            runningProcess = dequeue(ready);
            if(runningProcess->state == STATE_TERMINATED) {
                runningProcess = NULL; 
            } else {
                runningProcess->state = STATE_RUNNING;
            }
        }

        if (runningProcess != NULL) {
            runningProcess->remainingBurstTime--;
            
            int timeExecuted = runningProcess->totalBurstTime - runningProcess->remainingBurstTime;
            
            if (runningProcess->remainingBurstTime > 0 && 
                runningProcess->ioDuration > 0 && 
                timeExecuted == runningProcess->ioStartTime) {
                
                runningProcess->state = STATE_WAITING;
                enqueue(waiting, runningProcess);
                runningProcess = NULL;
            }
            else if (runningProcess->remainingBurstTime == 0) {
                runningProcess->state = STATE_TERMINATED;
                runningProcess->completionTime = currentTime + 1;
                enqueue(terminated, runningProcess);
                runningProcess = NULL;
            }
        }

        currentTime++;
        sleep(1);
    }
    
    printf("\n--- SIMULATION COMPLETED ---\n");
    printf("%-5s %-10s %-5s %-5s %-12s %-10s\n", "PID", "Name", "CPU", "I/O", "Turnaround", "Waiting");
    
    while(!isEmpty(terminated)) {
        struct process *p = dequeue(terminated);
        
        int turnaround = 0;
        int waitingT = 0;
        
        if (p->remainingBurstTime > 0) {
            
        } else {
             turnaround = p->completionTime - p->arrivalTime;
            waitingT = turnaround - p->totalBurstTime;
        }
        
        printf("%-5d %-10s %-5d %-5d %-12d %-10d\n", 
               p->id, p->name, p->totalBurstTime, p->ioDuration, turnaround, waitingT);
    }
}


int main() {
    char input[MAX_INPUT_LENGTH], command[MAX_LENGTH];
    int tempId, tempBurstTime, tempStart, tempDuration, tempKill;
    
    initQueues();
    
    printf("<process_name> <pid> <burst_time> <io_start_time> <io_duration>\n");
    printf("KILL <PID> <kill_time>\n");
    printf("RUN to start\n");

    while(1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "") == 0) continue;
        
        sscanf(input, "%s", command);
        
        if (strcmp(command, "RUN") == 0) {
            runSimulation();
            break;
        }
        else if (strcmp(command, "KILL") == 0) {
            int arg = sscanf(input, "%s %d %d", command, &tempId, &tempKill);
            if (arg < 3 || tempId < 0 || tempKill < 0){
                printf("Invalid input!\n");
                continue;
            }
            terminateProcess(tempId, tempKill);
        } 
        else {
            int args = sscanf(input, "%s %d %d %d %d", command, &tempId, &tempBurstTime, &tempStart, &tempDuration);
            
            if(args < 5 || tempId <0 || tempBurstTime< 0 || tempStart<0 || tempDuration<0){
                printf("Invalid input!\n");
                continue;
            }
            if(args == 3) { tempStart = 0; tempDuration = 0; }
            
            startProcess(command, tempId, tempBurstTime, tempStart, tempDuration);
        }
    }
    return 0;
}