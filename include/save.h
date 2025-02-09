#ifndef SAVE_H
#define SAVE_H

#include "player.h"
#include "supemon.h"

// save system functions for game state persistence:
// - uses JSON format for save files
// - handles player data, supemons, and items

// saves complete player data to save file:
// - player info (name, supcoins)
// - owned supemons and their stats
// - inventory items
void save_player(const Player *player);

// loads player data from save file by name:
// - restores player state
// - loads owned supemons
// - restores inventory
void load_player(Player *player, const char *name);

// saves current game state after important actions:
// - battle results
// - shop purchases
// - healing
void save_game_state(Player *player);

// updates specific supemon's stats in save file:
// - maintains consistency after battles/healing
// - preserves experience and level progress
void update_player_supemon_stats(Player* player, Supemon* supemon);

// displays HP status of all player's supemons:
// - reads from save file for current values
// - shows current/max HP
// - indicates selected supemon
void display_all_supemons_hp(const Player* player);

#endif // SAVE_H