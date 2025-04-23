#ifndef SETUP_H
#define SETUP_H

#include "combat.h"
#include "sqlite3.h"
#include "global.h"

/*========================================================================*
 *  SECTION - Global definitions
 *========================================================================*
 */
#define SCENARIO_MENU   0
#define COMBAT_MENU     1
#define ALLOWED_MONSTERS 20

/*========================================================================*
 *  SECTION - Global variables
 *========================================================================*
 */
extern char PlayersInScenario[CHARACTER_BUFFER];
extern char MonstersInScenario[MONSTER_BUFFER];
extern int MonsterInitiativesInScenario[ALLOWED_MONSTERS];

/*========================================================================*
 *  SECTION - Extern global function prototypes                           *
 *========================================================================*
 */
void gvCliDC_Setup_Main();
void gvCliDC_Setup_CountScenarios(int ScenarioOrCombatMenu);
void gvCliDC_Setup_FindParticipant(int ScenarioID);


#endif /* setup.h */