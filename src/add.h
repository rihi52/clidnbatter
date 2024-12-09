#ifndef LOOKUP_H
#define LOOKUP_H

#include "lookup.h"
#include "sqlite3.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*========================================================================* 
 *  SECTION - Global definitions 
 *========================================================================* 
 */

/*========================================================================* 
 *  SECTION - Extern global function prototypes                           * 
 *========================================================================* 
 */
int giCliDC_Add_NewPlayer(char *Name, int16_t Ac, int16_t Hp);

#endif /* lookup.h */