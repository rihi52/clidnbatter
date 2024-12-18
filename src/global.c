#include "global.h"

/*========================================================================* 
 *  SECTION - Local definitions 
 *========================================================================* 
 */
#define CHARACTER_BUFFER_BYTE   100
#define SMALL_BUFFER_BYTE       10

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

void gvCliDC_Combat_CheckIntegerInputs(int *numberOf)
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
}

int giCliDC_Global_GetInput(char *Buffer)
{
    int result = 0;
    fgets(Buffer, sizeof(Buffer), stdin);
        if (Buffer[0] != '\n' && Buffer[0] != ' ')
        {
            Buffer[strcspn(Buffer, "\n")] = '\0';
        }
        else if ('x' == Buffer[0] && '\n' == Buffer[1])
        {
            result = 2;
        }
        else
        {
            printf("Error: blank input. Starting over.\n");
            result = 1;
        }

        return result;
}