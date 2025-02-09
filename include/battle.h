#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "supemon.h"

// battle structure managing combat state:
// - player: reference to the current player
// - player_supemon: currently active player's supemon
// - enemy_supemon: wild supemon being battled
// - is_player_turn: flag for turn management
typedef struct {
    Player *player;
    Supemon *player_supemon;
    Supemon *enemy_supemon;
    int is_player_turn;
} Battle;

// core battle functions
void start_battle(Player *player);                          // initiates a wild supemon encounter
void battle_change_supemon(Battle *battle, Player *player); // switches active supemon during battle
void battle_use_item(Battle *battle);                       // uses an item from inventory during battle

#endif // BATTLE_H