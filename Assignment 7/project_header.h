#ifndef PROJECT_HEADER_H
#define PROJECT_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "Players_data.h"

#define NAME_LEN 100
#define TEAM_NAME_LEN 50
#define ROLE_LEN 20
#define MAX_TEAMS 20
#define MIN_ID 1
#define MAX_ID 10000

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

void getValidInteger(int *out);
void getValidFloat(float *out);
RoleType stringToRole(const char *str);
const char *roleToString(RoleType role);
float calculatePerformanceIndexNode(PlayerNode *playerNode);
void getInputPlayerID(int *id, SystemContextData *context);
void getInputPlayerRole(RoleType *role);
void getInputPlayerStats(PlayerNode *playerNode);
PlayerNode *createPlayerNodeFromInput(int teamId, const char *teamName, SystemContextData *context);
bool initContext(SystemContextData *context);
void freeContext(SystemContextData *context);
bool isPlayerIdDuplicate(PlayerNode *head, int id);
PlayerNode *createPlayerNodeFromPlayer(const Player *p);
bool insertIntoMasterListById(PlayerNode **headReference, PlayerNode *playerNode);
void insertIntoSortedRoleList(PlayerNode **headReference, PlayerNode *playerNode);
TeamStruct *searchTeamById(SystemContextData *context, int teamId);
bool initializeTeams(SystemContextData *context);
bool populateContextFromData(SystemContextData *context);
bool insertPlayerToTeamLists(TeamStruct *teamRecord, PlayerNode *roleNode, bool isInitialLoad);
bool addPlayerByID(SystemContextData *context);
void displayPlayersOfTeam(SystemContextData *context);
void displayTeamsByAvgStrikeRate(SystemContextData *context);
void displayTopKPlayersOfTeamByRole(SystemContextData *context);
void displayAllPlayersOfRole(SystemContextData *context);
void printPlayerTableHeader(void);
void displayPlayerDetailsLine(PlayerNode *playerNode, const char *teamNameOrRole);
void heapify(HeapItem heap[], int size, int i);
void buildHeap(HeapItem heap[], int size);
HeapItem extractMax(HeapItem heap[], int *size);
void pushHeap(HeapItem heap[], int *size, HeapItem item);
void swapTeam(TeamStruct *a, TeamStruct *b);
int partitionIndex(TeamStruct teamArray[], int low, int high);
void quickSort(TeamStruct teamArray[], int low, int high);
void handleMenu(SystemContextData *context);

#endif