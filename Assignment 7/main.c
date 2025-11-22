#include "project_header.h"

bool addPlayerByID(SystemContextData *context)
{
    if (context == NULL)
    {
        printf("Error: System context is not initialized.\n");
        return false;
    }

    printf("Choose Team ID to add player:\n");
    for (int i = 0; i < context->teamCount; ++i)
    {
        printf("Id %d\t%s\n", context->teams[i].teamId, context->teams[i].name);
    }

    int teamId = 0;
    while (1)
    {
        printf("Enter Team ID (%d-%d): ", MIN_ID, context->teamCount);
        getValidInteger(&teamId);
        if (teamId >= MIN_ID && teamId <= context->teamCount) break;
        printf("Invalid team id. Must be between %d and %d.\n", MIN_ID, context->teamCount);
    }

    TeamStruct *teamRecord = searchTeamById(context, teamId);
    if (teamRecord == NULL)
    {
        printf("Team lookup failed.\n");
        return false;
    }

    PlayerNode *newPlayerNode = createPlayerNodeFromInput(teamId, teamRecord->name, context);
    if (newPlayerNode == NULL) return false;

    if (!insertIntoMasterListById(&context->masterHead, newPlayerNode))
    {
        free(newPlayerNode);
        return false;
    }
    context->totalPlayers += 1;

    PlayerNode *roleNode = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (roleNode == NULL)
    {
        printf("Memory allocation failed for role list copy.\n");
        return false;
    }
    memcpy(roleNode, newPlayerNode, sizeof(PlayerNode));
    roleNode->next = NULL;

    if (insertPlayerToTeamLists(teamRecord, roleNode, false))
    {
        printf("Player %s added successfully to Team %s as a %s.\n",
               newPlayerNode->name, teamRecord->name, roleToString(newPlayerNode->role));
        return true;
    }
    else
    {
        printf("Error: Player role is unknown.\n");
        free(roleNode);
        return false;
    }
}

void handleMenu(SystemContextData *context)
{
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
        case 1: addPlayerByID(context); break;
        case 2: displayPlayersOfTeam(context); break;
        case 3: displayTeamsByAvgStrikeRate(context); break;
        case 4: displayTopKPlayersOfTeamByRole(context); break;
        case 5: displayAllPlayersOfRole(context); break;
        case 6:
            freeContext(context);
            printf("Exiting program.\n");
            return;
        default: printf("Invalid choice.\n");
        }
    }
}

int main()
{
    SystemContextData contextData;
    memset(&contextData, 0, sizeof(contextData));

    if (!initContext(&contextData))
    {
        printf("Initialization failed. Exiting.\n");
        return 1;
    }

    handleMenu(&contextData);

    return 0;
}