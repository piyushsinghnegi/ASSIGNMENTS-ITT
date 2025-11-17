#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "Players_data.h"

#define MAX_PLAYER_ID 1000
#define MAX_PLAYERS_PER_TEAM 50
#define MAX_NAME_LENGTH 50
#define MAX_TEAMS 10
#define MIN_ID 0

typedef enum {
    BATSMAN,
    BOWLER,
    ALL_ROUNDER
} Role;

struct PlayerNode {
    int id;
    char name[MAX_NAME_LENGTH];
    char teamName[MAX_NAME_LENGTH];
    Role role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    double performanceIndex;
    struct PlayerNode *next;
};

struct Team {
    char name[MAX_NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikeRate;
    
    double totalStrikeRate;
    int battingPlayerCount;
};

struct PlayerNode *head = NULL;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Role stringToRole(const char* roleStr) {
    if (strcasecmp(roleStr, "Batsman") == 0) return BATSMAN;
    if (strcasecmp(roleStr, "Bowler") == 0) return BOWLER;
    if (strcasecmp(roleStr, "All-rounder") == 0) return ALL_ROUNDER;
    return BATSMAN;
}

const char* getRoleString(Role role) {
    switch (role) {
        case BATSMAN:
            return "Batsman";
        case BOWLER:
            return "Bowler";
        case ALL_ROUNDER:
            return "All-rounder";
        default:
            return "Unknown";
    }
}

void getValidatedString(const char* inputText, char* value) {
    int valid = 0;
    while (!valid) {
        printf("%s", inputText);
        if (scanf(" %49[^\n]", value) == 1) {
            if (strlen(value) > 0) {
                valid = 1;
            } else {
                printf("Input cannot be empty.\n");
            }
        } else {
            printf("Invalid input.\n");
        }
        clearInputBuffer();
    }
}

float calculatePerformanceIndex(struct PlayerNode *player){
    switch (player->role) {
        case BATSMAN:
            return (player->battingAverage * player->strikeRate) / 100.0f;
        case BOWLER:
            return (player->wickets * 2.0f) + (100.0f - player->economyRate);
        case ALL_ROUNDER:
            return ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wickets * 2.0f);
        default:
            return 0.0f;
    }
}

