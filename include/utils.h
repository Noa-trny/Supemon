#ifndef UTILS_H
#define UTILS_H

#include "player.h"
#include "supemon.h"

// utility functions for game mechanics:
// - level_up_supemon: handles stat increases on level up
// - create_supemon_by_name: creates new supemon instance
// - create_supemon_copy: creates copy of template supemon
// - update_player_supemon_stats: updates supemon stats in save

void level_up_supemon(Supemon* supemon);                   // increases supemon level and stats
Supemon* create_supemon_by_name(const char* name);         // creates new supemon from name
Supemon* create_supemon_copy(const Supemon* template);     // creates copy of template supemon
void update_player_supemon_stats(Player *player, Supemon *supemon); // updates supemon stats in save

#endif // UTILS_H