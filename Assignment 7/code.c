#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Players_data.h"

#define NAME_LEN 100
#define TEAM_NAME_LEN 50
#define ROLE_LEN 20
#define MAX_TEAMS 20

typedef enum RoleType
{
    ROLE_BATSMAN = 1,
    ROLE_BOWLER = 2,
    ROLE_ALLROUNDER = 3,
    ROLE_UNKNOWN = -1
} RoleType;

typedef struct PlayerNode
{
    int id;
    char name[NAME_LEN];
    char teamName[TEAM_NAME_LEN];
    RoleType role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode *next;
} PlayerNode;

typedef struct TeamStruct
{
    int teamId;
    char name[TEAM_NAME_LEN];
    int totalPlayers;
    int battingPlayerCount;
    float averageBattingStrikeRate;
    PlayerNode *batsmanHead;
    PlayerNode *bowlerHead;
    PlayerNode *allrounderHead;
} TeamStruct;

typedef struct SystemContextData
{
    TeamStruct *teams;
    int teamCount;
    PlayerNode *masterHead;
    int totalPlayers;
} SystemContextData;

typedef struct HeapItem
{
    PlayerNode *node;
    int teamIndex;
} HeapItem;

bool initContext(SystemContextData *context);
void freeContext(SystemContextData *context);

void getValidInteger(int *out);
void getValidFloat(float *out);
RoleType stringToRole(const char *str);
const char *roleToString(RoleType role);
float calculatePerformanceIndexNode(PlayerNode *playerNode);
PlayerNode *createPlayerNodeFromPlayer(const Player *p);
PlayerNode *createPlayerNodeFromInput(void);
void insertIntoMasterListById(PlayerNode **headReference, PlayerNode *playerNode);
void insertIntoSortedRoleList(PlayerNode **headReference, PlayerNode *playerNode);
TeamStruct *searchTeamByIdBinary(SystemContextData *context, int teamId);
void addPlayerByID(SystemContextData *context);
void displayPlayersOfTeam(SystemContextData *context);
void displayTeamsByAvgStrikeRate(SystemContextData *context);
void displayTopKPlayersOfTeamByRole(SystemContextData *context);
void displayAllPlayersOfRole(SystemContextData *context);
void printPlayerTableHeader(void);
void displayPlayerDetailsLine(PlayerNode *playerNode, const char *teamName);
void heapify(HeapItem heap[], int size, int i);
void buildHeap(HeapItem heap[], int size);
HeapItem extractMax(HeapItem heap[], int *size);
void pushHeap(HeapItem heap[], int *size, HeapItem item);
void swapTeam(TeamStruct *a, TeamStruct *b);
int partitionIndex(TeamStruct teamArray[], int low, int high);
void quickSort(TeamStruct teamArray[], int low, int high);

void getValidInteger(int *out)
{
    while (scanf("%d", out) != 1)
    {
        printf("Invalid input. Enter integer: ");
        while (getchar() != '\n')
        {
            ;
        }
    }
    while (getchar() != '\n')
    {
        ;
    }
}

void getValidFloat(float *out)
{
    while (scanf("%f", out) != 1)
    {
        printf("Invalid input. Enter float: ");
        while (getchar() != '\n')
        {
            ;
        }
    }
    while (getchar() != '\n')
    {
        ;
    }
}

RoleType stringToRole(const char *str)
{
    if (strcmp(str, "Batsman") == 0)
    {
        return ROLE_BATSMAN;
    }
    if (strcmp(str, "Bowler") == 0)
    {
        return ROLE_BOWLER;
    }
    if (strcasecmp(str, "All-rounder") == 0 || strcasecmp(str, "Allrounder") == 0 || strcasecmp(str, "All rounder") == 0 || strcasecmp(str, "All-Rounder") == 0 || strcasecmp(str, "All Rounder") == 0)
    {
        return ROLE_ALLROUNDER;
    }
    return ROLE_UNKNOWN;
}

const char *roleToString(RoleType role)
{
    switch (role)
    {
    case ROLE_BATSMAN:
    {
        return "Batsman";
    }
    case ROLE_BOWLER:
    {
        return "Bowler";
    }
    case ROLE_ALLROUNDER:
    {
        return "All-rounder";
    }
    default:
    {
        return "Unknown";
    }
    }
}

