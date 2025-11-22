#include "project_header.h"

void printPlayerTableHeader(void)
{
    printf("=====================================================================================\n");
    printf("%-5s %-20s %-12s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Team/Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=====================================================================================\n");
}

void displayPlayerDetailsLine(PlayerNode *playerNode, const char *teamNameOrRole)
{
    if (playerNode == NULL) return;
    printf("%-5d %-20s %-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-12.2f\n",
           playerNode->id, playerNode->name, teamNameOrRole, playerNode->totalRuns,
           playerNode->battingAverage, playerNode->strikeRate, playerNode->wickets,
           playerNode->economyRate, playerNode->performanceIndex);
}

void displayPlayersOfTeam(SystemContextData *context)
{
    if (context == NULL) return;
    printf("Enter Team ID: ");
    int teamId = 0;
    getValidInteger(&teamId);
    TeamStruct *teamRecord = searchTeamById(context, teamId);
    if (teamRecord == NULL)
    {
        printf("Team not found or invalid ID.\n");
        return;
    }
    printf("\n================================================================================\n");
    printf("Players of Team %s\n", teamRecord->name);
    printPlayerTableHeader();
    
    PlayerNode *playerNode = teamRecord->batsmanHead;
    while (playerNode != NULL) { displayPlayerDetailsLine(playerNode, roleToString(playerNode->role)); playerNode = playerNode->next; }
    
    playerNode = teamRecord->allrounderHead;
    while (playerNode != NULL) { displayPlayerDetailsLine(playerNode, roleToString(playerNode->role)); playerNode = playerNode->next; }
    
    playerNode = teamRecord->bowlerHead;
    while (playerNode != NULL) { displayPlayerDetailsLine(playerNode, roleToString(playerNode->role)); playerNode = playerNode->next; }

    printf("--------------------------------------------------------------------------------\n");
    printf("Total Players in Team %s: %d\n", teamRecord->name, teamRecord->totalPlayers);
    if (teamRecord->battingPlayerCount > 0) printf("Average Batting Strike Rate: %.2f\n", teamRecord->averageBattingStrikeRate);
    else printf("Average Batting Strike Rate: N/A\n");
    printf("================================================================================\n");
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

void displayTeamsByAvgStrikeRate(SystemContextData *context)
{
    if (context == NULL || context->teamCount <= 0)
    {
        printf("No teams to display.\n");
        return;
    }
    TeamStruct *tempTeamArray = (TeamStruct *)malloc(sizeof(TeamStruct) * context->teamCount);
    if (tempTeamArray == NULL)
    {
        printf("Memory allocation failed for sorting.\n");
        return;
    }
    for (int i = 0; i < context->teamCount; ++i) tempTeamArray[i] = context->teams[i];

    quickSort(tempTeamArray, 0, context->teamCount - 1);

    printf("\n=====================================================================\n");
    printf("Teams Sorted by Average Batting Strike Rate (Descending)\n");
    printf("=====================================================================\n");
    printf("%-5s %-20s %-15s %-10s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("=====================================================================\n");

    for (int i = 0; i < context->teamCount; ++i)
    {
        printf("%-5d %-20s %-15.2f %-10d\n",
               tempTeamArray[i].teamId, tempTeamArray[i].name, tempTeamArray[i].averageBattingStrikeRate, tempTeamArray[i].totalPlayers);
    }
    printf("=====================================================================\n");
    free(tempTeamArray);
}

void displayTopKPlayersOfTeamByRole(SystemContextData *context)
{
    if (context == NULL) return;
    printf("Enter Team ID: ");
    int teamId = 0;
    getValidInteger(&teamId);
    TeamStruct *teamRecord = searchTeamById(context, teamId);
    if (teamRecord == NULL)
    {
        printf("Team not found or invalid ID.\n");
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
    printf("Enter number of players (K): ");
    int k = 0;
    getValidInteger(&k);
    if (k <= 0)
    {
        printf("Invalid number of players.\n");
        return;
    }

    PlayerNode *current = NULL;
    const char *roleName = NULL;
    if (roleChoice == 1) { current = teamRecord->batsmanHead; roleName = "Batsmen"; }
    else if (roleChoice == 2) { current = teamRecord->bowlerHead; roleName = "Bowlers"; }
    else { current = teamRecord->allrounderHead; roleName = "All-rounders"; }

    printf("\n================================================================================\n");
    printf("Top %d %s of Team %s (Sorted by Performance Index):\n", k, roleName, teamRecord->name);
    printPlayerTableHeader();
    int printed = 0;
    while (current != NULL && printed < k)
    {
        displayPlayerDetailsLine(current, roleToString(current->role));
        current = current->next;
        printed++;
    }
    if (printed == 0) printf("No %s found in this team.\n", roleName);
    printf("================================================================================\n");
}

void heapify(HeapItem heap[], int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].node->performanceIndex > heap[largest].node->performanceIndex)
        largest = left;
    if (right < size && heap[right].node->performanceIndex > heap[largest].node->performanceIndex)
        largest = right;
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
    for (int i = size / 2 - 1; i >= 0; --i) heapify(heap, size, i);
}

HeapItem extractMax(HeapItem heap[], int *size)
{
    if (*size <= 0)
    {
        HeapItem empty = {NULL, -1};
        return empty;
    }
    HeapItem root = heap[0];
    heap[0] = heap[(*size) - 1];
    (*size)--;
    heapify(heap, *size, 0);
    return root;
}

void pushHeap(HeapItem heap[], int *size, HeapItem item)
{
    if (*size >= MAX_TEAMS) return;
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

void displayAllPlayersOfRole(SystemContextData *context)
{
    if (context == NULL) return;
    int roleChoice = 0;
    do
    {
        printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        getValidInteger(&roleChoice);
        if (roleChoice < 1 || roleChoice > 3) printf("Enter a Valid Role.\n");
    } while (roleChoice < 1 || roleChoice > 3);

    const char *roleName = roleToString((RoleType)roleChoice);
    int teamCount = context->teamCount;
    HeapItem heap[MAX_TEAMS];
    int heapSize = 0;

    for (int i = 0; i < teamCount; ++i)
    {
        PlayerNode *head = NULL;
        if (roleChoice == 1) head = context->teams[i].batsmanHead;
        else if (roleChoice == 2) head = context->teams[i].bowlerHead;
        else head = context->teams[i].allrounderHead;
        
        if (head != NULL)
        {
            if (heapSize < MAX_TEAMS)
            {
                heap[heapSize].node = head;
                heap[heapSize].teamIndex = i;
                heapSize++;
            }
        }
    }

    if (heapSize == 0)
    {
        printf("No %s found across all teams.\n", roleName);
        return;
    }

    buildHeap(heap, heapSize);

    printf("\n================================================================================\n");
    printf("All %s Players Across All Teams (Sorted by Performance Index):\n", roleName);
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
    printf("================================================================================\n");
}