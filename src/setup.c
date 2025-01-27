#include "setup.h"

/*========================================================================*
 *  SECTION - Local definitions
 *========================================================================*
 */
#define PLAYER  0
#define MONSTER 1
#define ADD     0
#define REMOVE  1

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */

/*========================================================================*
 *  SECTION - Local function prototypes                                   *
 *========================================================================*
 */
static void vCliDC_Setup_ScenarioMenu();
static int CliDC_Setup_CreateScenario();
static void vCliDC_Setup_AddRemovePlayers(int ScenarioID);
static void vCliDC_Setup_AddRemoveMonsters(int ScenarioID);
static void vCliDC_Setup_ParseParticipants(int length, int ScenarioID, int Selector, int Indicator);
static void vCliDC_Setup_DisplayScenarios();
static void vCliDC_Setup_CountScenarios();
static void vCliDC_Setup_DisplayContents();
static void vCliDC_Setup_LaunchScenario();

/*========================================================================*
 *  SECTION - Local variables                                             *
 *========================================================================*
 */
static char participants[MONSTER_BUFFER];
static char monsters[MONSTER_BUFFER];

 /*=======================================================================*
 *  SECTION - Local function definitions                                  *
 *========================================================================*
 */
static void vCliDC_Setup_ScenarioMenu()
{
    int ScenarioID;
    ScenarioID = CliDC_Setup_CreateScenario();
    int loop = 1;
    while (1 == loop)
    {
        printf("\n*** Scenario Setup Menu ***\n");

        char choice[SMALL_BUFFER_BYTE];
        memset(choice, '\0', sizeof(choice));
        int check = 1;

        printf("\nHome Options:\n");
        printf( "p: Add/Remove players\n"
                "m: Add/Remove monsters\n"
                "x: Return to setup menu\n"
                "Please choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'p' || choice[0] == 'm' || choice[0] == 'x'))
            {
                check = 0;
                break;
            }
            else
            {
                printf("Error: choice must be a letter from the list provided.\n");
            }            
        }

        switch (choice[0])
        {
            case 'p':
                vCliDC_Setup_AddRemovePlayers(ScenarioID);
                break;

            case 'm':
                vCliDC_Setup_AddRemoveMonsters(ScenarioID);
                break;
            
            case 'x':
                loop = 0;
                break;

            default:
                break;
        }
        choice[0] = '\0';
    }
}

static int CliDC_Setup_CreateScenario()
{
    char ScenarioName[INPUT_BUFFER_BYTE];
    int rc, id;
    sqlite3_stmt *stmt = NULL;

    memset(ScenarioName, '\0', sizeof(ScenarioName));

    while (1)
    {
        printf("\nEnter Scenario Name: ");
        int input = giCliDC_Global_GetTextInput(ScenarioName, INPUT_BUFFER_BYTE);
        if (input == 1)
        {
            continue;
        }
        else if (input == 2)
        {
            return -1;
        }
        else
        {
            break;
        }
    }

    /* TODO: Check scenario name doesn't already exist */
    const char *sql = "INSERT INTO scenarios (name) VALUES (?);";

    stmt = CliDC_Global_PrepareAndBindText(sql, ScenarioName);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "INSERT failed: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    const char *sql2 = "SELECT id FROM scenarios WHERE name = ?;";

    stmt = CliDC_Global_PrepareAndBindText(sql2, ScenarioName);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
    }
    else
    {
        fprintf(stderr, "Failed to retrieve scenario ID: %s\n", sqlite3_errmsg(pMonsterDb));
    }

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return id;
}

static void vCliDC_Setup_AddRemovePlayers(int ScenarioID)
{
    char prompt[SMALL_BUFFER_BYTE];   

    int length, input = 1;

    do /* Keep asking if 'y' for yes or 'n' for no isn't entered */
    {
        printf("Do you wish to add or remove players (a/r)?: ");
        fgets(prompt, sizeof(prompt), stdin);
    } while (strlen(prompt) != 1 && ('a' != prompt[0] && 'r' != prompt[0] && 'x' != prompt[0]));

    /* If statment to add or remove players - NEEDS optimized TODO */
    if ('a' == prompt[0] && '\n' == prompt[1])
    {
        /* Add players TODO: check player isn't already in scenario first */
        input = CliDC_Combat_ChoosePlayers(participants, CHARACTER_BUFFER);
        if (X_INPUT_DETECTED == input)
        {
            return;
        }
        length = strlen(participants);

        vCliDC_Setup_ParseParticipants(length, ScenarioID, PLAYER, ADD);
    }
    else if('r' == prompt[0] && '\n' == prompt[1])
    {
        /* Remove players */
        input = CliDC_Combat_ChoosePlayers(participants, CHARACTER_BUFFER);
        if (X_INPUT_DETECTED == input)
        {
            return;
        }
        length = strlen(participants);

        vCliDC_Setup_ParseParticipants(length, ScenarioID, PLAYER, REMOVE);
    }
    return;
}

