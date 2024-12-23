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
    printf("Do you wish to add or remove players (a/r)?: ");

    do /* Keep asking if 'y' for yes or 'n' for no isn't entered */
    {
        printf("Do you wish to add any new players to the database? (y/n): ");
        fgets(prompt, sizeof(prompt), stdin);
    } while (strlen(prompt) != 1 && ('a' != prompt[0] && 'r' != prompt[0] && 'x' != prompt[0]));

    if ('a' == prompt[0] && '\n' == prompt[1])
    {   /* Add players TODO */
        // CliDC_Combat_ChoosePlayers();
    }
    else if('r' == prompt[0] && '\n' == prompt[1])
    {   /* Remove players TODO */

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
