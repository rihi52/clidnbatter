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
#define CHARACTER_BUFFER_BYTE   100
#define SMALL_BUFFER_BYTE       10
#define INPUT_BUFFER_BYTE    50

extern sqlite3 *pMonsterDb;

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCliDC_Global_CheckIntegerInputs(int *numberOf);
int giCliDC_Global_GetInput(char *Buffer, size_t Size);

#endif /* gloabl.h */