float calculatePerformanceIndexNode(PlayerNode *playerNode)
{
    if (playerNode == NULL)
    {
        return 0.0f;
    }
    if (playerNode->role == ROLE_BATSMAN)
    {
        playerNode->performanceIndex = (playerNode->battingAverage * playerNode->strikeRate) / 100.0f;
    }
    else if (playerNode->role == ROLE_BOWLER)
    {
        playerNode->performanceIndex = (playerNode->wickets * 2.0f) + (100.0f - playerNode->economyRate);
    }
    else if (playerNode->role == ROLE_ALLROUNDER)
    {
        playerNode->performanceIndex = ((playerNode->battingAverage * playerNode->strikeRate) / 100.0f) + (playerNode->wickets * 2.0f);
    }
    else
    {
        playerNode->performanceIndex = 0.0f;
    }
    return playerNode->performanceIndex;
}

PlayerNode *createPlayerNodeFromPlayer(const Player *p)
{
    if (p == NULL)
    {
        return NULL;
    }
    PlayerNode *playerNode = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (playerNode == NULL)
    {
        return NULL;
    }
    playerNode->id = p->id;
    strncpy(playerNode->name, p->name, NAME_LEN - 1);
    playerNode->name[NAME_LEN - 1] = '\0';
    strncpy(playerNode->teamName, p->team, TEAM_NAME_LEN - 1);
    playerNode->teamName[TEAM_NAME_LEN - 1] = '\0';
    playerNode->role = stringToRole(p->role);
    playerNode->totalRuns = p->totalRuns;
    playerNode->battingAverage = p->battingAverage;
    playerNode->strikeRate = p->strikeRate;
    playerNode->wickets = p->wickets;
    playerNode->economyRate = p->economyRate;
    playerNode->next = NULL;
    calculatePerformanceIndexNode(playerNode);
    return playerNode;
}

