#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "supemon.h"

typedef struct {
    Player *player;
    Supemon *player_supemon;
    Supemon *enemy_supemon;
    int is_player_turn;
} Battle;

void start_battle(Player *player);
void battle_change_supemon(Battle *battle, Player *player);
void battle_use_item(Battle *battle);

#endif // BATTLE_H