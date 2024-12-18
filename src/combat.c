#include "combat.h"

/*========================================================================* 
 *  SECTION - Local function prototypes                                   * 
 *========================================================================* 
 */
static void vCliDC_Combat_MainLoop();
static void vCliDC_Combat_PlayerSetUp();
static int CliDC_Combat_ChoosePlayers();
static part *vCliDC_Combat_CreatePlayer(char *name);
static int CliDC_Combat_ChooseMonstsers();
static part *vCliDC_Combat_CreateMonster(char *name);

static void vCliDC_Combat_SetInitiative(struct part *person);
static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition);
static void vCliDC_Combat_PrintInitiativeOrder();
static void vCliDC_Combat_PrintCurrentTurn();
static void vCliDC_Combat_IncrementTurn();

static void vCliDC_Combat_DealDamage(int init, int count, int amount);
static void vCliDC_Combat_FreeCombatants();

/*========================================================================* 
 *  SECTION - Local variables                                             * 
 *========================================================================* 
 */
#define CHARACTER_BUFFER    250
#define MONSTER_BUFFER      350

/***** Combatants *****/
char players[CHARACTER_BUFFER];
char monsters[MONSTER_BUFFER];

//static int currentInit = 0;
static int numCombatants = 1;
static int damInit = 0;
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
    char prompt[10];

    printf("*** Player Set Up ***\n\n");
    
    do 
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

static int CliDC_Combat_ChoosePlayers()
{
    while (1)
    {
        printf("\nPlease enter desired players from db separated only by commas (eg. ravi,finn,pax): ");
        fgets(players, sizeof(players), stdin);
        if (players[0] == '\n' && players[0] == ' ')
        {
            printf("Error: Input blank. Try again or enter x to return home.\n\n");
            continue;
        }
        else if (players[0] == 'x' && players[1] == '\n')
        {
            return 6;
        }
        else
        {
            break;
        }
    }
    return 0;
}

static part *vCliDC_Combat_CreatePlayer(char *name)
{
    part *new = malloc(sizeof(part));
    if (NULL == new){
        printf("New node creation failed\n");
        return NULL;
    }
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
    new->uniqueChar = true;
    new->initiative = 0;
    new->initiativeSpot = 0;
    new->isMalloc = true;
    new->turnCount = 0;
    new->next = NULL;

    return new;
}

static int CliDC_Combat_ChooseMonstsers()
{
    while (1)
    {
        printf("\nPlease enter desired monsters from db separated only by commas (eg. orc,animated armor,magmin): ");
        fgets(monsters, sizeof(monsters), stdin);
        if (monsters[0] == '\n' && monsters[0] == ' ')
        {
            printf("Error: Input blank. Try again or enter x to quit.\n");
            continue;
        }
        else if (monsters[0] == 'x' && monsters[1] == '\n')
        {
            return 6;
        }
        else
        {
            break;
        }
    }

    int length = strlen(monsters);
    for (int i = 0; i < length; i++)
    {
        if (monsters[i] != ' ' && monsters[i] != ',' && monsters[i] != '\n' && 0 != i)
        {
            monsters[i] = tolower(monsters[i]);
        }
        if (',' == monsters[i - 1] || 0 == i)
        {
            monsters[i] = toupper(monsters[i]);
        }
        else if (' ' == monsters[i - 1])
        {
            monsters[i] = toupper(monsters[i]);
        }
    }
    return 0;
}

