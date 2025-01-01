#include "global.h"

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

void gvCliDC_Global_CheckIntegerInputs(int *numberOf)
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
    return;
}

int giCliDC_Global_GetTextInput(char *Buffer, size_t Size)
{
    int result = 0;

    fgets(Buffer, Size, stdin);
    if (Buffer[0] != '\n' && Buffer[0] != ' ' && 2 < strlen(Buffer))
    {
        Buffer[strcspn(Buffer, "\n")] = '\0';

    }
    else if ('x' == Buffer[0] && '\n' == Buffer[1])
    {
        /* X input not allowed, always quits or returns to menu */
        printf("x entered, returning to menu\n");
        result = 2;

    }
    else
    {
        printf("Error: blank input. Starting over.\n");
        result = 1;
    }

    return result;
}

sqlite3_stmt *CliDC_Global_PrepareAndBindText(const char *sql, const char *BindValue)
{
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_close(pMonsterDb);
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, BindValue, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind AC: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return NULL;
    }

    return stmt;
}