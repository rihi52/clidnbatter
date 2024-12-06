#ifndef LOOKUP_H
#define LOOKUP_H

#include "sqlite3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*========================================================================* 
 *  SECTION - Global definitions 
 *========================================================================* 
 */
extern sqlite3 *pMonsterDb;

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCliDC_DatabaseOpen();
void gvCliDC_DatabaseClose();
void gvCliDC_Lookup_Cr();
void gvCliDC_Lookup_Hp(int hp);
void gvCliDC_Lookup_Name(char *name);

#endif /* lookup.h */