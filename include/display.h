#ifndef DISPLAY_H
#define DISPLAY_H

#include "player.h"
#include "battle.h"

void display_battle_menu(void);
void display_battle_status(Battle *battle);
void display_all_supemons_hp(const Player* player);

#endif // DISPLAY_H 