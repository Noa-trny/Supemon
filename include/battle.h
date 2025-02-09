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

void battle_move(Battle *battle);
void battle_change_supemon(Battle *battle, Player *player);
int battle_run_away(Battle *battle);
int battle_capture(Battle *battle, Player *player);
void battle_use_item(Battle *battle);

void display_battle_status(Battle *battle);
void display_battle_menu(void);
void enemy_turn(Battle *battle);
void calculate_rewards(Player *player, Battle *battle);
void initialize_battle(Battle *battle, Player *player);

#endif // BATTLE_H