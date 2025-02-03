#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "supemon.h"

#define MAX_ITEMS_PER_BATTLE 4

typedef struct {
    Player* player;
    Supemon* player_supemon;
    Supemon* enemy_supemon;
    int items_used;
} Battle;

// Battle initialization
void init_battle(Battle* battle, Player* player, Supemon* enemy);

// Player actions
int use_move(Battle* battle, int move_index);
int change_supemon(Battle* battle, int supemon_index);
int use_item(Battle* battle, int item_index, Supemon* target);
int try_run(Battle* battle);
int try_capture(Battle* battle);

// Enemy actions
void enemy_turn(Battle* battle);

// Battle status checks
int is_battle_over(Battle* battle);
void give_rewards(Battle* battle);

// Helper functions
float calculate_run_chance(Battle* battle);
float calculate_capture_chance(Battle* battle);
void apply_move_effects(Supemon* attacker, Supemon* defender, Move* move);

void display_battle_status(Supemon *enemy, Supemon *player_supemon);
void display_battle_menu(void);
void display_move_menu(void);

#endif // BATTLE_H