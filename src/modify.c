#include "modify.h"


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
static void vCliDC_Modify_DeletePlayer();
static void vCliDC_Modify_ModifyChoices();
static void vCliDC_Modify_ChangePlayerName();
static void vCliDC_Modify_ChangePlayerAC();
static void vCliDC_Modify_ChangePlayerHP();

/*========================================================================* 
 *  SECTION - Local variables                                             * 
 *========================================================================* 
 */

 /*=======================================================================* 
 *  SECTION - Local function definitions                                  * 
 *========================================================================* 
 */
static void vCliDC_Modify_DeletePlayer()
{
    char Name[50];

    while (1)
    {
        printf("Enter name of Player to delete (not case-sensitive): ");
        int input = giCliDC_Global_GetInput(Name);
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

    char prompt[5];
    printf("This will delete %s from the database permanently.\nAre you sure you want to proceed (y/n): ", Name);

    do 
    {
        fgets(prompt, sizeof(prompt), stdin);
    } while (strlen(prompt) != 1 && ('y' != prompt[0] && 'n' != prompt[0]));

    if ('n' == prompt[0])
    {
        return;
    }

    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "DELETE FROM players WHERE name IS ? COLLATE NOCASE";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, Name, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }
    else
    {
        printf("Player %s deleted successfully.\n", Name);
    }
    
    sqlite3_finalize(stmt);
}

static void vCliDC_Modify_ModifyChoices()
{
    char choice[5];
    int loop = 1;
    printf("\n** Modify Player **\n\n");

    while (loop == 1)
    {
        int check = 1;
        printf("Modify Player Options:\n");
        printf("n: change player name\na: change player AC\nh: change player HP\nx: return home\nPlease choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'n' || choice[0] == 'a' || choice[0] == 'h' || choice[0] == 'x'))
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
                vCliDC_Modify_ChangePlayerName();
                break;

            case 'a':
                vCliDC_Modify_ChangePlayerAC();
                break;

            case 'h':
                vCliDC_Modify_ChangePlayerHP();
                
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

static void vCliDC_Modify_ChangePlayerName()
{
    char Name[50];
    char NewName[50];

    while (1)
    {
        printf("Enter name of Player to change name of (not case-sensitive): ");
        int input = giCliDC_Global_GetInput(Name);
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

    while (1)
    {
        printf("Enter new name: ");
        int input = giCliDC_Global_GetInput(NewName);
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

    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "UPDATE players SET name = ? WHERE name IS ? COLLATE NOCASE";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, NewName, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_bind_text(stmt, 2, Name, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }
    else
    {
        printf("Player %s name changed to %s successfully.\n\n", Name, NewName);
    }
    
    sqlite3_finalize(stmt);

}

static void vCliDC_Modify_ChangePlayerAC()
{
    char Name[50];
    char NewAC[50];

    while (1)
    {
        printf("Enter name of Player to change AC of (not case-sensitive): ");
        int input = giCliDC_Global_GetInput(Name);
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

    while (1)
    {
        printf("Enter new AC: ");
        int input = giCliDC_Global_GetInput(NewAC);
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

    int rc;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE players SET ac = ? WHERE name IS ? COLLATE NOCASE";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, NewAC, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind ac: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_bind_text(stmt, 2, Name, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }
    else
    {
        printf("Player %s AC changed to %s successfully.\n\n", Name, NewAC);
    }
    
    sqlite3_finalize(stmt);
    return;
}

static void vCliDC_Modify_ChangePlayerHP()
{
    char Name[50];
    char NewHP[50];

    while (1)
    {
        printf("Enter name of Player to change AC of (not case-sensitive): ");
        int input = giCliDC_Global_GetInput(Name);
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

    while (1)
    {
        printf("Enter new HP: ");
        int input = giCliDC_Global_GetInput(NewHP);
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

    int rc;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE players SET hp = ? WHERE name IS ? COLLATE NOCASE";

    rc = sqlite3_prepare_v2(pMonsterDb, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, NewHP, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind ac: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_bind_text(stmt, 2, Name, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }
    else
    {
        printf("Player %s HP changed to %s successfully.\n\n", Name, NewHP);
    }
    
    sqlite3_finalize(stmt);
    return;
}

/*========================================================================* 
 *  SECTION - Global function definitions                                 * 
 *========================================================================* 
 */
void gvCliDC_Modify_MainLoop()
{
    char choice[5];
    int loop = 1;
    printf("\n*** Modify Player Database ***\n\n");

    while (loop == 1)
    {
        int check = 1;
        printf("Modify Player Database Options:\n");
        printf("a: add a player\nr: remove a player\nm: modify a player\nx: return home\nPlease choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'a' || choice[0] == 'r' || choice[0] == 'm' || choice[0] == 'x'))
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
            case 'a':
                gvCliDC_Modify_EnterPlayerInformation();
                break;

            case 'r':
                vCliDC_Modify_DeletePlayer();
                break;

            case 'm':
                vCliDC_Modify_ModifyChoices();
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

void gvCliDC_Modify_EnterPlayerInformation()
{
    char name[50];
    int Ac, Hp; 

    while (1)
    {
        printf("New Player name (50 character maximum): ");
        fgets(name, sizeof(name), stdin);
        if (name[0] != '\n' && name[0] != ' ')
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
        gvCliDC_Combat_CheckIntegerInputs(&Ac);

        printf("New Player hit point maximum: ");
        gvCliDC_Combat_CheckIntegerInputs(&Hp);
        break;
    }

    if (0 != giCliDC_Modify_NewPlayer(name, Ac, Hp))
    {
        printf("Error accessing database.\n");
        return;
    }
}

int giCliDC_Modify_NewPlayer(char *Name, int16_t Ac, int16_t Hp)
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