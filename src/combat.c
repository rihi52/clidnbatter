#include "combat.h"

/*========================================================================*
 *  SECTION - Local function prototypes                                   *
 *========================================================================*
 */
static void vCliDC_Combat_MainLoop();
static void vCliDC_Combat_PlayerSetUp();
// static int CliDC_Combat_ChoosePlayers();
static part *vCliDC_Combat_CreatePlayer(char *name);
// static int CliDC_Combat_ChooseMonstsers();
static part *vCliDC_Combat_CreateMonster(char *name);

static void vCliDC_Combat_SetInitiative(struct part *person);
static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition);
static void vCliDC_Combat_PrintInitiativeOrder();
static void vCliDC_Combat_PrintCurrentTurn();
static void vCliDC_Combat_IncrementTurn();

static int iCliDC_Combat_ChooseInitiative();
static int iCliDC_Combat_ChooseSpot();
static void vCliDC_Combat_DealDamage(int init, int count, int amount);
static void vCliDC_Combat_HealCombatant(int init, int count, int amount);
static void vCliDC_Combat_FreeCombatants();

/*========================================================================*
 *  SECTION - Local variables                                             *
 *========================================================================*
 */
// #define CHARACTER_BUFFER    250
// #define MONSTER_BUFFER      350

/***** Combatants *****/
static char players[CHARACTER_BUFFER];
static char monsters[MONSTER_BUFFER];

//static int currentInit = 0;
static int numCombatants = 1;
static int AffectedInitiative = 0;
static int AffectedSpot = 0;
static int UsedInitiative[INITIATIVE_SPREAD];
static int CurrentInitiative = 0;
static int PrintCounter = -1;
part *combatants[INITIATIVE_SPREAD];

 /*=======================================================================*
 *  SECTION - Local function definitions                                  *
 *========================================================================*
 */

static void vCliDC_Combat_PlayerSetUp()
{
    char prompt[SMALL_BUFFER_BYTE];

    printf("*** Player Set Up ***\n\n");

    do /* Keep asking if 'y' for yes or 'n' for no isn't entered */
    {
        printf("Do you wish to add any new players to the database? (y/n): ");
        fgets(prompt, sizeof(prompt), stdin);
    } while (strlen(prompt) != 1 && ('y' != prompt[0] && 'n' != prompt[0]));

    if ('n' == prompt[0])
    {
        return;
    }

    gvCliDC_Modify_EnterPlayerInformation();
}

static part *vCliDC_Combat_CreatePlayer(char *name)
{
    /* Create new player node */
    part *new = malloc(sizeof(part));
    if (NULL == new){
        printf("New node creation failed\n");
        return NULL;
    }

    /* Lookup and assign values to the player node */
    new->name = strdup(name);
    new->ac = giCliDC_Lookup_PlayerAc(name);
    if ( new->ac == 0)
    {
        free(new->name);
        return NULL;
    }
    new->hp = giCliDC_Lookup_PlayerHp(name);
    if (new->hp == 0)
    {
        free(new->name);
        return NULL;
    }
    new->MaxHp = new->hp;
    new->uniqueChar = true;
    new->initiative = 0;
    new->initiativeSpot = 0;
    new->isMalloc = true;
    new->turnCount = 0;
    new->next = NULL;

    return new;
}

int CliDC_Combat_ChooseMonstsers(char *ChosenMonsters, size_t size)
{
    while (1)
    {
        printf("\nPlease enter desired monsters from db separated only by commas (eg. orc,animated armor,magmin): ");
        fgets(ChosenMonsters, size, stdin);
        if (ChosenMonsters[0] == '\n' && ChosenMonsters[0] == ' ')
        {
            printf("Error: Input blank. Try again or enter x to quit.\n");
            continue;
        }
        else if (ChosenMonsters[0] == 'x' && ChosenMonsters[1] == '\n')
        {   /* Return to home menu if 'x' entered */
            return 6;
        }
        else
        {   /* Break out of loop if valid input */
            break;
        }
    }

    return 0;
}

