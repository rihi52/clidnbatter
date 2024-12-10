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

static void vCliDC_Combat_SetInitiative(struct part *person, int numAddition);
static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition, int numAddition);
static void vCliDC_Combat_PrintInitiativeOrder();

static void vCliDC_Combat_DealDamage(int init, int count, int amount);
static void vCliDC_Combat_CheckIntegerInputs(int *numberOf);
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
/* To add more enemies, create a new part struct and point orc to it*/

/* Orc Enemies */
part orc = {"Orc", false, 0, 0, 0, 13, 15, false, NULL};

/* Orog Enemies */
part orog = {"Orog", false, 0, 0, 0, 18, 53, false, NULL};

/* Magmin Enemies */
part magmin = {"Magmin", false, 0, 0, 0, 15, 9, false, NULL};

/* Linked list of unique part stats */
part ildmane = {"ildmane", true, 0, 0, 0, 18, 162, false, NULL};
part okssort = {"okssort", true, 0, 0, 0, 17, 162, false, NULL};

/* Linked list of player stats */
part theon = {"theon", true, 0, 0, 0,16, 55, false, NULL};
part pax = {"pax", true, 0, 0, 0, 16, 57, false, NULL};
part finn = {"finn", true, 0, 0, 0, 15, 36, false, NULL};
part ravi = {"ravi", true, 0, 0, 0, 16, 34, false, NULL};

/* Global Variables*/
//static int currentInit = 0;
static int numCombatants = 1;
static int damInit = 0;
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
        printf("Do you wish to add any new players? (y/n): ");
        fgets(prompt, sizeof(prompt), stdin);
    } while (strlen(prompt) != 1 && ('y' != prompt[0] && 'n' != prompt[0]));

    if ('n' == prompt[0])
    {
        return;
    }

    char name[50];
    int Ac, Hp;    
    
    while (1)
    {
        printf("New Player name: ");
        fgets(name, sizeof(name), stdin);
        if (name != NULL && name[0] != '\n' && name[0] != ' ')
        {
            name[strcspn(name, "\n")] = '\0';
        }
        else
        {
            printf("Error: blank input. Starting over.\n");
            continue;
        }

        name[strcspn(name, "\n")] = '\0';

        printf("New Player armor class: ");
        vCliDC_Combat_CheckIntegerInputs(&Ac);

        printf("New Player hit point maximum: ");
        vCliDC_Combat_CheckIntegerInputs(&Hp);
        break;
    }

    if (0 != giCliDC_Add_NewPlayer(name, Ac, Hp))
    {
        printf("Error accessing database.\n");
        return;
    }
    
}

