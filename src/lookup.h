#ifndef LOOKUP_H
#define LOOKUP_H

#include "combat.h"
#include "sqlite3.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*========================================================================* 
 *  SECTION - Global definitions 
 *========================================================================* 
 */
extern sqlite3 *pMonsterDb;

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCliDC_Lookup_Main();
void gvCliDC_DatabaseOpen();
void gvCliDC_DatabaseClose();
int giCliDC_Lookup_PlayerAc(char *Name);
int giCliDC_Lookup_PlayerHp(char *Name);

#endif /* lookup.h */