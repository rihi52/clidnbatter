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
/* Buffers */
#define CHARACTER_BUFFER_BYTE       100
#define SMALL_BUFFER_BYTE           10
#define INPUT_BUFFER_BYTE           50
#define YES_NO_INPUT_BUFFER_BYTE    5
#define CHARACTER_BUFFER            250
#define MONSTER_BUFFER              350

/* Signals */
#define YES_INPUT                   0
#define NO_INPUT                    1
#define INPUT_ERROR                 2
#define X_INPUT_DETECTED            6
#define PLAYER                      0
#define MONSTER                     1

extern sqlite3 *pMonsterDb;

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCliDC_Global_CheckIntegerInputs(int *numberOf);
int giCliDC_Global_GetTextInput(char *Buffer, size_t Size);
int giCliDC_Global_YesNoInput(char *Buffer, size_t Size);
sqlite3_stmt *CliDC_Global_PrepareAndBindText(const char *sql, const char *BindValue);

#endif /* gloabl.h */