static int CliDC_Combat_ChoosePlayers()
{
    while (1)
    {
        printf("Please enter desired players from db separated only by commas (eg. ravi,finn,pax): ");
        fgets(players, sizeof(players), stdin);
        if (players == NULL && players[0] == '\n' && players[0] == ' ')
        {
            printf("Error: Input blank. Try again or enter x to quit.\n");
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

    int length = strlen(players);
    for (int i = 0; i < length; i++)
    {
        if (players[i] != ' ' && players[i] != ',' && players[i] != '\n')
        {
            players[i] = tolower(players[i]);
        }
    }
    return 0;
}

static part *vCliDC_Combat_CreatePlayer(char *name)
{   // TODO: How can I move this to lookup.c??
    /* Error check */
    part *new = malloc(sizeof(part));
    if (NULL == new){
        printf("New node creation failed\n");
        return NULL;
    }

    int rc;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT name, ac, hp FROM players WHERE name = ?";

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
        fprintf(stderr, "Player with name '%s' not found in database\n", name);
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
    new->uniqueChar = true;
    new->initiative = 0;
    new->initiativeSpot = 0;
    new->isMalloc = true;
    new->turnCount = 0;
    new->next = NULL;

    sqlite3_finalize(stmt);
    return new;
}

static int CliDC_Combat_ChooseMonstsers()
{
    while (1)
    {
        printf("Please enter desired monsters from db separated only by commas (eg. orc,orog,magmin): ");
        fgets(monsters, sizeof(monsters), stdin);
        if (monsters == NULL && monsters[0] == '\n' && monsters[0] == ' ')
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

    const char *sql = "SELECT name, ac, hp FROM monsters WHERE name = ?";

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
        fprintf(stderr, "Player with name '%s' not found in database\n", name);
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

static void vCliDC_Combat_SetInitiative(struct part *person, int numAddition)
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
    vCliDC_Combat_AddToInitiativeOrder(person, numAddition);
    return;
}

static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition, int numAddition)
{
    if (NULL == pAddition)
    {
        printf("Addition to initiative order is NULL\n");
    }
    /* Used to cycle through additions */
    part *temp = NULL;

    /* if a non-unique enemy with multiple instances, make more nodes */
    // if (numAddition > 1)
    // {        
    //     //part *head = pAddition;
    //     temp = pAddition;
    //     for(int i = 0; i < numAddition - 1; i++)
    //     {
    //         newEnemy = vCliDC_Combat_CreateMonster(pAddition);
    //         if (NULL == newEnemy)
    //         {
    //             printf("newEnemy returned as NULL\n");
    //         }
    //         temp->next = newEnemy;
    //         temp = newEnemy;
    //     }
    // }

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

static void vCliDC_Combat_CheckIntegerInputs(int *numberOf)
{
    int check = 1;
    char buffer[10];
    char *endptr;
    while (check == 1)
    {
        fgets(buffer, sizeof(buffer), stdin);

        *numberOf = strtol(buffer, &endptr, 10);
        if (endptr == buffer || (*endptr != '\0' && *endptr != '\n'))
        {
            printf("Error - must be a positive integer: ");
            continue;
        }

        if (0 <= *numberOf)
        {
            check = 0;
        }
        else
        {
            printf("Error - must be a positive integer: \n");
            continue;
        }
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

    printf("\n****** Intiative Order End ******\n");
    return;
}

static void vCliDC_Combat_MainLoop()
{
    part *temp = NULL;
    int damaged = 0, damAmount = 0;
    char event[5];
    bool combat = true;

    while (combat == true)
    {
        int check = 1;
        damInit = 0;
        damaged = 0;
        damAmount = 0;
        bool confirmed = false;
        printf("Options:\n'd' for damage\n'n' for next turn\n'x' for end combat\n"); 
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
                    vCliDC_Combat_CheckIntegerInputs(&damInit);
                    if (NULL != combatants[damInit])
                    {
                        confirmed = true;
                        break;
                    }
                    printf("Error: no comabatant on given Initiaive count.\nPlease try again.\n");
                }
                confirmed = false;

                while (!confirmed)
                {
                    printf("Which creature on %d initiative: ", damInit);
                    vCliDC_Combat_CheckIntegerInputs(&damaged);
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
                vCliDC_Combat_CheckIntegerInputs(&damAmount);

                vCliDC_Combat_DealDamage(damInit, damaged, damAmount);

                vCliDC_Combat_PrintInitiativeOrder();

                break;
            case 'x':
                printf("\n********** Combat Over **********\n\n");
                combat = false;
                break;

            case 'n':
                printf("Next Turn\n");          
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

    printf("\n**** Begin acquiring player character information ****\n");

    vCliDC_Combat_PlayerSetUp();
    while (0 != CliDC_Combat_ChoosePlayers())
    {
        return;
    }

    char namePlayers[CHARACTER_BUFFER];    
    char endchar = ' ';
    int length = strlen(players), startPosition = 0;
    part *newPlayer = NULL;
    while (1)
    {
        memset(namePlayers, '\0', sizeof(namePlayers));
        int nameIndex = 0;
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
        namePlayers[nameIndex] = '\0';

        newPlayer = vCliDC_Combat_CreatePlayer(namePlayers);
        if (newPlayer == NULL)
        {
            printf("Error: newPlayer returned NULL.\n");
            return;
        }
        vCliDC_Combat_SetInitiative(newPlayer, 1);
        if (endchar == '\n')
        {
            break;
        }
    }

    while (0 != CliDC_Combat_ChooseMonstsers())
    {
        CliDC_Combat_ChooseMonstsers();
    }

    printf("\n**** End acquiring player character information ****\n");
    
    printf("\n**** Begin acquiring enemy information ****\n");

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
        vCliDC_Combat_CheckIntegerInputs(&num);
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
        
        vCliDC_Combat_SetInitiative(head, num);
        if (endchar == '\n')
        {
            break;
        }
    }
    
    printf("\n**** End acquiring enemy information ****\n\n");

    vCliDC_Combat_PrintInitiativeOrder();

    /* Main loop for combat */
    vCliDC_Combat_MainLoop();

    /* Free mallocs */
    vCliDC_Combat_FreeCombatants();    
    return;
}