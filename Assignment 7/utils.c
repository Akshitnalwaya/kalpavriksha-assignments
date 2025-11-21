#include "project_header.h"

void getValidInteger(int *out)
{
    while (scanf("%d", out) != 1)
    {
        printf("Invalid input. Enter integer: ");
        while (getchar() != '\n') { ; }
    }
    while (getchar() != '\n') { ; }
}

void getValidFloat(float *out)
{
    while (scanf("%f", out) != 1)
    {
        printf("Invalid input. Enter float: ");
        while (getchar() != '\n') { ; }
    }
    while (getchar() != '\n') { ; }
}

RoleType stringToRole(const char *str)
{
    char temp[50];
    int j = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != ' ' && str[i] != '-')
        {
            temp[j++] = tolower(str[i]);
        }
    }
    temp[j] = '\0';

    if (strcmp(temp, "batsman") == 0) return ROLE_BATSMAN;
    if (strcmp(temp, "bowler") == 0) return ROLE_BOWLER;
    if (strcmp(temp, "allrounder") == 0) return ROLE_ALLROUNDER;

    return ROLE_UNKNOWN;
}

const char *roleToString(RoleType role)
{
    switch (role)
    {
    case ROLE_BATSMAN: return "Batsman";
    case ROLE_BOWLER: return "Bowler";
    case ROLE_ALLROUNDER: return "All-rounder";
    default: return "Unknown";
    }
}

float calculatePerformanceIndexNode(PlayerNode *playerNode)
{
    if (playerNode == NULL) return 0.0f;
    
    if (playerNode->role == ROLE_BATSMAN)
        playerNode->performanceIndex = (playerNode->battingAverage * playerNode->strikeRate) / 100.0f;
    else if (playerNode->role == ROLE_BOWLER)
        playerNode->performanceIndex = (playerNode->wickets * 2.0f) + (100.0f - playerNode->economyRate);
    else if (playerNode->role == ROLE_ALLROUNDER)
        playerNode->performanceIndex = ((playerNode->battingAverage * playerNode->strikeRate) / 100.0f) + (playerNode->wickets * 2.0f);
    else
        playerNode->performanceIndex = 0.0f;
    
    return playerNode->performanceIndex;
}

void getInputPlayerID(int *id, SystemContextData *context)
{
    while (1)
    {
        printf("Player Id (%d-%d) : ", MIN_ID, MAX_ID);
        getValidInteger(id);
        if (*id < MIN_ID || *id > MAX_ID)
            printf("Error: Player ID must be between %d and %d.\n", MIN_ID, MAX_ID);
        else if (isPlayerIdDuplicate(context->masterHead, *id))
            printf("Error: Player ID %d already exists. Please enter a unique ID.\n", *id);
        else
            break;
    }
}

void getInputPlayerRole(RoleType *role)
{
    int roleChoice = 0;
    while (1)
    {
        printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        getValidInteger(&roleChoice);
        if (roleChoice >= 1 && roleChoice <= 3) break;
        else printf("Invalid role. Please try again.\n");
    }
    if (roleChoice == 1) *role = ROLE_BATSMAN;
    else if (roleChoice == 2) *role = ROLE_BOWLER;
    else *role = ROLE_ALLROUNDER;
}

void getInputPlayerStats(PlayerNode *playerNode)
{
    printf("Total Runs : "); getValidInteger(&playerNode->totalRuns);
    printf("Batting Average : "); getValidFloat(&playerNode->battingAverage);
    printf("Strike Rate : "); getValidFloat(&playerNode->strikeRate);
    printf("Total Wickets : "); getValidInteger(&playerNode->wickets);
    printf("Economy Rate : "); getValidFloat(&playerNode->economyRate);
}

PlayerNode *createPlayerNodeFromInput(int teamId, const char *teamName, SystemContextData *context)
{
    PlayerNode *playerNode = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (playerNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    getInputPlayerID(&playerNode->id, context);

    printf("Player Name : ");
    if (scanf(" %99[^\n]", playerNode->name) != 1) playerNode->name[0] = '\0';
    getchar();

    getInputPlayerRole(&playerNode->role);
    getInputPlayerStats(playerNode);

    strncpy(playerNode->teamName, teamName, TEAM_NAME_LEN - 1);
    playerNode->teamName[TEAM_NAME_LEN - 1] = '\0';
    playerNode->next = NULL;

    calculatePerformanceIndexNode(playerNode);
    return playerNode;
}