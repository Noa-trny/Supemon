#ifndef CENTER_H
#define CENTER_H

#include "player.h"

// supemon center functionality:
// - provides healing services for all player's supemons
// - restores HP to maximum for each supemon
// - maintains original stats (level, exp, etc.)
// - updates save file after healing

// heals all supemons in player's team to full health
// displays current HP before and after healing
void heal_supemons(Player *player);

#endif // CENTER_H 