part *vCliDC_Combat_CreateMonster(char *name)
{
    /* Create new node for new monster */
    part *new = malloc(sizeof(part));
    if (NULL == new){
        printf("New node creation failed\n");
        return NULL;
    }

    int rc;
    sqlite3_stmt *stmt = NULL;

    /* Lookup and assign ac and hp values from database */
    const char *sql = "SELECT name, ac, hp FROM monsters WHERE name IS ? COLLATE NOCASE";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        free(new);
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        free(new);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "Monster with name '%s' not found in database\n", name);
        sqlite3_finalize(stmt);
        free(new);
        return NULL;
    }

    /* Assign monster attributes to the newly created node */
    new->name = strdup((const char *)sqlite3_column_text(stmt, 0));
    if (new->name == NULL) {
        fprintf(stderr, "Memory allocation failed for name\n");
        sqlite3_finalize(stmt);
        free(new);
        return NULL;
    }
    new->ac = sqlite3_column_int(stmt, 1);
    new->hp = sqlite3_column_int(stmt, 2);
    new->MaxHp = new->hp;
    new->uniqueChar = false;
    new->initiative = 0;
    new->initiativeSpot = 0;
    new->isMalloc = true;
    new->turnCount = 0;
    new->next = NULL;

    sqlite3_finalize(stmt);
    return new;
}

static void vCliDC_Combat_SetInitiative(struct part *person)
{
    /* Initiative instructions with extra spacing for clarity */
    printf("\n** Initiative must be between 0 and 29, inclusive **\n\n");

    int check = 1;
    char buffer[10];
    char *endptr;
    while (check == 1)
    {
        printf("%s's initiative: ", person->name);
        fgets(buffer, sizeof(buffer), stdin);

        /* Convert input to number and assign to struct */
        person->initiative = strtol(buffer, &endptr, 10);
        if (endptr == buffer || (*endptr != '\0' && *endptr != '\n'))
        {
            /* Check input is within given bounds and is a number */
            printf("Error: initiative is out of bounds\n");
            continue;
        }

        if (INITIATIVE_SPREAD > person->initiative && 0 <= person->initiative)
        {
            /* Break out of loop if input is in bounds */
            check = 0;
        }
        else
        {
            printf("Error: initiative is out of bounds\n");
        }
    }

    part *temp = person->next;
    int count = 1;
    while (NULL != temp)
    {
        temp->initiative = person->initiative;
        temp = temp->next;
        count++;
    }

    if (0 != person->initiative)
    {
        numCombatants += count;
    }

    /* 1 for a single unique combatant - should #define */
    vCliDC_Combat_AddToInitiativeOrder(person);
    return;
}

static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition)
{
    if (NULL == pAddition)
    {
        /* Return if given node is NULL */
        printf("Addition to initiative order is NULL\n");
        return;
    }
    /* Used to cycle through additions */
    part *temp = NULL;

    /* Place combatant in the initiative order */
    if(NULL == combatants[pAddition->initiative])
    {
        combatants[pAddition->initiative] = pAddition;
    }
    else /* Start a linked list if the initiaive spot is filled */
    {
        temp = combatants[pAddition->initiative];
        while(NULL != temp->next)
        {
            temp = temp->next;
        }
        temp->next = pAddition;
    }
}

static void vCliDC_Combat_PrintInitiativeOrder()
{
    printf("\n****** Intiative Order Start ******\n\n");

    part *temp = NULL;

    printf("---------------------------------------------\n");
    printf("| Init | Spot |       Name      | AC |  HP  |\n");
    printf("---------------------------------------------\n");

    for (int i = INITIATIVE_SPREAD - 1; i > 0; i--)
    {
        int count = 1;
        temp = combatants[i];
        while(temp != NULL)
        {
            /* Cycle through initiative array and print out any nodes with hp left */
            if (0 <= temp->hp)
            {
                printf("| %4d | %4d | %-15s | %2d | %4d |\n", temp->initiative, count, temp->name, temp->ac, temp->hp);
            }
            temp->initiativeSpot = count;
            count++;
            temp = temp->next;
        }
        temp = NULL;
    }
    printf("---------------------------------------------\n");

    printf("\n****** Intiative Order End ******\n\n");

    return;
}

static void vCliDC_Combat_PrintCurrentTurn()
{
    /* Prints the current Initiative turn */
    printf("Initiative Turn: %d\n\n", CurrentInitiative);
}

