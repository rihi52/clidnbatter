#ifndef ADD_H
#define ADD_H

#include "sqlite3.h"
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
int giCliDC_Add_NewPlayer(char *Name, int16_t Ac, int16_t Hp);

#endif /* add.h */