static void vCliDC_Setup_AddRemoveMonsters(int ScenarioID)
{
    char prompt[SMALL_BUFFER_BYTE]; 
    int length, input = 1;

    do /* Keep asking if 'y' for yes or 'n' for no isn't entered */
    {
        printf("Do you wish to add or remove monsters (a/r)?: ");
        fgets(prompt, sizeof(prompt), stdin);
    } while (strlen(prompt) != 1 && ('a' != prompt[0] && 'r' != prompt[0] && 'x' != prompt[0]));

    if ('a' == prompt[0] && '\n' == prompt[1])
    {
        while (0 != input)
        {
            input = CliDC_Combat_ChooseMonstsers(participants, CHARACTER_BUFFER);
            if (X_INPUT_DETECTED == input)
            {
                return;
            }        
        }

        length = strlen(participants);

        vCliDC_Setup_ParseParticipants(length, ScenarioID, MONSTER, ADD);
    }
    else if('r' == prompt[0] && '\n' == prompt[1])
    {
        /* Remove monster */
        while (0 != input)
        {
            input = CliDC_Combat_ChooseMonstsers(monsters, CHARACTER_BUFFER);
            if (X_INPUT_DETECTED == input)
            {
                return;
            }        
        }
        
        length = strlen(monsters);

        vCliDC_Setup_ParseParticipants(length, ScenarioID, MONSTER, REMOVE);
    }
}

static void vCliDC_Setup_ParseParticipants(int length, int ScenarioID, int Selector, int Indicator)
{
    char nameParticipant[CHARACTER_BUFFER];
    char endchar = ' ';
    int startPosition = 0, loop = 0;

    while (0 == loop)
    {
        memset(nameParticipant, '\0', sizeof(nameParticipant));    
        int nameIndex = 0;
        /* Read the inputted players into players[] one at a time */
        for (int i = startPosition; i <= length; i++)
        {
            if (participants[i] != ',' && participants[i] != '\n')
            {
                if (nameIndex < CHARACTER_BUFFER)
                {
                    nameParticipant[nameIndex] = participants[i];
                    nameIndex++;
                }
            }
            else
            {
                endchar = participants[i];
                startPosition = i + 1;
                break;
            }
        }
        /* Null terminate player's name */
        nameParticipant[nameIndex] = '\0';

        if ('\0' != nameParticipant[0])
        {
            // Add chosen players to scenario in db, 1 for player character
            
            if (PLAYER == Selector)
            {
                if (ADD == Indicator)
                {
                    gvCliDC_Modify_ScenarioAddParticipant(nameParticipant, 1, ScenarioID);
                }
                else
                {
                    gvCliDC_Modify_ScenarioRemoveParticipant(nameParticipant, ScenarioID);
                }                
            }
            else
            {
                if (ADD == Indicator)
                {
                    int Quantity;
                    printf("How many %s: ", nameParticipant);
                
                    gvCliDC_Global_CheckIntegerInputs(&Quantity);
                    // Add chosen monster(s) to scenario in db
                    gvCliDC_Modify_ScenarioAddParticipant(nameParticipant, Quantity, ScenarioID);
                    int Initiative;
                    printf("\n%s's initiative: ", nameParticipant);
                    gvCliDC_Global_CheckIntegerInputs(&Initiative);
                    gvCliDC_Modify_ScenarioAddInitiative(nameParticipant, Initiative, ScenarioID);
                }
                else
                {
                    gvCliDC_Modify_ScenarioRemoveParticipant(nameParticipant, ScenarioID);
                }
            }          
        }
        else
        {
            loop = 0;
            continue;         
        }

        if (endchar == '\n')
        {
            loop = 2;
        }
    }
    return;
}

// TODO
// static void vCliDC_Setup_StoreInitiative(struct part *person)
// {
//     /* Initiative instructions with extra spacing for clarity */
//     printf("\n** Initiative must be between 0 and 29, inclusive **\n\n");

//     int check = 1;
//     char buffer[10];
//     char *endptr;
//     while (check == 1)
//     {
//         printf("%s's initiative: ", person->name);
//         fgets(buffer, sizeof(buffer), stdin);

//         /* Convert input to number and assign to struct */
//         person->initiative = strtol(buffer, &endptr, 10);
//         if (endptr == buffer || (*endptr != '\0' && *endptr != '\n'))
//         {
//             /* Check input is within given bounds and is a number */
//             printf("Error: initiative is out of bounds\n");
//             continue;
//         }

//         if (INITIATIVE_SPREAD > person->initiative && 0 <= person->initiative)
//         {
//             /* Break out of loop if input is in bounds */
//             check = 0;
//         }
//         else
//         {
//             printf("Error: initiative is out of bounds\n");
//         }
//     }

//     part *temp = person->next;
//     int count = 1;
//     while (NULL != temp)
//     {
//         temp->initiative = person->initiative;
//         temp = temp->next;
//         count++;
//     }

//     if (0 != person->initiative)
//     {
//         numCombatants += count;
//     }

//     /* 1 for a single unique combatant - should #define */
//     vCliDC_Combat_AddToInitiativeOrder(person);
//     return;
// }
    

