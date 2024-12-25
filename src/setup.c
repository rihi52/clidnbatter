#include "setup.h"

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
static void vCliDC_Setup_ScenarioMenu();
static void vCliDC_Setup_AddRemovePlayers();
static void vCliDC_Setup_DisplayScenarios();

/*========================================================================*
 *  SECTION - Local variables                                             *
 *========================================================================*
 */
static char players[CHARACTER_BUFFER];
static char monsters[MONSTER_BUFFER];

 /*=======================================================================*
 *  SECTION - Local function definitions                                  *
 *========================================================================*
 */
static void vCliDC_Setup_ScenarioMenu()
{
    int loop = 1;
    while (1 == loop)
    {
        printf("\n*** Scenario Setup Menu ***\n");

        char choice[SMALL_BUFFER_BYTE];
        memset(choice, '\0', sizeof(choice));
        int check = 1;

        printf("\nHome Options:\n");
        printf( "p: Add/Remove players\n"
                "m: Add/Remove monsters\n"
                "x: Return to setup menu\n"
                "Please choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'p' || choice[0] == 'm' || choice[0] == 'x'))
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
            case 'p':
                vCliDC_Setup_AddRemovePlayers();
                break;

            case 'm':
                
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

static void vCliDC_Setup_AddRemovePlayers()
{
    char prompt[10];

    printf("\n** Player Menu **\n");    

    char namePlayers[CHARACTER_BUFFER];
    char endchar = ' ';
    int length, startPosition = 0, loop = 0;

    /* Loop to acquire player information
     * Two loop statuses so the user input functions can be returned to if needed */
    while (0 == loop || 1 == loop)
    {
        //startPosition = 0;
        if (0 == loop)
        {
            startPosition = 0;
            /* Add new players if desired */
            do /* Keep asking if 'y' for yes or 'n' for no isn't entered */
            {
                printf("Do you wish to add or remove players (a/r)?: ");
                fgets(prompt, sizeof(prompt), stdin);
            } while (strlen(prompt) != 1 && ('a' != prompt[0] && 'r' != prompt[0] && 'x' != prompt[0]));

            /* If statment to add or remove players - NEEDS optimized TODO */
            if ('a' == prompt[0] && '\n' == prompt[1])
            {   /* Add players TODO */
                while (0 != CliDC_Combat_ChoosePlayers(players, CHARACTER_BUFFER))
                {
                    /* Return to home menu if 'x' entered */
                    return;
                }
                length = strlen(players);
                loop = 1;

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

                if ('\0' != namePlayers[0])
                {
                    // TODO: Add chosen players to scenario in db
                }
                else
                {
                    loop = 0;
                    continue;
                }

            }
            else if('r' == prompt[0] && '\n' == prompt[1])
            {   /* Remove players TODO */
                while (0 != CliDC_Combat_ChoosePlayers(players, CHARACTER_BUFFER))
                {
                    /* Return to home menu if 'x' entered */
                    return;
                }
                length = strlen(players);
                loop = 1;

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

                if ('\0' != namePlayers[0])
                {
                    // TODO: Remove chosen players scenario in db
                }
                else
                {
                    loop = 0;
                    continue;
                }

            }

        }

        

        /* If there is no name do not attempt to create a player struct and restart loop */
        

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

    
    
    /* Skips if statement and returns if 'x' is chosen */
    return;
}

static void vCliDC_Setup_DisplayScenarios()
{   
    // TODO
    return;
}

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void gvCliDC_Setup_Main()
{
    int loop = 1;
    while (1 == loop)
    {
        printf("\n*** Scenario Setup Menu ***\n");

        char choice[SMALL_BUFFER_BYTE];
        memset(choice, '\0', sizeof(choice));
        int check = 1;

        printf("\nHome Options:\n");
        printf( "n: New scenario\n"
                "m: Modify scenario\n"
                "l: Launch scenario\n"
                "x: Return to home menu\n"
                "Please choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'n' || choice[0] == 'm' || choice[0] == 'l' || choice[0] == 'x'))
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
                vCliDC_Setup_ScenarioMenu();
                break;

            case 'm':
                vCliDC_Setup_DisplayScenarios();
                break;

            case 'l':
                
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
