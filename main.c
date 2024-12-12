#include "src/lookup.h"
#include "src/combat.h"

int main (void)
{
    pMonsterDb = NULL;
    gvCliDC_DatabaseOpen();
    char choice[5];
    int loop = 1;
    printf("Welcome to clidnbatter!\n\n");

    while (loop == 1)
    {
        int check = 1;
        printf("Home Options:\n");
        printf("l: lookup a monster(s)\nc: enter combat\na: add a player stat block\nx: exit program\nPlease choose from the above: ");
        while (check == 1)
        {
            fgets(choice, sizeof(choice), stdin);
            if (isalpha(choice[0]) && (choice[0] == 'l' || choice[0] == 'c' || choice[0] == 'a' || choice[0] == 'x'))
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
                gvCliDC_Lookup_Cr();
                break;

            case 'c':
                gvCliDC_Combat_Main();
                break;

            case 'a':

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