#ifndef COMBAT_H
#define COMBAT_H

#include "lookup.h"
#include "add.h"
#include "sqlite3.h"
#include <stdbool.h>
#include <stdio.h>

#define INITIATIVE_SPREAD 30

typedef struct part{
    char *name;
    bool uniqueChar; // True for player character or Unique Enemy
    int initiative;
    int initiativeSpot;
    int turnCount;
    int ac;
    int hp;
    bool isMalloc;
    struct part *next;
}part;

// part enemies[] = {{"Orc", false, 0, 0, 0, 13, 15, NULL}, {"Orog", false, 0, 0, 0, 18, 53, NULL}}; // Test - do I need individually named structs or array of structs?

void gvCliDC_Combat_Main(void);

#endif  /*combat.h*/