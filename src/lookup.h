#ifndef LOOKUP_H
#define LOOKUP_H

#include "combat.h"
#include "sqlite3.h"
#include "global.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*========================================================================*
 *  SECTION - Global definitions
 *========================================================================*
 */


/*========================================================================*
 *  SECTION - Extern global function prototypes                           *
 *========================================================================*
 */
void gvCliDC_Lookup_Main();
void gvCliDC_DatabaseOpen();
void gvCliDC_DatabaseClose();
int giCliDC_Lookup_PlayerAc(char *Name);
int giCliDC_Lookup_PlayerHp(char *Name);
int giCliDC_Lookup_ScenarioExist(char *Name);
void gvCliDC_Lookup_PrintSingleScenario(int ScenarioID, int ChosenOrDisplay);

#endif /* lookup.h */
