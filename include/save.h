#ifndef SAVE_H
#define SAVE_H

#include "player.h"
#include "supemon.h"

// save system functions:
// - save_player: saves player data to JSON file
// - load_player: loads player data from JSON file
// - update_player_supemon_stats: updates supemon stats in save file

void save_player(const Player *player);                    // saves all player data to save.json
void load_player(Player *player, const char *name);        // loads player data from save.json
void update_player_supemon_stats(Player *player, Supemon *supemon); // updates supemon stats in save

// displays HP status of all player's supemons:
// - reads from save file for current values
// - shows current/max HP
// - indicates selected supemon
void display_all_supemons_hp(const Player* player);

#endif // SAVE_H