static void vCliDC_Combat_IncrementTurn()
{
    /* Increments the variable used to determine which initiative turn is used */
    if (0 != UsedInitiative[PrintCounter])
    {
        CurrentInitiative = UsedInitiative[PrintCounter];
    }
    else
    {
        /* Resets to the top of initiative when 0 is reached in the UsedInitiative array, meaning the bottom of initiative has been reached */
        PrintCounter = 0;
        CurrentInitiative = UsedInitiative[PrintCounter];
    }
}

static void vCliDC_Combat_MainLoop()
{
    // part *temp = NULL;
    int /* AffectedSpot = 0,  */HitpointCount = 0;
    char event[5];
    bool combat = true;

    while (combat == true)
    {
        vCliDC_Combat_PrintCurrentTurn();
        int check = 1;
        AffectedInitiative = 0;
        AffectedSpot = 0;
        HitpointCount = 0;
        int confirmed = 0;
        printf( "Combat Options:\n"
                "d: Deal damage\n"
                "h: Heal combatant\n"
                "n: Proceed to next turn\n"
                "x: End combat\n" );
        printf("What happens: ");

        /* Loop to ensure only one of the provided options can be selected */
        while (check == 1)
        {
            fgets(event, sizeof(event), stdin);
            if (isalpha(event[0]) && (event[0] == 'd' || event[0] == 'h' || event[0] == 'n' || event[0] == 'x'))
            {
                check = 0;
                break;
            }
            else
            {
                printf("Error: choice must be a letter from the list provided.\n");
            }
        }

        /* Combat options */
        switch (event[0])
        {
            case 'd': /* Deal damage */
                while (0 == confirmed)
                {
                    confirmed = iCliDC_Combat_ChooseInitiative();
                }
                confirmed = 0;

                while (0 == confirmed)
                {
                    confirmed = iCliDC_Combat_ChooseSpot();
                }

                printf("How much damage: ");
                gvCliDC_Global_CheckIntegerInputs(&HitpointCount);

                vCliDC_Combat_DealDamage(AffectedInitiative, AffectedSpot, HitpointCount);

                vCliDC_Combat_PrintInitiativeOrder();
                break;
            case 'h':
                while (0 == confirmed)
                {
                    confirmed = iCliDC_Combat_ChooseInitiative();
                }

                confirmed = 0;

                while (0 == confirmed)
                {
                    confirmed = iCliDC_Combat_ChooseSpot();
                }

                printf("How much healing: ");
                gvCliDC_Global_CheckIntegerInputs(&HitpointCount);                

                vCliDC_Combat_HealCombatant(AffectedInitiative, AffectedSpot, HitpointCount);
                vCliDC_Combat_PrintInitiativeOrder();
                break;

            case 'x': /* End combat */
                printf("\n********** Combat Over **********\n\n");
                combat = false;
                break;

            case 'n': /* Next Turn */
                printf("\nNext Turn\n");
                vCliDC_Combat_PrintInitiativeOrder();
                PrintCounter++;
                vCliDC_Combat_IncrementTurn();
                break;

            default:
                printf( "Invalid Choice:\n"
                        "d: Deal damage\n"
                        "n: Proceed to next turn\n"
                        "x: End combat\n" );
                break;
        }
    }
}

static int iCliDC_Combat_ChooseInitiative()
{
    /* Receive input for which battle participant is to take damage */
    printf("Which initiative count is affected (enter initiative): ");
    gvCliDC_Global_CheckIntegerInputs(&AffectedInitiative);
    if (NULL != combatants[AffectedInitiative] && INITIATIVE_SPREAD > AffectedInitiative && 0 < AffectedInitiative)
    {
        /* Ensures input is within bounds */
        return 1;
    }
    printf("Error: no comabatant on given Initiaive count.\nPlease try again.\n");
    return 0;
}

