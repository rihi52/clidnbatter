#include <math.h>
#include "combat.h"

/*========================================================================* 
 *  SECTION - Local function prototypes                                   * 
 *========================================================================* 
 */
static void vCliDC_Combat_MainLoop();
static void vCliDC_Combat_SetInitiative(struct part *person, int numAddition);
static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition, int numAddition);
static void vCliDC_Combat_PrintInitiativeOrder();
static part *vCliDC_Combat_CreateNode(struct part *enemy);
static void vCliDC_Combat_DealDamage(int init, int count, int amount);
static void vCliDC_Combat_CheckIntegerInputs(int *numberOf);

/*========================================================================* 
 *  SECTION - Local variables                                             * 
 *========================================================================* 
 */
/***** Combatants *****/
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

    printf("Player Set Up\n\n");
    printf("Do you wish to add any new players?: ");
    do 
    {
        fgets(prompt, sizeof(prompt), stdin);
    } while (!isalpha(prompt[0])); // TODO: not a great way to check input probably. I want yes or no specifically.
    
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

static void vCliDC_Combat_SetInitiative(struct part *person, int numAddition)
{
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

    if (0 != person->initiative)
    {
        numCombatants++;
    }

    /* 1 for a single unique combatant - should #define */
    vCliDC_Combat_AddToInitiativeOrder(person, numAddition);
    return;
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
            printf("Error - must be a positive integer: \n");
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

part *vCliDC_Combat_CreateNode(struct part *enemy)
{
    part *new = malloc(sizeof(part));
    if (NULL == new){
        printf("New node creation failed\n");
        return NULL;
    }

    if (enemy == &orc)
    {
        new->name = orc.name;
        new->uniqueChar = orc.uniqueChar;
        new->ac = orc.ac;
        new->hp = orc.hp;
        new->initiative = orc.initiative; 
        new->initiativeSpot = orc.initiativeSpot;
        new->turnCount = orc.turnCount;
        new->next = NULL;
    }
    else if(enemy == &orog)
    {
        new->name = orog.name;
        new->uniqueChar = orog.uniqueChar;
        new->ac = orog.ac;
        new->hp = orog.hp;
        new->initiative = orog.initiative;
        new->initiativeSpot = orog.initiativeSpot;
        new->turnCount = orog.turnCount;
        new->next = NULL;
    }
    else if(enemy == &magmin)
    {
        new->name = magmin.name;
        new->uniqueChar = magmin.uniqueChar;
        new->ac = magmin.ac;
        new->hp = magmin.hp;
        new->initiative = magmin.initiative;
        new->initiativeSpot = magmin.initiativeSpot;
        new->turnCount = magmin.turnCount;
        new->next = NULL;
    }
    else
    {

    }
    new->isMalloc = true;
    return new;
}

static void vCliDC_Combat_AddToInitiativeOrder(part *pAddition, int numAddition)
{
    if (NULL == pAddition)
    {
        printf("Addition to initiative order is NULL\n");
    }
    /* Used to cycle through additions */
    part *temp = NULL;
    part *newEnemy = NULL;

    /* if a non-unique enemy with multiple instances, make more nodes */
    if (!(pAddition->uniqueChar) && numAddition > 1)
    {        
        //part *head = pAddition;
        temp = pAddition;
        for(int i = 0; i < numAddition - 1; i++)
        {
            newEnemy = vCliDC_Combat_CreateNode(pAddition);
            if (NULL == newEnemy)
            {
                printf("newEnemy returned as NULL\n");
            }
            temp->next = newEnemy;
            temp = newEnemy;
        }
    }

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
            /*ERROR OCCURS HERE*/ if (0 < temp->hp)
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
        sqlite3_close(pMonsterDb);
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
        sqlite3_close(pMonsterDb);
    }

    sqlite3_finalize(stmt);
}

/*========================================================================* 
 *  SECTION - Global function definitions                                 * 
 *========================================================================* 
 */
void gvCliDC_Combat_Main(void)
{
    vCliDC_Combat_PlayerSetUp();
    
    part *temp = NULL;
    int numOrc = 0, numOrog = 0, numMagmin = 0;
    
    for(int i = 0; i < INITIATIVE_SPREAD; i++){
        combatants[i] = NULL;
    }

    printf("\n**** Begin acquiring player character information ****\n");

    printf("*** Intiative ***\n");
    printf("** Must be between 0 and 29, inclusive **\n\n");
    /* Assign player and unique initiative */
    vCliDC_Combat_SetInitiative(&ravi, 1);
    // vCliDC_Combat_SetInitiative(&finn, 1);
    // vCliDC_Combat_SetInitiative(&pax, 1);
    // vCliDC_Combat_SetInitiative(&theon, 1);
    // vCliDC_Combat_SetInitiative(&okssort, 1);
    // vCliDC_Combat_SetInitiative(&ildmane, 1);

    printf("\n**** End acquiring player character information ****\n");
    
    printf("\n**** Begin acquiring enemy information ****\n");
    
    /* part count - establish how many enemies of which type */
    printf("How many %s: ", orc.name);
    vCliDC_Combat_CheckIntegerInputs(&numOrc);
   
    printf("How many %s: ", orog.name);
    vCliDC_Combat_CheckIntegerInputs(&numOrog);

    printf("How many %s: ", magmin.name);
    vCliDC_Combat_CheckIntegerInputs(&numMagmin);

    /* Assign Enemy Initiative */
    if (0 < numOrc)
    {
        vCliDC_Combat_SetInitiative(&orc, numOrc);
    }

    if (0 < numOrog)
    {
        vCliDC_Combat_SetInitiative(&orog, numOrog);
    }

    if (0 < numMagmin)
    {
        vCliDC_Combat_SetInitiative(&magmin, numMagmin);
    }
    
    printf("\n**** End acquiring enemy information ****\n\n");

    vCliDC_Combat_PrintInitiativeOrder();

    /* Main loop for combat */
    vCliDC_Combat_MainLoop();

    /* Free mallocs */    
    for (int i = 0; i < INITIATIVE_SPREAD; i++)
    {
        if (NULL != combatants[i])
        {
            temp = NULL;
            part *current = combatants[i];

            while(current != NULL)
            {
                temp = current;
                current = current->next;
                if (temp->isMalloc)
                {
                    free(temp);
                }
            }
        }
    }
    return;
}