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
    char Name[INPUT_BUFFER_BYTE];

    while (1)
    {
        printf("\n** Delete Player **\n");
        printf("\nEnter name of Player to delete (not case-sensitive): ");
        int input = giCliDC_Global_GetTextInput(Name, INPUT_BUFFER_BYTE);
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
    printf("\n** Modify Player **\n");

    while (loop == 1)
    {
        int check = 1;
        printf("\nModify Player Options:\n");
        printf( "n: Change player name\n"
                "a: Change player AC\n"
                "h: Change player HP\n"
                "x: Return to modify database menu\n"
                "Please choose from the above: " );
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
    char Name[INPUT_BUFFER_BYTE];
    char NewName[INPUT_BUFFER_BYTE];

    while (1)
    {
        printf("Enter name of Player to change name of (not case-sensitive): ");
        int input = giCliDC_Global_GetTextInput(Name, INPUT_BUFFER_BYTE);
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
        int input = giCliDC_Global_GetTextInput(NewName, INPUT_BUFFER_BYTE);
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
    char Name[INPUT_BUFFER_BYTE];
    char NewAC[INPUT_BUFFER_BYTE];

    while (1)
    {
        printf("Enter name of Player to change AC of (not case-sensitive): ");
        int input = giCliDC_Global_GetTextInput(Name, INPUT_BUFFER_BYTE);
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
        int input = giCliDC_Global_GetTextInput(NewAC, INPUT_BUFFER_BYTE);
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
    char Name[INPUT_BUFFER_BYTE];
    char NewHP[INPUT_BUFFER_BYTE];

    while (1)
    {
        printf("Enter name of Player to change AC of (not case-sensitive): ");
        int input = giCliDC_Global_GetTextInput(Name, INPUT_BUFFER_BYTE);
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
        int input = giCliDC_Global_GetTextInput(NewHP, INPUT_BUFFER_BYTE);
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


    while (loop == 1)
    {
        printf("\n*** Modify Player Database ***\n");
        int check = 1;
        printf("\nModify Player Database Options:\n");
        printf( "a: Add a player\n"
                "r: Remove a player\n"
                "m: Modify a player\n"
                "x: Return to home menu\n"
                "Please choose from the above: " );
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
        if (name[0] != '\n' && name[0] != ' ' && 2 < strlen(name))
        {
            name[strcspn(name, "\n")] = '\0';
        }
        else if ('x' == name[0] && '\n' == name[1])
        {
            /* x not allowed as input, always quits or returns to menu */
            printf("x entered as input. Quiting add new player.\n");
            return;
        }
        else
        {
            printf("Error: blank input. Starting over.\n");
            continue;
        }

        name[strcspn(name, "\n")] = '\0';

        printf("New Player armor class: ");
        gvCliDC_Global_CheckIntegerInputs(&Ac);

        printf("New Player hit point maximum: ");
        gvCliDC_Global_CheckIntegerInputs(&Hp);
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

/* Scenario Functions */
void gvCliDC_Modify_ScenarioAddParticipant(char *Name, int Quantity, int ScenarioID)
{
    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "INSERT INTO participants (name, quantity, scenarioid) VALUES (?, ?, ?)";

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

    rc = sqlite3_bind_int(stmt, 2, Quantity);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind AC: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_bind_int(stmt, 3, ScenarioID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind HP: %s\n", sqlite3_errmsg(pMonsterDb));
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
        printf("%s added successfully.\n", Name);
    }

    sqlite3_finalize(stmt);
    return;
}

void gvCliDC_Modify_ScenarioRemoveParticipant(char *Name, int ScenarioID)
{
    sqlite3_stmt *stmt = NULL;
    int rc;

    const char *sql = "DELETE FROM participants WHERE name = ? AND scenarioid = ?;";

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

    rc = sqlite3_bind_int(stmt, 2, ScenarioID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind HP: %s\n", sqlite3_errmsg(pMonsterDb));
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
        printf("%s removed successfully.\n", Name);
    }

    sqlite3_finalize(stmt);
    return;
}

void gvCliDC_Modify_ScenarioAddInitiative(char *Name, int Initiative, int ScenarioID)
{
    int rc;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT id FROM participants WHERE name = ? AND scenarioid = ?";

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

    rc = sqlite3_bind_int(stmt, 2, ScenarioID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind HP: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    int ParticipantID = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Step 2: Insert the initiative for the participant
    const char *sql2 = "INSERT INTO initiatives (partid, initiative, scenarioid) VALUES (?, ?, ?)";

    rc = sqlite3_prepare_v2(pMonsterDb, sql2, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement (Step 2): %s\n", sqlite3_errmsg(pMonsterDb));
        return;
    }

    rc = sqlite3_bind_int(stmt, 1, ParticipantID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind participant ID (Step 2): %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_bind_int(stmt, 2, Initiative);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind Initiative (Step 2): %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_bind_int(stmt, 3, ScenarioID);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind ScenarioID (Step 2): %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to execute INSERT statement: %s\n", sqlite3_errmsg(pMonsterDb));
        sqlite3_finalize(stmt);
        return;
    }

    printf("Initiative added successfully for participant ID %d.\n", ParticipantID);
    sqlite3_finalize(stmt);
    return;
}