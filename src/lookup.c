#include "lookup.h"

/*========================================================================* 
 *  SECTION - Local definitions 
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - External variables that cannot be defined in header files   * 
 *========================================================================*
 */
sqlite3 *pMonsterDb;

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

void gvCliDC_Lookup_Cr()
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

// void gvCliDC_Lookup_Hp(int hp)
// {

// }

// void gvCliDC_Lookup_Name(char *name)
// {

// }