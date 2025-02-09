#ifndef UTILS_H
#define UTILS_H

#include "player.h"
#include "supemon.h"

// utility functions for game state management:
// update_player_supemon_stats: saves current supemon stats to player save file
// - updates HP, level, experience, and all battle stats
// - maintains consistency between memory and save file
void update_player_supemon_stats(Player* player, Supemon* supemon);

#endif // UTILS_H