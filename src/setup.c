#include "setup.h"

/*========================================================================*
 *  SECTION - Local definitions
 *========================================================================*
 */
#define PLAYER              0
#define MONSTER             1
#define ADD                 0
#define REMOVE              1
#define MAX_SCENARIO_IDS    50

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */

char PlayersInScenario[CHARACTER_BUFFER];
char MonstersInScenario[MONSTER_BUFFER];
int MonsterInitiativesInScenario[ALLOWED_MONSTERS];

part *ScenarioMonsterHead;
part *ScenarioMonsterTail;

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
    if (ScenarioID == -1)
    {
        return;
    }
    
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
    int DoesItExist = giCliDC_Global_DoesNameExist(ScenarioName, SCENARIOS);

    if (DoesItExist == YES)
    {
        printf("Scenario name already exists\n");
        return -1;
    }

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

    do /* Keep asking if 'a' for add or 'r' for remove isn't entered */
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
                    gvCliDC_Modify_ScenarioAddParticipant(nameParticipant, 1, 1, ScenarioID, PLAYER);
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
                    int Quantity, Initiative;
                    printf("How many %s: ", nameParticipant);                
                    gvCliDC_Global_CheckIntegerInputs(&Quantity);

                    printf("\n%s's initiative: ", nameParticipant);                
                    gvCliDC_Global_CheckIntegerInputs(&Initiative);

                    // Add chosen monster(s) to scenario in db
                    gvCliDC_Modify_ScenarioAddParticipant(nameParticipant, Quantity, Initiative, ScenarioID, MONSTER);
                    // int Initiative;
                    // printf("\n%s's initiative: ", nameParticipant);
                    // gvCliDC_Global_CheckIntegerInputs(&Initiative);
                    // gvCliDC_Modify_ScenarioAddInitiative(nameParticipant, Initiative, ScenarioID);
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

static void vCliDC_Setup_DisplayScenarios()
{   
    // TODO
    // sqlite3_stmt *stmt = NULL;

    // const char *sql = "SELECT COUNT(1) FROM scenarios";
    printf("\ndisplay\n");
    gvCliDC_Setup_CountScenarios(SCENARIO_MENU);

    return;
}

void gvCliDC_Setup_FindParticipant(int ScenarioID, int ChosenOrDisplay)
{
    sqlite3_stmt *stmt = NULL;
    
    const char *sql = "SELECT playerormonster, name, quantity, initiative FROM participants WHERE scenarioid IS ?";

    int rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    rc = sqlite3_bind_int(stmt, 1, ScenarioID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind scenarioid: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    /* Print table header */
    // printf("-----------------------------------------------------\n");
    printf("| Initiative |              Name              | Quantity |\n");
    printf("----------------------------------------------------------\n");

    int PlayerNameIndex = 0, MonsterNameIndex = 0, startPosition = 0, length = 0, monsterStartPosition = 0;
    char endchar = ' ';
    ScenarioMonsterHead = NULL;
    ScenarioMonsterTail = NULL;
    part *newMonster = NULL;
    
    /* Print columns from database as rows in table */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int PlayerOrMonster = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        int qty = sqlite3_column_int(stmt, 2);
        int initiative = sqlite3_column_int(stmt, 3);
        
        length = strlen(name);

        if(PlayerOrMonster == PLAYER) // 0 = player
        {
            // Add player name to PlayersInScenario
            
            /* Read the inputted players into PlayersInScenario[] one at a time ***/
            for (int i = startPosition; i <= length; i++)
            {
                if (name[i] != ',' && name[i] != '\n' && name[i] != '\0')
                {
                    if (PlayerNameIndex < CHARACTER_BUFFER)
                    {
                        PlayersInScenario[PlayerNameIndex] = name[i];
                        PlayerNameIndex++;                    
                    }
                }
                else
                {
                    endchar = PlayersInScenario[i];
                    PlayersInScenario[i] = ',';
                    PlayerNameIndex++;
                    break;
                }
            }
            /* Null terminate player's name */
            PlayersInScenario[PlayerNameIndex-1] = '\0';
            
        }
        else if(PlayerOrMonster == MONSTER)
        {
            for (int i = 0; i < qty; i++)
            {
                newMonster = gvCliDC_Combat_CreateMonster(name);
                newMonster->initiative = initiative;
                if (newMonster == NULL)
                {
                    printf("Error: newMonster returned NULL.\n");
                    return;
                }

                if (NULL == ScenarioMonsterHead)
                {
                    ScenarioMonsterHead = newMonster;
                }
                else
                {
                    ScenarioMonsterTail->next = newMonster;
                }

                ScenarioMonsterTail = newMonster;

                newMonster = newMonster->next;
            }
            gvCliDC_Combat_AddToInitiativeOrder(ScenarioMonsterHead);
        }
        else
        {

        }
        

        // TODO: FINISH ADDING SIZE TO QUERIES AND TABLE

        printf("|  %-8d  | %-30s |    %-5d |\n", initiative, name, qty);
    }
    PlayersInScenario[PlayerNameIndex-1] = '\n';

    /* End table */
    printf("----------------------------------------------------------\n");

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
    // int loop = 1; TODO
    // while (1 == loop)
    // {
    //     printf("\n*** Launch Scenario Menu ***\n");

    //     printf("Which scenario would you like to launch?\n");

    //     char choice[SMALL_BUFFER_BYTE];
    //     memset(choice, '\0', sizeof(choice));
    //     int check = 1;
    // }
    gvCliDC_Setup_CountScenarios(SCENARIO_MENU);
}

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void gvCliDC_Setup_CountScenarios(int ScenarioOrCombatMenu)
{
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
    char ScenarioNames[MAX_SCENARIO_IDS][MONSTER_BUFFER];

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
        strcpy(ScenarioNames[sqlite3_column_int(stmt, 0)], (const char *)sqlite3_column_text(stmt, 1));
    }

    sqlite3_finalize(stmt);
    
    /* Lookup in participants and initiatives based on scenarioid in ScenarioID array and then print
     * Is this another function or in this function??  */

    int qty, partID;
    printf("\n*---------------------------------------------------------*\n");
    printf("                     Scenario List                           \n");
    for (int i = 0; i < count; i++)
    {
        if (NULL != ScenarioNames[ScenarioIDs[i]])
        {
            printf("----------------------------------------------------------\n");
            printf("| %-54s | \n", ScenarioNames[ScenarioIDs[i]]);
            printf("----------------------------------------------------------\n");

            if (SCENARIO_MENU == ScenarioOrCombatMenu)
            {
                gvCliDC_Setup_FindParticipant(ScenarioIDs[i], DISPLAY); // Pass through ScenarioIDs[i] and print participant lists and initiatives
            }
            else
            {
                // Do nothing (TODO: error check)
            }
        }
    }

    return;
}

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
                printf("\nchoice\n");
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