PlayerNode *createPlayerNodeFromInput()
{
    PlayerNode *playerNode = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (playerNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    printf("Player Id : ");
    getValidInteger(&playerNode->id);
    printf("Player Name : ");
    scanf(" %99[^\n]", playerNode->name);
    getchar();
    int roleChoice = 0;
    while (1)
    {
        printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        getValidInteger(&roleChoice);
        if (roleChoice >= 1 && roleChoice <= 3)
        {
            break;
        }
        else
        {
            printf("Invalid role. Please try again.\n");
        }
    }
    if (roleChoice == 1)
    {
        playerNode->role = ROLE_BATSMAN;
    }
    else if (roleChoice == 2)
    {
        playerNode->role = ROLE_BOWLER;
    }
    else
    {
        playerNode->role = ROLE_ALLROUNDER;
    }
    printf("Total Runs : ");
    getValidInteger(&playerNode->totalRuns);
    printf("Batting Average : ");
    getValidFloat(&playerNode->battingAverage);
    printf("Strike Rate : ");
    getValidFloat(&playerNode->strikeRate);
    printf("Total Wickets : ");
    getValidInteger(&playerNode->wickets);
    printf("Economy Rate : ");
    getValidFloat(&playerNode->economyRate);
    playerNode->next = NULL;
    calculatePerformanceIndexNode(playerNode);
    return playerNode;
}

void insertIntoMasterListById(PlayerNode **headReference, PlayerNode *playerNode)
{
    if (headReference == NULL || playerNode == NULL)
    {
        return;
    }
    PlayerNode *head = *headReference;
    if (head == NULL || playerNode->id < head->id)
    {
        playerNode->next = head;
        *headReference = playerNode;
        return;
    }
    PlayerNode *current = head;
    while (current->next != NULL && current->next->id < playerNode->id)
    {
        current = current->next;
    }
    playerNode->next = current->next;
    current->next = playerNode;
}

void insertIntoSortedRoleList(PlayerNode **headReference, PlayerNode *playerNode)
{
    if (headReference == NULL || playerNode == NULL)
    {
        return;
    }
    PlayerNode *head = *headReference;
    if (head == NULL || playerNode->performanceIndex > head->performanceIndex)
    {
        playerNode->next = head;
        *headReference = playerNode;
        return;
    }
    PlayerNode *current = head;
    while (current->next != NULL && current->next->performanceIndex >= playerNode->performanceIndex)
    {
        current = current->next;
    }
    playerNode->next = current->next;
    current->next = playerNode;
}

TeamStruct *searchTeamByIdBinary(SystemContextData *context, int teamId)
{
    if (context == NULL || context->teams == NULL)
    {
        return NULL;
    }
    int low = 0;
    int high = context->teamCount - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (context->teams[mid].teamId == teamId)
        {
            return &context->teams[mid];
        }
        if (context->teams[mid].teamId < teamId)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return NULL;
}

bool initContext(SystemContextData *context)
{
    if (context == NULL)
    {
        return false;
    }
    context->teamCount = teamCount;
    if (context->teamCount <= 0 || context->teamCount > MAX_TEAMS)
    {
        return false;
    }
    context->teams = (TeamStruct *)malloc(sizeof(TeamStruct) * context->teamCount);
    if (context->teams == NULL)
    {
        return false;
    }
    for (int i = 0; i < context->teamCount; ++i)
    {
        context->teams[i].teamId = i + 1;
        strncpy(context->teams[i].name, teams[i], TEAM_NAME_LEN - 1);
        context->teams[i].name[TEAM_NAME_LEN - 1] = '\0';
        context->teams[i].totalPlayers = 0;
        context->teams[i].battingPlayerCount = 0;
        context->teams[i].averageBattingStrikeRate = 0.0f;
        context->teams[i].batsmanHead = NULL;
        context->teams[i].bowlerHead = NULL;
        context->teams[i].allrounderHead = NULL;
    }
    context->masterHead = NULL;
    context->totalPlayers = 0;
    for (int pIndex = 0; pIndex < playerCount; ++pIndex)
    {
        PlayerNode *playerNode = createPlayerNodeFromPlayer(&players[pIndex]);
        if (playerNode == NULL)
        {
            continue;
        }
        insertIntoMasterListById(&context->masterHead, playerNode);
        int teamIndex = -1;
        for (int teamSearchIndex = 0; teamSearchIndex < context->teamCount; ++teamSearchIndex)
        {
            if (strcmp(context->teams[teamSearchIndex].name, playerNode->teamName) == 0)
            {
                teamIndex = teamSearchIndex;
                break;
            }
        }
        if (teamIndex == -1)
        {
            free(playerNode);
            continue;
        }
        PlayerNode *roleNode = (PlayerNode *)malloc(sizeof(PlayerNode));
        if (roleNode == NULL)
        {
            free(playerNode);
            continue;
        }
        memcpy(roleNode, playerNode, sizeof(PlayerNode));
        roleNode->next = NULL;
        TeamStruct *teamRecord = &context->teams[teamIndex];
        if (roleNode->role == ROLE_BATSMAN)
        {
            insertIntoSortedRoleList(&teamRecord->batsmanHead, roleNode);
            teamRecord->battingPlayerCount += 1;
            teamRecord->averageBattingStrikeRate = ((teamRecord->averageBattingStrikeRate * (teamRecord->battingPlayerCount - 1)) + roleNode->strikeRate) / teamRecord->battingPlayerCount;
            teamRecord->totalPlayers += 1;
        }
        else if (roleNode->role == ROLE_BOWLER)
        {
            insertIntoSortedRoleList(&teamRecord->bowlerHead, roleNode);
            teamRecord->totalPlayers += 1;
        }
        else if (roleNode->role == ROLE_ALLROUNDER)
        {
            insertIntoSortedRoleList(&teamRecord->allrounderHead, roleNode);
            teamRecord->battingPlayerCount += 1;
            teamRecord->averageBattingStrikeRate = ((teamRecord->averageBattingStrikeRate * (teamRecord->battingPlayerCount - 1)) + roleNode->strikeRate) / teamRecord->battingPlayerCount;
            teamRecord->totalPlayers += 1;
        }
        else
        {
            free(roleNode);
        }
        context->totalPlayers += 1;
    }
    return true;
}

void freeContext(SystemContextData *context)
{
    if (context == NULL)
    {
        return;
    }
    PlayerNode *playerNode = context->masterHead;
    while (playerNode != NULL)
    {
        PlayerNode *nextNode = playerNode->next;
        free(playerNode);
        playerNode = nextNode;
    }
    context->masterHead = NULL;
    if (context->teams != NULL)
    {
        for (int i = 0; i < context->teamCount; ++i)
        {
            PlayerNode *tempPlayerNode = NULL;
            tempPlayerNode = context->teams[i].batsmanHead;
            while (tempPlayerNode != NULL)
            {
                PlayerNode *nextNode = tempPlayerNode->next;
                free(tempPlayerNode);
                tempPlayerNode = nextNode;
            }
            tempPlayerNode = context->teams[i].bowlerHead;
            while (tempPlayerNode != NULL)
            {
                PlayerNode *nextNode = tempPlayerNode->next;
                free(tempPlayerNode);
                tempPlayerNode = nextNode;
            }
            tempPlayerNode = context->teams[i].allrounderHead;
            while (tempPlayerNode != NULL)
            {
                PlayerNode *nextNode = tempPlayerNode->next;
                free(tempPlayerNode);
                tempPlayerNode = nextNode;
            }
        }
        free(context->teams);
        context->teams = NULL;
    }
    context->teamCount = 0;
    context->totalPlayers = 0;
}

void printPlayerTableHeader(void)
{
    printf("=====================================================================================\n");
    printf("%-5s %-20s %-12s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Team/Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=====================================================================================\n");
}

void displayPlayerDetailsLine(PlayerNode *playerNode, const char *teamName)
{
    if (playerNode == NULL)
    {
        return;
    }
    printf("%-5d %-20s %-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-12.2f\n",
           playerNode->id,
           playerNode->name,
           teamName ? teamName : roleToString(playerNode->role),
           playerNode->totalRuns,
           playerNode->battingAverage,
           playerNode->strikeRate,
           playerNode->wickets,
           playerNode->economyRate,
           playerNode->performanceIndex);
}

void addPlayerByID(SystemContextData *context)
{
    if (context == NULL)
    {
        return;
    }
    printf("Choose Team ID to add player:\n");
    for (int i = 0; i < context->teamCount; ++i)
    {
        printf("Id %d\t%s\n", context->teams[i].teamId, context->teams[i].name);
    }
    printf("Enter Team ID (1-%d): ", context->teamCount);
    int teamId = 0;
    getValidInteger(&teamId);
    if (teamId < 1 || teamId > context->teamCount)
    {
        printf("Invalid team id.\n");
        return;
    }
    TeamStruct *teamRecord = searchTeamByIdBinary(context, teamId);
    if (teamRecord == NULL)
    {
        printf("Team lookup failed.\n");
        return;
    }
    PlayerNode *newPlayerNode = createPlayerNodeFromInput();
    if (newPlayerNode == NULL)
    {
        return;
    }
    strncpy(newPlayerNode->teamName, teamRecord->name, TEAM_NAME_LEN - 1);
    newPlayerNode->teamName[TEAM_NAME_LEN - 1] = '\0';
    insertIntoMasterListById(&context->masterHead, newPlayerNode);
    context->totalPlayers += 1;
    PlayerNode *roleNode = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (roleNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    memcpy(roleNode, newPlayerNode, sizeof(PlayerNode));
    roleNode->next = NULL;
    if (roleNode->role == ROLE_BATSMAN)
    {
        insertIntoSortedRoleList(&teamRecord->batsmanHead, roleNode);
        teamRecord->battingPlayerCount += 1;
        teamRecord->averageBattingStrikeRate = ((teamRecord->averageBattingStrikeRate * (teamRecord->battingPlayerCount - 1)) + roleNode->strikeRate) / teamRecord->battingPlayerCount;
        teamRecord->totalPlayers += 1;
    }
    else if (roleNode->role == ROLE_BOWLER)
    {
        insertIntoSortedRoleList(&teamRecord->bowlerHead, roleNode);
        teamRecord->totalPlayers += 1;
    }
    else if (roleNode->role == ROLE_ALLROUNDER)
    {
        insertIntoSortedRoleList(&teamRecord->allrounderHead, roleNode);
        teamRecord->battingPlayerCount += 1;
        teamRecord->averageBattingStrikeRate = ((teamRecord->averageBattingStrikeRate * (teamRecord->battingPlayerCount - 1)) + roleNode->strikeRate) / teamRecord->battingPlayerCount;
        teamRecord->totalPlayers += 1;
    }
    else
    {
        free(roleNode);
    }
    printf("Player added successfully to Team %s\n", teamRecord->name);
}

void displayPlayersOfTeam(SystemContextData *context)
{
    if (context == NULL)
    {
        return;
    }
    printf("Enter Team ID: ");
    int teamId = 0;
    getValidInteger(&teamId);
    TeamStruct *teamRecord = searchTeamByIdBinary(context, teamId);
    if (teamRecord == NULL)
    {
        printf("Team not found.\n");
        return;
    }
    printf("================================================================================\n");
    printf("Players of Team %s\n", teamRecord->name);
    printPlayerTableHeader();
    PlayerNode *playerNode = NULL;
    int count = 0;
    playerNode = teamRecord->batsmanHead;
    while (playerNode != NULL)
    {
        displayPlayerDetailsLine(playerNode, roleToString(playerNode->role));
        playerNode = playerNode->next;
        count++;
    }
    playerNode = teamRecord->allrounderHead;
    while (playerNode != NULL)
    {
        displayPlayerDetailsLine(playerNode, roleToString(playerNode->role));
        playerNode = playerNode->next;
        count++;
    }
    playerNode = teamRecord->bowlerHead;
    while (playerNode != NULL)
    {
        displayPlayerDetailsLine(playerNode, roleToString(playerNode->role));
        playerNode = playerNode->next;
        count++;
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("Total Players: %d\n", teamRecord->totalPlayers);
    if (teamRecord->battingPlayerCount > 0)
    {
        printf("Average Batting Strike Rate: %.2f\n", teamRecord->averageBattingStrikeRate);
    }
    else
    {
        printf("Average Batting Strike Rate: N/A (no batsmen/all-rounders)\n");
    }
}

void displayTeamsByAvgStrikeRate(SystemContextData *context)
{
    if (context == NULL || context->teamCount <= 0)
    {
        return;
    }
    TeamStruct *tempTeamArray = (TeamStruct *)malloc(sizeof(TeamStruct) * context->teamCount);
    if (tempTeamArray == NULL)
    {
        return;
    }
    for (int i = 0; i < context->teamCount; ++i)
    {
        tempTeamArray[i] = context->teams[i];
    }
    quickSort(tempTeamArray, 0, context->teamCount - 1);
    printf("\n=====================================================================\n");
    printf("%-5s %-20s %-15s %-10s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("=====================================================================\n");
    for (int i = 0; i < context->teamCount; ++i)
    {
        printf("%-5d %-20s %-15.2f %-10d\n",
               tempTeamArray[i].teamId, tempTeamArray[i].name, tempTeamArray[i].averageBattingStrikeRate, tempTeamArray[i].totalPlayers);
    }
    free(tempTeamArray);
}

void displayTopKPlayersOfTeamByRole(SystemContextData *context)
{
    if (context == NULL)
    {
        return;
    }
    printf("Enter Team ID: ");
    int teamId = 0;
    getValidInteger(&teamId);
    TeamStruct *teamRecord = searchTeamByIdBinary(context, teamId);
    if (teamRecord == NULL)
    {
        printf("Team not found.\n");
        return;
    }
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int roleChoice = 0;
    getValidInteger(&roleChoice);
    if (roleChoice < 1 || roleChoice > 3)
    {
        printf("Invalid role selection.\n");
        return;
    }
    printf("Enter number of players: ");
    int k = 0;
    getValidInteger(&k);
    if (k <= 0)
    {
        printf("Invalid number.\n");
        return;
    }
    PlayerNode *current = NULL;
    if (roleChoice == 1)
    {
        current = teamRecord->batsmanHead;
    }
    else if (roleChoice == 2)
    {
        current = teamRecord->bowlerHead;
    }
    else
    {
        current = teamRecord->allrounderHead;
    }
    printf("Top %d %s of Team %s:\n", k, (roleChoice == 1) ? "Batsman" : (roleChoice == 2) ? "Bowler"
                                                                                           : "All-rounder",
           teamRecord->name);
    printPlayerTableHeader();
    int printed = 0;
    while (current != NULL && printed < k)
    {
        displayPlayerDetailsLine(current, teamRecord->name);
        current = current->next;
        printed++;
    }
}

void displayAllPlayersOfRole(SystemContextData *context)
{
    if (context == NULL)
    {
        return;
    }
    int roleChoice = 0;
    do
    {
        printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        getValidInteger(&roleChoice);
        if (roleChoice < 1 || roleChoice > 3)
        {
            printf("Enter a Valid Role.\n");
        }
    } while (roleChoice < 1 || roleChoice > 3);
    int teamCount = context->teamCount;
    HeapItem heap[MAX_TEAMS];
    int heapSize = 0;
    for (int i = 0; i < teamCount; ++i)
    {
        PlayerNode *head = NULL;
        if (roleChoice == 1)
        {
            head = context->teams[i].batsmanHead;
        }
        else if (roleChoice == 2)
        {
            head = context->teams[i].bowlerHead;
        }
        else
        {
            head = context->teams[i].allrounderHead;
        }
        if (head != NULL)
        {
            heap[heapSize].node = head;
            heap[heapSize].teamIndex = i;
            heapSize++;
        }
    }
    if (heapSize == 0)
    {
        printf("No players found for that role across teams.\n");
        return;
    }
    buildHeap(heap, heapSize);
    printPlayerTableHeader();
    while (heapSize > 0)
    {
        HeapItem top = extractMax(heap, &heapSize);
        PlayerNode *playerNode = top.node;
        displayPlayerDetailsLine(playerNode, context->teams[top.teamIndex].name);
        PlayerNode *nextNode = playerNode->next;
        if (nextNode != NULL)
        {
            HeapItem it;
            it.node = nextNode;
            it.teamIndex = top.teamIndex;
            pushHeap(heap, &heapSize, it);
        }
    }
}

void heapify(HeapItem heap[], int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < size && heap[left].node->performanceIndex > heap[largest].node->performanceIndex)
    {
        largest = left;
    }
    if (right < size && heap[right].node->performanceIndex > heap[largest].node->performanceIndex)
    {
        largest = right;
    }
    if (largest != i)
    {
        HeapItem tmp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = tmp;
        heapify(heap, size, largest);
    }
}

void buildHeap(HeapItem heap[], int size)
{
    for (int i = size / 2 - 1; i >= 0; --i)
    {
        heapify(heap, size, i);
    }
}

HeapItem extractMax(HeapItem heap[], int *size)
{
    HeapItem root = heap[0];
    heap[0] = heap[(*size) - 1];
    (*size)--;
    heapify(heap, *size, 0);
    return root;
}

void pushHeap(HeapItem heap[], int *size, HeapItem item)
{
    int i = (*size);
    heap[(*size)++] = item;
    while (i != 0 && heap[(i - 1) / 2].node->performanceIndex < heap[i].node->performanceIndex)
    {
        HeapItem tmp = heap[i];
        heap[i] = heap[(i - 1) / 2];
        heap[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
}

void swapTeam(TeamStruct *team1, TeamStruct *team2)
{
    TeamStruct tmp = *team1;
    *team1 = *team2;
    *team2 = tmp;
}

int partitionIndex(TeamStruct teamArray[], int low, int high)
{
    float pivot = teamArray[high].averageBattingStrikeRate;
    int i = low - 1;
    for (int j = low; j < high; ++j)
    {
        if (teamArray[j].averageBattingStrikeRate > pivot)
        {
            ++i;
            swapTeam(&teamArray[i], &teamArray[j]);
        }
    }
    swapTeam(&teamArray[i + 1], &teamArray[high]);
    return i + 1;
}

void quickSort(TeamStruct teamArray[], int low, int high)
{
    if (low < high)
    {
        int partition = partitionIndex(teamArray, low, high);
        quickSort(teamArray, low, partition - 1);
        quickSort(teamArray, partition + 1, high);
    }
}

int main()
{
    SystemContextData contextData;
    memset(&contextData, 0, sizeof(contextData));
    if (!initContext(&contextData))
    {
        printf(stderr, "Initialization failed. Exiting.\n");
        return 1;
    }
    int choice = 0;
    while (1)
    {
        printf("\n===========================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("===========================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        getValidInteger(&choice);
        switch (choice)
        {
        case 1:
        {
            addPlayerByID(&contextData);
            break;
        }
        case 2:
        {
            displayPlayersOfTeam(&contextData);
            break;
        }
        case 3:
        {
            displayTeamsByAvgStrikeRate(&contextData);
            break;
        }
        case 4:
        {
            displayTopKPlayersOfTeamByRole(&contextData);
            break;
        }
        case 5:
        {
            displayAllPlayersOfRole(&contextData);
            break;
        }
        case 6:
        {
            freeContext(&contextData);
            printf("Exiting program.\n");
            return 0;
        }
        default:
        {
            printf("Invalid choice.\n");
        }
        }
    }
    return 0;
}
