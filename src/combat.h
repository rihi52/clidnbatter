#ifndef COMBAT_H
#define COMBAT_H

#include "lookup.h"
#include "modify.h"
#include "global.h"
#include "sqlite3.h"
#include <stdbool.h>
#include <stdio.h>

#define INITIATIVE_SPREAD   30

typedef struct part{
    char *name;
    bool uniqueChar; // True for player character or Unique Enemy
    int initiative;
    int initiativeSpot;
    int turnCount;
    int ac;
    int MaxHp;
    int hp;
    bool isMalloc;
    struct part *next;
}part;

void gvCliDC_Combat_Main(void);
int CliDC_Combat_ChoosePlayers(char *ChosenPlayers, size_t size);
int CliDC_Combat_ChooseMonstsers(char *ChosenPlayers, size_t size);
#endif  /*combat.h*/