part *vCliDC_Combat_CreateMonster(char *name)
{
    part *new = malloc(sizeof(part));
    if (NULL == new){
        printf("New node creation failed\n");
        return NULL;
    }

    int rc;
    sqlite3_stmt *stmt = NULL;

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

    /* Assign player attributes to new part struct */
    new->name = strdup((const char *)sqlite3_column_text(stmt, 0));
    if (new->name == NULL) {
        fprintf(stderr, "Memory allocation failed for name\n");
        sqlite3_finalize(stmt);
        free(new);
        return NULL;
    }
    new->ac = sqlite3_column_int(stmt, 1);
    new->hp = sqlite3_column_int(stmt, 2);
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
    printf("\n** Initiative must be between 0 and 29, inclusive **\n\n");
    int check = 1;
    char buffer[10];
    char *endptr;
    while (check == 1)
    {
        printf("%s's initiative: ", person->name);
        fgets(buffer, sizeof(buffer), stdin);

        person->initiative = strtol(buffer, &endptr, 10);
        if (endptr == buffer || (*endptr != '\0' && *endptr != '\n'))
        {
            printf("Error: initiative is out of bounds\n");
            continue;
        }

        if (INITIATIVE_SPREAD > person->initiative && 0 <= person->initiative)
        {
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
        printf("Addition to initiative order is NULL\n");
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
            if (0 < temp->hp)
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
    printf("Initiative Turn: %d\n\n", CurrentInitiative);
}

static void vCliDC_Combat_IncrementTurn()
{
    if (0 != UsedInitiative[PrintCounter])
    {
        CurrentInitiative = UsedInitiative[PrintCounter];
    }
    else
    {
        PrintCounter = 0;
        CurrentInitiative = UsedInitiative[PrintCounter];
    }
}

static void vCliDC_Combat_MainLoop()
{
    part *temp = NULL;
    int damaged = 0, damAmount = 0;
    char event[5];
    bool combat = true;

    while (combat == true)
    {
        printf("printcounter start main loop: %d\n", PrintCounter);
        
        vCliDC_Combat_PrintCurrentTurn();
        int check = 1;
        damInit = 0;
        damaged = 0;
        damAmount = 0;
        bool confirmed = false;
        printf("Combat Options:\n'd' for damage\n'n' for next turn\n'x' for end combat\n"); 
        printf("What happens: ");
        while (check == 1)
        {
            fgets(event, sizeof(event), stdin);
            if (isalpha(event[0]) && (event[0] == 'd' || event[0] == 'n' || event[0] == 'x'))
            {
                check = 0;
                break;
            }
            else
            {
                printf("Error: choice must be a letter from the list provided.\n");
            }            
        }
        switch (event[0])
        {
            case 'd':
                while (!confirmed)
                {
                    printf("Which initiative count takes damage (enter initiative): ");
                    gvCliDC_Combat_CheckIntegerInputs(&damInit);
                    if (NULL != combatants[damInit] && INITIATIVE_SPREAD > damInit && 0 < damInit)
                    {
                        confirmed = true;
                        break;
                    }
                    printf("Error: no comabatant on given Initiaive count.\nPlease try again.\n");
                }
                confirmed = false;

                while (!confirmed)
                {
                    printf("Which creature on %d initiative (enter number from Spot column): ", damInit);
                    gvCliDC_Combat_CheckIntegerInputs(&damaged);
                    temp = combatants[damInit];
                    if (NULL != temp)
                    {
                        while (temp->initiativeSpot != damaged)
                        {
                            if (NULL == temp->next && temp->initiativeSpot != damaged)
                            {
                                printf("Error: No combatant at initiaive spot %d for initiaive count %d\nPlease try again.\n", damaged, damInit);
                                break;
                            }
                            else if (temp->initiativeSpot == damaged && 0 >= temp->hp)
                            {
                                printf("Combatant already unconscious or dead.\nPlease try again.\n");
                                break;
                            }
                            else if (NULL == temp->next)
                            {
                                printf("Error: No combatant at initiaive spot %d for initiaive count %d\nPlease try again.\n", damaged, damInit);
                                break;
                            }
                            else if (damaged == temp->initiativeSpot)
                            {
                                confirmed = true;
                                break;
                            }
                            else
                            {

                            }

                            temp = temp->next;                    
                        }
                        if (temp->initiativeSpot == damaged)
                        {
                            confirmed = true;
                            break;
                        }
                    }
                    else
                    {
                        printf("Error: No combatant at initiaive spot %d for initiaive count %d\nPlease try again/\n", damaged, damInit);
                    }
                }

                printf("How much damage: ");
                gvCliDC_Combat_CheckIntegerInputs(&damAmount);

                vCliDC_Combat_DealDamage(damInit, damaged, damAmount);
                
                vCliDC_Combat_PrintInitiativeOrder();
                break;
            case 'x':
                printf("\n********** Combat Over **********\n\n");
                combat = false;
                break;

            case 'n':
                printf("Next Turn\n");
                vCliDC_Combat_PrintInitiativeOrder();
                PrintCounter++;
                vCliDC_Combat_IncrementTurn();
                break;

            default:
                printf("Invalid Choice:\n'd' for damage\n'n' for next turn\n'x' for end combat\n");
                break;
        } 
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
        temp->hp = temp->hp - amount;
    }
    return;
}

void gvCliDC_Combat_LookupMonster()
{
    char buffer[10];
    char *endptr;
    int userCR, rc;
    sqlite3_stmt *stmt = NULL;

    printf("CR Lookup: ");
    if(fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        userCR = strtol(buffer, &endptr, 10);
    }
    //scanf("%d", &userCR);

    const char *sql = "SELECT id, name, type, cr, hp FROM monsters WHERE cr = ?";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    sqlite3_bind_int(stmt, 1, userCR);

    printf("ID  | Name                      |         Type         | CR | HP\n");
    printf("---------------------------------------------------------------------\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);         // First column: ID
        const char *name = (const char *)sqlite3_column_text(stmt, 1); // Second column: Name
        const char *type = (const char *)sqlite3_column_text(stmt, 2);
        int cr = sqlite3_column_int(stmt, 3);        // Fourth column: CR
        int hp = sqlite3_column_int(stmt, 4);        // Third column: HP
        

        printf("%-3d | %-25s | %-20s | %-2d | %-3d\n", id, name, type, cr, hp);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error during iteration: %s\n", sqlite3_errmsg(pMonsterDb));
    }

    sqlite3_finalize(stmt);
}

static void vCliDC_Combat_FreeCombatants()
{
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
        if (0 == loop)
        {
            /* Add new players if desired */
            vCliDC_Combat_PlayerSetUp();
            /* Choose existing players and make sure there are no invalid characters */
            while (0 != CliDC_Combat_ChoosePlayers())
            {
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

        /* If there is no name do not attempt to create a player struct and exit loop */
        if ('\0' != namePlayers[0])
        {
            newPlayer = vCliDC_Combat_CreatePlayer(namePlayers);
        }
        else
        {
            loop = 2;
            break;
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

    while (0 != CliDC_Combat_ChooseMonstsers())
    {
        CliDC_Combat_ChooseMonstsers();
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
        gvCliDC_Combat_CheckIntegerInputs(&num);
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