/* TODO: Working on this one */
static int iCliDC_Combat_ChooseSpot()
{
    part *temp = NULL;

    /* Recieves input on which battle participant on the before given initiative count is to receive damage in case there are multiple */
    printf("Which creature on %d initiative (enter number from Spot column): ", AffectedInitiative);
    gvCliDC_Global_CheckIntegerInputs(&AffectedSpot);
    temp = combatants[AffectedInitiative];
    /* NULL check */
    if (NULL != temp)
    {
        /* Loop through to get to the correct initiative and initiative spot */
        while (temp->initiativeSpot != AffectedSpot)
        {
            if (NULL == temp->next && temp->initiativeSpot != AffectedSpot)
            {
                printf("Error: No combatant at initiaive spot %d for initiaive count %d\nPlease try again.\n", AffectedSpot, AffectedInitiative);
                break;
            }
            else if (NULL == temp->next)
            {
                printf("Error: No combatant at initiaive spot %d for initiaive count %d\nPlease try again.\n", AffectedSpot, AffectedInitiative);
                break;
            }
            else if (AffectedSpot == temp->initiativeSpot)
            {
                /* Found the correct spot and it is valid */
                return 1;
            }
            else
            {

            }

            temp = temp->next;
        }
        if (temp->initiativeSpot == AffectedSpot)
        {
            return 1;
        }
        return 0;
    }
    else
    {
        printf("Error: No combatant at initiaive spot %d for initiaive count %d\nPlease try again/\n", AffectedSpot, AffectedInitiative);
        return 0;
    }
}

static void vCliDC_Combat_DealDamage(int init, int count, int amount)
{
    /* set initiative count */
    part *temp = combatants[init];
    if (NULL != temp)
    {
        /* loop through to find the correct creature on the initiative count */
        while (temp->initiativeSpot != count && temp != NULL)
        {
            temp = temp->next;
        }
        /* remove hp */
        if (0 < temp->hp && 0 < (temp->hp - amount))
        {
            temp->hp = temp->hp - amount;
        }
        else if(0 < temp->hp &&  0 >= (temp->hp - amount))
        {
            temp->hp = 0;
        }
    }
    return;
}

static void vCliDC_Combat_HealCombatant(int init, int count, int amount)
{
    /* set initiative count */
    part *temp = combatants[init];
    if (NULL != temp)
    {
        /* loop through to find the correct creature on the initiative count */
        while (temp->initiativeSpot != count && temp != NULL)
        {
            temp = temp->next;
        }
        /* Add hp */
        if (temp->MaxHp > temp->hp && temp->MaxHp > (temp->hp + amount))
        {
            temp->hp = temp->hp + amount;
        }
        else if(temp->MaxHp > temp->hp &&  temp->MaxHp >= (temp->hp + amount))
        {
            temp->hp = temp->MaxHp;
        }
    }
    return;
}

