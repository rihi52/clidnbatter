#include "lookup.h"
#include "combat.h"

int main (void)
{
    pMonsterDb = NULL;
    gvCombat_DatabaseOpen();
    char choice[5];
    int loop = 1;
    printf("Welcome to Combat V2.0\n\n");

    while (loop == 1)
    {
        int check = 1;
        printf("Options:\n");
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
                gvCombat_Lookup_Cr();
                break;

            case 'c':
                gvCombat_CombatLoop();
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
    gvCombat_DatabaseClose();
}