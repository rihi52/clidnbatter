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
// sqlite3_stmt *stmt;

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCombat_DatabaseOpen();
void gvCombat_DatabaseClose();
void gvCombat_Lookup_Cr();
void gvCombat_Lookup_Hp(int hp);
void gvCombat_Lookup_Name(char *name);

#endif /* lookup.h */