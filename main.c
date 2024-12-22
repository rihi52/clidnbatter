#include "src/lookup.h"
#include "src/combat.h"

int main (void)
{
    pMonsterDb = NULL;
    gvCliDC_DatabaseOpen();
    char choice[5];
    int loop = 1;
    printf("Welcome to clidnbatter!\n");

    while (loop == 1)
    {
        printf("\n*** Home Menu ***\n");
        int check = 1;
        printf("\nHome Options:\n");
        printf( "l: Database lookup\n"
                "c: Enter combat\n"
                "m: Modify player database\n"
                "x: Exit program (x can always be used to quit current function or return to menu)\n"
                "Please choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'l' || choice[0] == 'c' || choice[0] == 'm' || choice[0] == 'x'))
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
            case 'l':
                gvCliDC_Lookup_Main();
                break;

            case 'c':
                gvCliDC_Combat_Main();
                break;

            case 'm':
                gvCliDC_Modify_MainLoop();
                break;
            
            case 'x':
                loop = 0;
                break;

            default:
                break;
        }
        choice[0] = '\0';
    }
    gvCliDC_DatabaseClose();
}