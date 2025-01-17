#include "lookup.h"

/*========================================================================*
 *  SECTION - Local definitions
 *========================================================================*
 */
// #define CHARACTER_BUFFER_BYTE   100
// #define SMALL_BUFFER_BYTE       10

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */

/*========================================================================*
 *  SECTION - Local function prototypes                                   *
 *========================================================================*
 */
void vCliDC_Lookup_MonsterName();
void vCliDC_Lookup_MonsterSize();
void vCliDC_Lookup_MonsterCr();
void vCliDC_Lookup_MonsterType();
int iCliDC_Lookup_GetInput(char *buffer);
void vCliDC_Lookup_PrintDbContents(sqlite3_stmt *stmt, char *buffer);

/*========================================================================*
 *  SECTION - Local variables                                             *
 *========================================================================*
 */

/*========================================================================*
 *  SECTION - Local function definitions                                  *
 *========================================================================*
 */
void vCliDC_Lookup_AllPlayers()
{
    char buffer[SMALL_BUFFER_BYTE];
    memset(buffer, '\0', sizeof(buffer));
    sqlite3_stmt *stmt = NULL;

    printf("\n*** All Players Lookup ***\n");

    const char *sql = "SELECT * FROM players";

    int rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return;
    }

    /* Print table header */
    printf("-------------------------------------------------\n");
    printf("| ID | Name                           | AC | HP |\n");
    printf("-------------------------------------------------\n");

    int found = 0;
    /* Print columns from database as rows in table */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = 1;
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        int ac = sqlite3_column_int(stmt, 2);
        int hp = sqlite3_column_int(stmt, 3);

        printf("|%-3d | %-30s | %-3d| %-2d |\n", id, name, ac, hp);
    }

    if (!found)
    {
        printf("No players found.\n");
    }

    /* End table */
    printf("-------------------------------------------------\n");

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error during iteration: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
    }

    sqlite3_finalize(stmt);
}

void vCliDC_Lookup_MonsterCr()
{
    char buffer[SMALL_BUFFER_BYTE];
    memset(buffer, '\0', sizeof(buffer));
    sqlite3_stmt *stmt = NULL;

    printf("\n*** CR Lookup ***");
    while (1)
    {
        printf("\nPlease enter CR of desired monster (eg: 1/2): ");
        int input = iCliDC_Lookup_GetInput(buffer);
        if (input == 1)
        {
            continue;
        }
        else if (input == 2)
        {
            return;
        }
        else
        {
            break;
        }
    }

    const char *sql = "SELECT id, name, type, size, cr, hp, ac FROM monsters WHERE cr = ?";

    stmt = CliDC_Global_PrepareAndBindText(sql, buffer);

    vCliDC_Lookup_PrintDbContents(stmt, buffer);

    sqlite3_finalize(stmt);
}

void vCliDC_Lookup_MonsterName()
{
    char monsters[CHARACTER_BUFFER_BYTE];
    memset(monsters, '\0', sizeof(monsters));
    sqlite3_stmt *stmt = NULL;

    printf("\n*** Name lookup ***");
    while (1)
    {
        printf("\nSearch for desired monster (will return all matches): ");
        int input = iCliDC_Lookup_GetInput(monsters);
        if (input == 1)
        {
            continue;
        }
        else if (input == 2)
        {
            return;
        }
        else
        {
            break;
        }
    }

    const char *sql = "SELECT id, name, type, size, cr, hp, ac FROM monsters WHERE name LIKE ? COLLATE NOCASE";

    char searchPattern[CHARACTER_BUFFER_BYTE + 10];
    snprintf(searchPattern, sizeof(searchPattern), "%%%s%%", monsters);

    stmt = CliDC_Global_PrepareAndBindText(sql, searchPattern);

    vCliDC_Lookup_PrintDbContents(stmt, searchPattern);

    sqlite3_finalize(stmt);
}

void vCliDC_Lookup_MonsterType()
{
    char types[CHARACTER_BUFFER_BYTE];
    memset(types, '\0', sizeof(types));
    sqlite3_stmt *stmt = NULL;

    printf("\n*** Type lookup ***");
    while (1)
    {
        printf("\nPlease enter Type of desired monster: ");
        int input = iCliDC_Lookup_GetInput(types);
        if (input == 1)
        {
            continue;
        }
        else if (input == 2)
        {
            return;
        }
        else
        {
            break;
        }
    }

    const char *sql = "SELECT id, name, type, size, cr, hp, ac FROM monsters WHERE type LIKE ? COLLATE NOCASE";

    /* Copy to new buffer to add wildcards */
    char searchPattern[CHARACTER_BUFFER_BYTE + 10];
    snprintf(searchPattern, sizeof(searchPattern), "%%%s%%", types);

    stmt = CliDC_Global_PrepareAndBindText(sql, searchPattern);

    vCliDC_Lookup_PrintDbContents(stmt, searchPattern);

    sqlite3_finalize(stmt);
}