static void vCliDC_Combat_FreeCombatants()
{
    /* Free all malloc'd memory */
    for (int i = 0; i < INITIATIVE_SPREAD; i++)
    {
        if (NULL != combatants[i])
        {
            part *current = combatants[i];
            part *temp;

            while(current != NULL)
            {
                temp = current;
                current = current->next;
                free(temp->name);
                free(temp);
            }
        }
    }
}

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void gvCliDC_Combat_Main(void)
{
    for(int i = 0; i < INITIATIVE_SPREAD; i++){
        combatants[i] = NULL;
    }

    printf("\n**** Begin acquiring player character information ****\n\n");

    char namePlayers[CHARACTER_BUFFER];
    char endchar = ' ';
    int length, startPosition = 0, loop = 0;
    part *newPlayer = NULL;
    /* Loop to acquire player information
     * Two loop statuses so the user input functions can be returned to if needed */
    while (0 == loop || 1 == loop)
    {
        //startPosition = 0;
        if (0 == loop)
        {
            startPosition = 0;
            /* Add new players if desired */
            vCliDC_Combat_PlayerSetUp();
            /* Choose existing players and make sure there are no invalid characters 
             * Don't use Global_GetInput to allow spaces in player names */
            while (0 != CliDC_Combat_ChoosePlayers(players, CHARACTER_BUFFER))
            {
                /* Return to home menu if 'x' entered */
                return;
            }
            length = strlen(players);
            loop = 1;
        }

        memset(namePlayers, '\0', sizeof(namePlayers));
        int nameIndex = 0;
        /* Read the inputted players into players[] one at a time */
        for (int i = startPosition; i <= length; i++)
        {
            if (players[i] != ',' && players[i] != '\n')
            {
                if (nameIndex < CHARACTER_BUFFER)
                {
                    namePlayers[nameIndex] = players[i];
                    nameIndex++;                    
                }
            }
            else
            {
                endchar = players[i];
                startPosition = i + 1;
                break;
            }
        }
        /* Null terminate player's name */
        namePlayers[nameIndex] = '\0';

        /* If there is no name do not attempt to create a player struct and restart loop */
        if ('\0' != namePlayers[0])
        {
            newPlayer = vCliDC_Combat_CreatePlayer(namePlayers);
        }
        else
        {
            loop = 0;
            continue;
        }

        if (newPlayer == NULL)
        {
            printf("Please re-enter players' names or enter 'x' to return to home\n\n");
            loop = 0;
            continue;
        }

        vCliDC_Combat_SetInitiative(newPlayer);

        if (endchar == '\n')
        {
            loop = 2;
            break;
        }
    }

    printf("\n**** End acquiring player character information ****\n");
    printf("\n**** Begin acquiring enemy information ****\n");

    int input = 1;
    while (0 != input)
    {
        input = CliDC_Combat_ChooseMonstsers(monsters, CHARACTER_BUFFER);
        if (X_INPUT_DETECTED == input)
        {
            return;
        }        
    }

    char nameMonsters[MONSTER_BUFFER];
    endchar = ' ';
    length = strlen(monsters);
    startPosition = 0;
    part *newMonster = NULL;
    while (1)
    {
        memset(nameMonsters, '\0', sizeof(nameMonsters));
        int nameIndex = 0;
        for (int i = startPosition; i <= length; i++)
        {
            if (monsters[i] != ',' && monsters[i] != '\n')
            {
                if (nameIndex < CHARACTER_BUFFER)
                {
                    nameMonsters[nameIndex] = monsters[i];
                    nameIndex++;
                }
            }
            else
            {
                endchar = monsters[i];
                startPosition = i + 1;
                break;
            }
        }
        nameMonsters[nameIndex] = '\0';
        printf("How many %s: ", nameMonsters);
        int num;
        gvCliDC_Global_CheckIntegerInputs(&num);
        part *head = NULL;
        part *tail = NULL;
        for (int i = 0; i < num; i++)
        {
            newMonster = vCliDC_Combat_CreateMonster(nameMonsters);
            if (newMonster == NULL)
            {
                printf("Error: newMonster returned NULL.\n");
                return;
            }

            if (NULL == head)
            {
                head = newMonster;
            }
            else
            {
                tail->next = newMonster;
            }

            tail = newMonster;

            newMonster = newMonster->next;
        }

        vCliDC_Combat_SetInitiative(head);
        if (endchar == '\n')
        {
            break;
        }
    }

    printf("\n**** End acquiring enemy information ****\n\n");

    vCliDC_Combat_PrintInitiativeOrder();

    /* Fill array */
    for (int i = INITIATIVE_SPREAD - 1; i > 0; i--)
    {
        UsedInitiative[i] = 0;
    }

    /* Find highest initiative */
    int count = 0;
    for (int i = INITIATIVE_SPREAD - 1; i > 0; i--)
    {
        if (NULL != combatants[i])
        {
            UsedInitiative[count] = combatants[i]->initiative;
            count++;
        }
    }

    /* Initialize turn order */
    vCliDC_Combat_IncrementTurn();

    /* Main loop for combat */
    vCliDC_Combat_MainLoop();

    /* Free mallocs */
    vCliDC_Combat_FreeCombatants();
    PrintCounter = -1;
    return;
}

int CliDC_Combat_ChoosePlayers(char *ChosenPlayers, size_t size)
{
    int result = 0;
    while (1)
    {
        memset(ChosenPlayers, '\0', size);
        printf("\nPlease enter desired players from db separated only by commas (eg. ravi,finn,pax): ");
        fgets(ChosenPlayers, size, stdin);
        if (ChosenPlayers[0] == '\n' && ChosenPlayers[0] == ' ')
        {
            printf("Error: Input blank. Try again or enter x to return home.\n\n");
            continue;
        }
        else if (ChosenPlayers[0] == 'x' && ChosenPlayers[1] == '\n')
        {   /* Return to home menu if 'x' entered */
            result = 6;
            break;
        }
        else
        {   /* Break out of loop if valid input */
            result = 0;
            break;
        }
    }
    return result;
}
