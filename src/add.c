#include "add.h"


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
int giCliDC_Add_NewPlayer(char *Name, int16_t Ac, int16_t Hp)
{
    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "INSERT INTO players (name, ac, hp) VALUES (?, ?, ?)";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return 1;
    }

    rc = sqlite3_bind_text(stmt, 1, Name, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return 2;
    }

    rc = sqlite3_bind_int(stmt, 2, Ac);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind AC: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return 3;
    }

    rc = sqlite3_bind_int(stmt, 3, Hp);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind HP: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return 4;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return 5;
    }
    else
    {
        printf("New player added successfully.\n");
    }
    
    sqlite3_finalize(stmt);
    return 0;
}