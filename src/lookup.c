#include "lookup.h"



/*========================================================================* 
 *  SECTION - Local definitions 
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - External variables that cannot be defined in header files   * 
 *========================================================================*
 */

/*========================================================================* 
 *  SECTION - Local function prototypes                                   * 
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Local variables                                             * 
 *========================================================================* 
 */

 /*=======================================================================* 
 *  SECTION - Local function definitions                                  * 
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Global function definitions                                 * 
 *========================================================================* 
 */
void gvCliDC_DatabaseOpen()
{
    
    int rc = sqlite3_open("monsters.db", &pMonsterDb);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pMonsterDb));
        return; // Exit if the database can't be opened
    }
    else
    {
        printf("Opened database successfully\n");
    }
    return;
}

void gvCliDC_DatabaseClose()
{
    sqlite3_close(pMonsterDb);
}

int giCliDC_Lookup_PlayerAc(char *Name)
{
    int rc, ac;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT ac FROM players WHERE name = ?";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return 0;
    }

    rc = sqlite3_bind_text(stmt, 1, Name, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return 0;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "Player with name '%s' not found in database\n", Name);
        sqlite3_finalize(stmt);
        return 0;
    }

    /* Assign player attributes to new part struct */
    ac = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return ac;
}

int giCliDC_Lookup_PlayerHp(char *Name)
{
    int rc, hp;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT hp FROM players WHERE name = ?";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return 0;
    }

    rc = sqlite3_bind_text(stmt, 1, Name, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return 0;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "Player with name '%s' not found in database\n", Name);
        sqlite3_finalize(stmt);
        return 0;
    }

    /* Assign player attributes to new part struct */
    hp = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return hp;
}

void gvCliDC_Lookup_MonsterCr()
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

    const char *sql = "SELECT id, name, type, cr, hp FROM monsters WHERE cr = ?";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    sqlite3_bind_int(stmt, 1, userCR);

    printf("---------------------------------------------------------------------------\n");
    printf("| ID | Name                      |             Type             | CR | HP |\n");
    printf("---------------------------------------------------------------------------\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *type = (const char *)sqlite3_column_text(stmt, 2);
        int cr = sqlite3_column_int(stmt, 3);
        int hp = sqlite3_column_int(stmt, 4);

        printf("|%-3d | %-25s | %-28s | %-2d | %-3d|\n", id, name, type, cr, hp);
    }

    printf("---------------------------------------------------------------------------\n");

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error during iteration: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
    }

    sqlite3_finalize(stmt);
}

// void gvCliDC_Lookup_Hp(int hp)
// {

// }

void gvCliDC_Lookup_MonsterName()
{
    char monsters[50];
    int rc;
    sqlite3_stmt *stmt = NULL;

    printf("Name lookup: ");
    while (1)
    {
        printf("\nPlease enter desired monsters from db separated only by commas (eg. orc,orog,magmin): ");
        fgets(monsters, sizeof(monsters), stdin);
        if (monsters[0] == '\n' && monsters[0] == ' ')
        {
            printf("Error: Input blank. Try again or enter x to quit.\n");
            continue;
        }
        else if (monsters[0] == 'x' && monsters[1] == '\n')
        {
            return;
        }
        else
        {
            break;
        }
    }

    const char *sql = "SELECT id, name, type, cr, hp FROM monsters WHERE name = ?";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, monsters, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "Player with name '%s' not found in database\n", monsters);
        sqlite3_finalize(stmt);
        return;
    }

    printf("---------------------------------------------------------------------------\n");
    printf("| ID | Name                      |             Type             | CR | HP |\n");
    printf("---------------------------------------------------------------------------\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);         // First column: ID
        const char *name = (const char *)sqlite3_column_text(stmt, 1); // Second column: Name
        const char *type = (const char *)sqlite3_column_text(stmt, 2);
        int cr = sqlite3_column_int(stmt, 3);        // Fourth column: CR
        int hp = sqlite3_column_int(stmt, 4);        // Third column: HP

        printf("|%-3d | %-25s | %-28s | %-2d | %-3d|\n", id, name, type, cr, hp);
    }

    printf("---------------------------------------------------------------------------\n");

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error during iteration: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
    }

    sqlite3_finalize(stmt);
}

void gvCliDC_Lookup_Main()
{
    char choice[5];
    int check = 1;
    printf("Lookup Options:\n");
    printf("c: Lookup a monster(s) by CR\nn: Lookup monster by name\nx: exit program\nPlease choose from the above: ");
    while (check == 1)
    {
        fgets(choice, sizeof(choice), stdin);
        if (isalpha(choice[0]) && (choice[0] == 'l' || choice[0] == 'c' || choice[0] == 'n' || choice[0] == 'x'))
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
        case 'c':
            gvCliDC_Lookup_MonsterCr();
            break;

        case 'n':
            gvCliDC_Lookup_MonsterName();
            break;

        case 'x':
            break;

        default:
            break;
    }
}