#ifndef SAVE_H
#define SAVE_H

#include "player.h"
#include "supemon.h"

void save_player(const Player *player);
void load_player(Player *player, const char *name);
void save_game_state(Player *player);
void update_player_supemon_stats(Player* player, Supemon* supemon);
void display_all_supemons_hp(const Player* player);

#endif // SAVE_H