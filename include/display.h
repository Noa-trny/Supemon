#ifndef DISPLAY_H
#define DISPLAY_H

#include "player.h"
#include "battle.h"

// display system functionality:
// - handles all game UI elements
// - provides consistent formatting for menus and status displays
// - shows battle information and player choices
// - displays supemon stats and status

// displays battle menu with available actions:
// - move: use supemon abilities
// - change: switch active supemon
// - item: use items from inventory
// - capture: attempt to catch wild supemon
// - run: try to escape battle
void display_battle_menu(void);

// shows current battle status:
// - displays both supemons' stats (HP, level, etc.)
// - shows attack, defense, accuracy, and evasion
// - formats information in an easy-to-read layout
void display_battle_status(Battle *battle);

// displays HP status for all player's supemons:
// - shows current and maximum HP for each supemon
// - indicates which supemon is currently selected
// - reads current values from save file
void display_all_supemons_hp(const Player* player);

#endif // DISPLAY_H 