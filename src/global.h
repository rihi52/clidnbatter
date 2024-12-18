#ifndef GLOBAL_H
#define GLOBAL_H

#include "sqlite3.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*========================================================================* 
 *  SECTION - Global definitions 
 *========================================================================* 
 */
extern sqlite3 *pMonsterDb;

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCliDC_Combat_CheckIntegerInputs(int *numberOf);
int giCliDC_Global_GetInput(char *Buffer);

#endif /* gloabl.h */