void addPlayer(const Player *player) {
    struct PlayerNode *newNode = (struct PlayerNode *)malloc(sizeof(struct PlayerNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->id = player->id;
    strcpy(newNode->name, player->name);
    strcpy(newNode->teamName, player->team);
    newNode->role = stringToRole(player->role);
    newNode->totalRuns = player->totalRuns;
    newNode->battingAverage = player->battingAverage;
    newNode->strikeRate = player->strikeRate;
    newNode->wickets = player->wickets;
    newNode->economyRate = player->economyRate;

    newNode->performanceIndex = calculatePerformanceIndex(newNode);

    newNode->next = head;
    head = newNode;
}

void displayTeamPlayers(){
    char teamInput[MAX_NAME_LENGTH];
    getValidatedString("Enter Team Name: ", teamInput);

    struct PlayerNode *temp = head;
    int found = 0;
    int totalPlayers = 0;
    double totalSR = 0;
    int battingPlayers = 0;

    printf("\nPlayers of Team %s:\n", teamInput);
    printf("================================================================================================\n");
    printf("%-4s %-20s %-12s %-6s %-6s %-6s %-6s %-6s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("================================================================================================\n");
    
    while (temp != NULL) {
        if (strcasecmp(temp->teamName, teamInput) == 0) {
            found = 1;
            totalPlayers++;
            
            printf("%-4d %-20s %-12s %-6d %-6.1f %-6.1f %-6d %-6.1f %-10.2f\n",
                   temp->id,
                   temp->name,
                   getRoleString(temp->role),
                   temp->totalRuns,
                   temp->battingAverage,
                   temp->strikeRate,
                   temp->wickets,
                   temp->economyRate,
                   temp->performanceIndex);

            if (temp->role == BATSMAN || temp->role == ALL_ROUNDER) {
                totalSR += temp->strikeRate;
                battingPlayers++;
            }
        }
        temp = temp->next;
    }
    printf("================================================================================================\n");

    if (!found) {
        printf("No players found for team %s.\n", teamInput);
    } else {
        printf("Total Players: %d\n", totalPlayers);
        float averageStrikeRate = 0.0;
        if (battingPlayers > 0) {
            averageStrikeRate = (float)totalSR / battingPlayers;
        }
        printf("Average Batting Strike Rate: %.2f\n", averageStrikeRate);
    }
}

void sortPlayers(struct PlayerNode *players[], int count) {
    for (int outerIndex = 0; outerIndex < count - 1; outerIndex++) {
        for (int innerIndex = 0; innerIndex < count - outerIndex - 1; innerIndex++) {
            if (players[innerIndex]->performanceIndex < players[innerIndex+1]->performanceIndex) {
                struct PlayerNode *temp = players[innerIndex];
                players[innerIndex] = players[innerIndex+1];
                players[innerIndex+1] = temp;
            }
        }
    }
}

void displayTeamsByStrikeRate(){
    struct Team teamStats[MAX_TEAMS];
    int teamCount = 0;

    struct PlayerNode *temp = head;
    while (temp != NULL) {
        int exists = 0;
        for (int teamIndex = 0; teamIndex < teamCount; teamIndex++) {
            if (strcasecmp(teamStats[teamIndex].name, temp->teamName) == 0) {
                exists = 1;
                break;
            }
        }
        
        if (!exists && teamCount < MAX_TEAMS) {
            strcpy(teamStats[teamCount].name, temp->teamName);
            teamStats[teamCount].totalPlayers = 0;
            teamStats[teamCount].averageBattingStrikeRate = 0.0f;
            teamStats[teamCount].totalStrikeRate = 0.0;
            teamStats[teamCount].battingPlayerCount = 0;
            teamCount++;
        }
        temp = temp->next;
    }

    temp = head;
    while (temp != NULL) {
        for (int teamIndex = 0; teamIndex < teamCount; teamIndex++) {
            if (strcasecmp(teamStats[teamIndex].name, temp->teamName) == 0) {
                teamStats[teamIndex].totalPlayers++;
                if (temp->role == BATSMAN || temp->role == ALL_ROUNDER) {
                    teamStats[teamIndex].totalStrikeRate += temp->strikeRate;
                    teamStats[teamIndex].battingPlayerCount++;
                }
                break;
            }
        }
        temp = temp->next;
    }

    for (int teamIndex = 0; teamIndex < teamCount; teamIndex++) {
        if (teamStats[teamIndex].battingPlayerCount > 0) {
            teamStats[teamIndex].averageBattingStrikeRate = (float)teamStats[teamIndex].totalStrikeRate / teamStats[teamIndex].battingPlayerCount;
        }
    }

    for (int outerIndex = 0; outerIndex < teamCount - 1; outerIndex++) {
        for (int innerIndex = 0; innerIndex < teamCount - outerIndex - 1; innerIndex++) {
            if (teamStats[innerIndex].averageBattingStrikeRate < teamStats[innerIndex+1].averageBattingStrikeRate) {
                struct Team t = teamStats[innerIndex];
                teamStats[innerIndex] = teamStats[innerIndex+1];
                teamStats[innerIndex+1] = t;
            }
        }
    }

    printf("\nTeams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("%-20s %-12s %-12s\n", "Team Name", "Avg Bat SR", "Total Players");
    printf("=========================================================\n");
    for (int teamIndex = 0; teamIndex < teamCount; teamIndex++) {
        printf("%-20s %-12.2f %-12d\n", teamStats[teamIndex].name, teamStats[teamIndex].averageBattingStrikeRate, teamStats[teamIndex].totalPlayers);
    }
    printf("=========================================================\n");
}


void displayTopKPlayersOfTeam(){
    char teamInput[MAX_NAME_LENGTH];
    int roleInt, k;
    Role role;

    getValidatedString("Enter Team Name: ", teamInput);
    
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    while(scanf("%d", &roleInt) != 1 || roleInt < 1 || roleInt > 3) {
        printf("Invalid input: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    role = (Role)(roleInt - 1);
    
    printf("Enter number of players (K): ");
    while(scanf("%d", &k) != 1 || k < 1) {
        printf("Invalid input. Enter a positive number: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    struct PlayerNode *matchingPlayers[playerCount];
    int count = 0;

    struct PlayerNode *temp = head;
    while (temp != NULL) {
        if (strcasecmp(temp->teamName, teamInput) == 0 && temp->role == role) {
            matchingPlayers[count] = temp;
            count++;
        }
        temp = temp->next;
    }

    if (count == 0) {
        printf("No players found for team %s with role %s.\n", teamInput, getRoleString(role));
        return;
    }

    sortPlayers(matchingPlayers, count);

    printf("\nTop %d %s of Team %s:\n", k, getRoleString(role), teamInput);
    printf("================================================================================================\n");
    printf("%-4s %-20s %-12s %-6s %-6s %-6s %-6s %-6s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("================================================================================================\n");
    
    for (int playerIndex = 0; playerIndex < k && playerIndex < count; playerIndex++) {
        struct PlayerNode *player = matchingPlayers[playerIndex];
        printf("%-4d %-20s %-12s %-6d %-6.1f %-6.1f %-6d %-6.1f %-10.2f\n",
               player->id, player->name, getRoleString(player->role), player->totalRuns, player->battingAverage,
               player->strikeRate, player->wickets, player->economyRate, player->performanceIndex);
    }
    printf("================================================================================================\n");
}


void displayAllPlayersByRole() {
    int roleInt;
    Role role;
    
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    while(scanf("%d", &roleInt) != 1 || roleInt < 1 || roleInt > 3) {
        printf("Invalid input. Enter 1, 2, or 3: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    role = (Role)(roleInt - 1);

    struct PlayerNode *matchingPlayers[playerCount];
    int count = 0;

    struct PlayerNode *temp = head;
    while (temp != NULL) {
        if (temp->role == role) {
            matchingPlayers[count] = temp;
            count++;
        }
        temp = temp->next;
    }

    if (count == 0) {
        printf("No players found for role %s.\n", getRoleString(role));
        return;
    }

    sortPlayers(matchingPlayers, count);

    printf("\nAll %s of all teams:\n", getRoleString(role));
    printf("============================================================================================================\n");
    printf("%-4s %-20s %-12s %-12s %-6s %-6s %-6s %-6s %-6s %-10s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("============================================================================================================\n");
    
    for (int playerIndex = 0; playerIndex < count; playerIndex++) {
        struct PlayerNode *p = matchingPlayers[playerIndex];
        printf("%-4d %-20s %-12s %-12s %-6d %-6.1f %-6.1f %-6d %-6.1f %-10.2f\n",
               p->id, p->name, p->teamName, getRoleString(p->role), p->totalRuns, p->battingAverage,
               p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
    }
    printf("============================================================================================================\n");
}

void initializeTeamsAndPlayers(){
    for (int playerIndex = 0; playerIndex < playerCount; playerIndex++) {
        addPlayer(&players[playerIndex]);
    }
    printf("%d Players Loaded\n", playerCount);
}

void freePlayers() {
    struct PlayerNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(){
    initializeTeamsAndPlayers();

    int choice;

    while(1){
        printf("\n============================================\n");
        printf("    ICC Player Analyzer\n");
        printf("============================================\n");
        printf(" 1. Display Players of a Specific Team\n");
        printf(" 2. Display Teams by Average SR\n");
        printf(" 3. Display Top K Players of a Team by Role\n");
        printf(" 4. Display All Players of Role (Global)\n");
        printf(" 5. Exit\n");
        printf("============================================\n");
        printf("Enter your choice: ");

        if(scanf("%d",&choice) != 1){
            printf("Invalid input!\n Please enter numbers only.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                displayTeamPlayers(); 
                break;
            case 2:
                displayTeamsByStrikeRate(); 
                break;
            case 3:
                displayTopKPlayersOfTeam(); 
                break;
            case 4:
                displayAllPlayersByRole(); 
                break;
            case 5:
                freePlayers();
                printf("Freed the memory allocated. Exiting.\n");
                exit(0);
            default:
                printf("Invalid choice! Please enter a number from 1 to 5.\n");
        }
    }
    return 0;
}