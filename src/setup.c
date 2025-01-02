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
    return;
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
