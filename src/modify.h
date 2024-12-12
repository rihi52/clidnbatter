#ifndef MODIFY_H
#define MODIFY_H

#include "sqlite3.h"
#include "lookup.h"
#include "global.h"
#include <stdint.h>
#include <stdio.h>

/*========================================================================* 
 *  SECTION - Global definitions 
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
void gvCliDC_Modify_MainLoop();
void gvCliDC_Modify_EnterPlayerInformation();
int giCliDC_Modify_NewPlayer(char *Name, int16_t Ac, int16_t Hp);

#endif /* modify.h */