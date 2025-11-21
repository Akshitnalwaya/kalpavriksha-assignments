#include "project_header.h"

PlayerNode *createPlayerNodeFromPlayer(const Player *p)
{
    if (p == NULL) return NULL;
    
    PlayerNode *playerNode = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (playerNode == NULL)
    {
        printf("Memory allocation failed.\n");
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

bool isPlayerIdDuplicate(PlayerNode *head, int id)
{
    PlayerNode *current = head;
    while (current != NULL)
    {
        if (current->id == id) return true;
        if (current->id > id) return false;
        current = current->next;
    }
    return false;
}

bool insertIntoMasterListById(PlayerNode **headReference, PlayerNode *playerNode)
{
    if (headReference == NULL || playerNode == NULL) return false;

    if (isPlayerIdDuplicate(*headReference, playerNode->id))
    {
        printf("Error: Player ID %d already exists in the master list.\n", playerNode->id);
        return false;
    }

    PlayerNode *head = *headReference;
    if (head == NULL || playerNode->id < head->id)
    {
        playerNode->next = head;
        *headReference = playerNode;
        return true;
    }
    PlayerNode *current = head;
    while (current->next != NULL && current->next->id < playerNode->id)
    {
        current = current->next;
    }
    playerNode->next = current->next;
    current->next = playerNode;
    return true;
}

void insertIntoSortedRoleList(PlayerNode **headReference, PlayerNode *playerNode)
{
    if (headReference == NULL || playerNode == NULL) return;
    
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

TeamStruct *searchTeamById(SystemContextData *context, int teamId)
{
    if (context == NULL || context->teams == NULL) return NULL;
    if (teamId < MIN_ID || teamId > MAX_ID) return NULL;

    int low = 0;
    int high = context->teamCount - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (context->teams[mid].teamId == teamId) return &context->teams[mid];
        if (context->teams[mid].teamId < teamId) low = mid + 1;
        else high = mid - 1;
    }
    return NULL;
}

bool initializeTeams(SystemContextData *context)
{
    context->teamCount = teamCount; 
    if (context->teamCount <= 0 || context->teamCount > MAX_TEAMS)
    {
        printf("Error: Invalid team count (%d).\n", context->teamCount);
        return false;
    }
    context->teams = (TeamStruct *)malloc(sizeof(TeamStruct) * context->teamCount);
    if (context->teams == NULL)
    {
        printf("Error: Failed to allocate memory for teams.\n");
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
    return true;
}

bool insertPlayerToTeamLists(TeamStruct *teamRecord, PlayerNode *roleNode, bool isInitialLoad)
{
    if (roleNode->role == ROLE_BATSMAN)
    {
        insertIntoSortedRoleList(&teamRecord->batsmanHead, roleNode);
        teamRecord->battingPlayerCount += 1;
        if (teamRecord->battingPlayerCount > 0)
            teamRecord->averageBattingStrikeRate = ((teamRecord->averageBattingStrikeRate * (teamRecord->battingPlayerCount - 1)) + roleNode->strikeRate) / teamRecord->battingPlayerCount;
        teamRecord->totalPlayers += isInitialLoad ? 1 : 0;
    }
    else if (roleNode->role == ROLE_BOWLER)
    {
        insertIntoSortedRoleList(&teamRecord->bowlerHead, roleNode);
        teamRecord->totalPlayers += isInitialLoad ? 1 : 0;
    }
    else if (roleNode->role == ROLE_ALLROUNDER)
    {
        insertIntoSortedRoleList(&teamRecord->allrounderHead, roleNode);
        teamRecord->battingPlayerCount += 1;
        if (teamRecord->battingPlayerCount > 0)
            teamRecord->averageBattingStrikeRate = ((teamRecord->averageBattingStrikeRate * (teamRecord->battingPlayerCount - 1)) + roleNode->strikeRate) / teamRecord->battingPlayerCount;
        teamRecord->totalPlayers += isInitialLoad ? 1 : 0;
    }
    else return false;

    return true;
}

bool populateContextFromData(SystemContextData *context)
{
    for (int pIndex = 0; pIndex < playerCount; ++pIndex) 
    {
        const Player *player = &players[pIndex]; 
        if (player->id < MIN_ID || player->id > MAX_ID)
        {
            printf("Warning: Skipping player '%s' due to invalid ID %d.\n", player->name, player->id);
            continue;
        }

        PlayerNode *playerNode = createPlayerNodeFromPlayer(player);
        if (playerNode == NULL) continue;

        if (!insertIntoMasterListById(&context->masterHead, playerNode))
        {
            free(playerNode);
            continue;
        }

        TeamStruct *teamRecord = NULL;
        for (int teamSearchIndex = 0; teamSearchIndex < context->teamCount; ++teamSearchIndex)
        {
            if (strcmp(context->teams[teamSearchIndex].name, playerNode->teamName) == 0)
            {
                teamRecord = &context->teams[teamSearchIndex];
                break;
            }
        }
        if (teamRecord == NULL)
        {
            printf("Warning: Player '%s' belongs to unknown team '%s'.\n", playerNode->name, playerNode->teamName);
            continue;
        }

        PlayerNode *roleNode = (PlayerNode *)malloc(sizeof(PlayerNode));
        if (roleNode == NULL)
        {
            printf("Error: Failed to allocate memory for role node.\n");
            continue;
        }
        memcpy(roleNode, playerNode, sizeof(PlayerNode));
        roleNode->next = NULL;

        if (!insertPlayerToTeamLists(teamRecord, roleNode, true))
        {
            printf("Warning: Player '%s' has an unknown role.\n", playerNode->name);
            free(roleNode);
            continue;
        }
        context->totalPlayers += 1;
    }
    return true;
}

bool initContext(SystemContextData *context)
{
    if (context == NULL) return false;

    context->teams = NULL;
    context->teamCount = 0;
    context->masterHead = NULL;
    context->totalPlayers = 0;

    if (!initializeTeams(context))
    {
        freeContext(context);
        return false;
    }

    populateContextFromData(context);
    return true;
}

void freeContext(SystemContextData *context)
{
    if (context == NULL) return;

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
            PlayerNode *tempPlayerNode = context->teams[i].batsmanHead;
            while (tempPlayerNode != NULL) { PlayerNode *n = tempPlayerNode->next; free(tempPlayerNode); tempPlayerNode = n; }
            
            tempPlayerNode = context->teams[i].bowlerHead;
            while (tempPlayerNode != NULL) { PlayerNode *n = tempPlayerNode->next; free(tempPlayerNode); tempPlayerNode = n; }

            tempPlayerNode = context->teams[i].allrounderHead;
            while (tempPlayerNode != NULL) { PlayerNode *n = tempPlayerNode->next; free(tempPlayerNode); tempPlayerNode = n; }
        }
        free(context->teams);
        context->teams = NULL;
    }
    context->teamCount = 0;
    context->totalPlayers = 0;
}