void vCliDC_Lookup_MonsterSize()
{
    char Sizes[CHARACTER_BUFFER_BYTE];
    memset(Sizes, '\0', sizeof(Sizes));
    sqlite3_stmt *stmt = NULL;

    printf("\n*** Size lookup ***");
    while (1)
    {
        printf("\nPlease enter Type of desired monster: ");
        int input = iCliDC_Lookup_GetInput(Sizes);
        if (input == 1)
        {
            continue;
        }
        else if (input == 2)
        {
            return;
        }
        else
        {
            break;
        }
    }

    const char *sql = "SELECT id, name, type, size, cr, hp, ac FROM monsters WHERE size LIKE ? COLLATE NOCASE";

    /* Copy to new buffer to add wildcards */
    char searchPattern[CHARACTER_BUFFER_BYTE + 10];
    snprintf(searchPattern, sizeof(searchPattern), "%%%s%%", Sizes);

    stmt = CliDC_Global_PrepareAndBindText(sql, searchPattern);

    vCliDC_Lookup_PrintDbContents(stmt, searchPattern);

    sqlite3_finalize(stmt);
}

void vCliDC_Lookup_MonsterAc()
{
    char buffer[SMALL_BUFFER_BYTE];
    memset(buffer, '\0', sizeof(buffer));
    sqlite3_stmt *stmt = NULL;

    printf("\n*** AC Lookup ***");
    while (1)
    {
        printf("\nPlease enter AC of desired monster: ");
        int input = iCliDC_Lookup_GetInput(buffer);
        if (input == 1)
        {
            continue;
        }
        else if (input == 2)
        {
            return;
        }
        else
        {
            break;
        }
    }

    const char *sql = "SELECT id, name, type, size, cr, hp, ac FROM monsters WHERE ac = ?";

    stmt = CliDC_Global_PrepareAndBindText(sql, buffer);

    vCliDC_Lookup_PrintDbContents(stmt, buffer);

    sqlite3_finalize(stmt);
}

int iCliDC_Lookup_GetInput(char *buffer)
{
    int result = 0;
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (buffer[0] == '\n' || buffer[0] == ' ')
    {
        printf("Error: Input blank. Try again or enter x to quit.\n");
        result = 1;
    }
    else if (strcmp(buffer, "x") == 0)
    {
        printf("\n");
        result = 2;
    }
    return result;
}

void vCliDC_Lookup_PrintDbContents(sqlite3_stmt *stmt, char *buffer)
{
    int rc, found = 0;

    /* Print table header */
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| ID | Name                      |               Type               |    SIZE    |  CR | HP | AC |\n");
    printf("--------------------------------------------------------------------------------------------------\n");

    /* Print columns from database as rows in table */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = 1;
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *type = (const char *)sqlite3_column_text(stmt, 2);
        const char *size = (const char *)sqlite3_column_text(stmt, 3);
        const char *cr = (const char *)sqlite3_column_text(stmt, 4);        
        int hp = sqlite3_column_int(stmt, 5);
        int ac = sqlite3_column_int(stmt, 6);

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

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void gvCliDC_DatabaseOpen()
{

    int rc = sqlite3_open("monsters.db", &pMonsterDb);
    if (rc)
    {
        /* Exit if the database can't be opened */
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
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
    int ac;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT ac FROM players WHERE name IS ? COLLATE NOCASE";

    stmt = CliDC_Global_PrepareAndBindText(sql, Name);

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
    int hp;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT hp FROM players WHERE name IS ? COLLATE NOCASE";

    stmt = CliDC_Global_PrepareAndBindText(sql, Name);

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

void gvCliDC_Lookup_Main()
{
    int loop = 1;
    while (1 == loop)
    {
        printf("\n*** Lookup Menu ***\n");

        /* Buffer to contain menu choice, memset to '\0' to ensure no garbage values */
        char choice[SMALL_BUFFER_BYTE];
        memset(choice, '\0', sizeof(choice));
        int check = 1;

        printf("\nLookup Options:\n");
        printf( "p: Display all players\n"
                "c: Lookup monsters by CR\n"
                "n: Lookup monsters by Name\n"                
                "t: Lookup monsters by Type\n"
                "s: Lookup monsters by Size\n"
                "a: Lookup monsters by AC\n"
                "x: Return to home menu\n"
                "Please choose from the above: ");
        /* Loop to ensure only one of the provided options can be selected */
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'p' || choice[0] == 'c' || choice[0] == 'n' || choice[0] == 's' || choice[0] == 'x' || choice[0] == 't' || choice[0] == 'a'))
            {
                check = 0;
                break;
            }
            else
            {
                printf("Error: choice must be a letter from the list provided.\n");
            }
        }

        /* Menu transfer options */
        switch (choice[0])
        {
            case 'p':
                vCliDC_Lookup_AllPlayers();
                break;

            case 'c':
                vCliDC_Lookup_MonsterCr();
                break;

            case 'n':
                vCliDC_Lookup_MonsterName();
                break;
            
            case 's':
                vCliDC_Lookup_MonsterSize();
                break;

            case 't':
                vCliDC_Lookup_MonsterType();
                break;

            case 'a':
                vCliDC_Lookup_MonsterAc();
                break;

            case 'x':
                printf("\n");
                loop = 0;
                break;

            default:
                break;
        }
    }
}