static void vCliDC_Setup_DisplayScenarios()
{   
    // TODO
    // sqlite3_stmt *stmt = NULL;

    // const char *sql = "SELECT COUNT(1) FROM scenarios";
    vCliDC_Setup_CountScenarios();

    return;
}

static void vCliDC_Setup_CountScenarios()
{
    // TODO
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT COUNT(1) FROM scenarios";

    int rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        sqlite3_close(pMonsterDb);
        return;
    }

    int IDcount = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    const char *sql2 = "SELECT id FROM scenarios";
    int ScenarioIDs[IDcount];

    rc = sqlite3_prepare_v2(pMonsterDb, sql2, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        ScenarioIDs[count] = sqlite3_column_int(stmt, 0);
        count++;
    }

    sqlite3_finalize(stmt);

    const char *sql3 = "SELECT id, name FROM scenarios";

    /* Create an array of strings to hold the names of the scenarios. Use the highest scenarioid from the scenarios table to set the number of rows in the table.
     * This will allow matching the name to the scenarioid by placing the name at the index that is its scenarioid. ScenarioIDs[count] here will always be the
     * last scenarioid found, which will be the highest number in the sqlite table. This ensures you don't try and access non-existent memory */
    char ScenarioNames[ScenarioIDs[count]][MONSTER_BUFFER];

    rc = sqlite3_prepare_v2(pMonsterDb, sql3, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    /* Store names at [scenarioid] */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        strcopy(ScenarioNames[sqlite3_column_int(stmt, 0)], (const char *)sqlite3_column_text(stmt, 1));
    }

    sqlite3_finalize(stmt);
    
    /* Lookup in participants and initiatives based on scenarioid in ScenarioID array and then print
     * Is this another function or in this function??  */

    int qty, partID;
    for (int i = 0; i < count; i++)
    {
        if (NULL != ScenarioNames[ScenarioIDs[i]])
        {
            printf("\n%s | ", ScenarioNames[ScenarioIDs[i]]);

            // TODO: sql to print out each scenario's name on its own line

            for (int j = 0; j < 5; j++)
            {
                vCliDC_Lookup_FindParticipant(ScenarioIDs[i]); // Pass through ScenarioIDs[i] and print participant lists and initiatives
                // vCliDC_Lookup_FindInitiative(ScenarioIDs[i], partID); // Pass through ScenarioIDs[i] and partID
            }
        }
    }

    return;
}

static void vCliDC_Lookup_FindParticipant(int ScenarioID)
{
    sqlite3_stmt *stmt = NULL;
    const char sql = "SELECT name, quantity WHERE scenarioid IS ?";

    int rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    rc = sqlite3_bind_int(pMonsterDb, 1, ScenarioID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind scenarioid: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    int found = 0;

    /* Print table header */
    printf("-----------------------------\n");
    printf("|      Name      | Quantity |\n");
    printf("-----------------------------\n");

    /* Print columns from database as rows in table */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = 1;
        const char *name = (const char *)sqlite3_column_text(stmt, 1);      
        int qty = sqlite3_column_int(stmt, 5);

        // TODO: FINISH ADDING SIZE TO QUERIES AND TABLE

        printf("|%-3d | %-25s | %-32s | %-10s | %-3s | %-3d| %-2d |\n", id, name, type, size, cr, hp, ac);
    }

    /* End table */
    printf("---------------------------------------------------------------------------------------------------\n");

    if (0 == found)
    {
        printf("Lookup value %s not found.\n", buffer);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error during iteration: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
    }
}


static void vCliDC_Setup_DisplayContents()
{
    // TODO
    return;
}

static void vCliDC_Setup_LaunchScenario()
{
    // int loop = 1;
    // while (1 == loop)
    // {
    //     printf("\n*** Launch Scenario Menu ***\n");

    //     printf("Which scenario would you like to launch?\n");

    //     char choice[SMALL_BUFFER_BYTE];
    //     memset(choice, '\0', sizeof(choice));
    //     int check = 1;
    // }
    vCliDC_Setup_CountScenarios();
}

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void gvCliDC_Setup_Main()
{
    int loop = 1;
    while (1 == loop)
    {
        printf("\n*** Scenario Setup Menu ***\n");

        char choice[SMALL_BUFFER_BYTE];
        memset(choice, '\0', sizeof(choice));
        int check = 1;

        printf("\nHome Options:\n");
        printf( "n: New scenario\n"
                "m: Modify scenario\n"
                "l: Launch scenario\n"
                "x: Return to home menu\n"
                "Please choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'n' || choice[0] == 'm' || choice[0] == 'l' || choice[0] == 'x'))
            {
                check = 0;
                break;
            }
            else
            {
                printf("Error: choice must be a letter from the list provided.\n");
            }            
        }

        switch (choice[0])
        {
            case 'n':
                vCliDC_Setup_ScenarioMenu();
                break;

            case 'm':
                vCliDC_Setup_DisplayScenarios();
                break;

            case 'l':
                vCliDC_Setup_LaunchScenario();
                break;
            
            case 'x':
                loop = 0;
                break;

            default:
                break;
        }
        choice[0] = '\0';
    }
}
