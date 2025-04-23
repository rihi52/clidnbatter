#ifndef COMBAT_H
#define COMBAT_H

#include "lookup.h"
#include "modify.h"
#include "global.h"
#include "setup.h"
#include "sqlite3.h"
#include <stdbool.h>
#include <stdio.h>

#define INITIATIVE_SPREAD   30
#define SCENARIO_COMBAT     1
#define DIRECT_COMBAT       0

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

void gvCliDC_Combat_Main(int ScenarioOrDirect);
int CliDC_Combat_ChoosePlayers(char *ChosenPlayers, size_t size);
int CliDC_Combat_ChooseMonstsers(char *ChosenPlayers, size_t size);
part *gvCliDC_Combat_CreatePlayer(char *name);
void gvCliDC_Combat_SetInitiative(struct part *person);
int gvCliDC_Combat_ReadsNameAndAsksInitiativeAssignsStruct(int length);
#endif  /*combat.h*/