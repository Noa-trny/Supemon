#include "battle.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void init_battle(Battle* battle, Player* player, Supemon* enemy) {
    battle->player = player;
    battle->player_supemon = &player->supemons[0];
    battle->enemy_supemon = enemy;
    battle->items_used = 0;
}

float calculate_run_chance(Battle* battle) {
    float player_speed = battle->player_supemon->speed;
    float enemy_speed = battle->enemy_supemon->speed;
    return player_speed / (player_speed + enemy_speed);
}

float calculate_capture_chance(Battle* battle) {
    float enemy_max_hp = battle->enemy_supemon->max_hp;
    float enemy_current_hp = battle->enemy_supemon->hp;
    return ((enemy_max_hp - enemy_current_hp) / enemy_max_hp) - 0.5;
}

int try_run(Battle* battle) {
    float chance = calculate_run_chance(battle);
    float roll = (float)rand() / RAND_MAX;
    return roll < chance;
}

int try_capture(Battle* battle) {
    float chance = calculate_capture_chance(battle);
    float roll = (float)rand() / RAND_MAX;
    
    if (roll < chance) {
        // Add enemy supemon to player's collection
        // TODO: Implement add_supemon_to_player function
        return 1;
    }
    return 0;
}

void enemy_turn(Battle* battle) {
    // Randomly select a move
    int move_index = rand() % 2;
    apply_move_effects(battle->enemy_supemon, battle->player_supemon, 
                      &battle->enemy_supemon->moves[move_index]);
}

void give_rewards(Battle* battle) {
    // Give Supcoins (100-500)
    int supcoins = 100 + (rand() % 401);
    battle->player->supcoins += supcoins;
    
    // Give experience (100-500 * enemy level)
    int exp_multiplier = 100 + (rand() % 401);
    int exp_gained = exp_multiplier * battle->enemy_supemon->level;
    battle->player_supemon->exp += exp_gained;
    
    // TODO: Implement level up logic when exp reaches threshold
}

void apply_move_effects(Supemon* attacker, Supemon* defender, Move* move) {
    // Calculate dodge chance
    float dodge_chance = (float)attacker->accuracy / (attacker->accuracy + defender->evasion) + 0.1;
    float roll = (float)rand() / RAND_MAX;

    if (roll < dodge_chance) {
        printf("%s dodged the attack!\n", defender->name);
        return; // Attack missed
    }

    if (move->damage > 0) {
        // Calculate damage considering attack and defense
        float damage = (float)attacker->attack * move->damage / defender->defense;
        int final_damage = (int)damage; // Initial damage calculation

        // Rounding logic
        if ((damage - final_damage) > 0.5) {
            final_damage += 1; // Round up
        }

        defender->hp -= final_damage;
        if (defender->hp < 0) defender->hp = 0;
        printf("%s took %d damage!\n", defender->name, final_damage);
    }

    if (move->stat_boost > 0) {
        // Apply stat boost
        if (strcmp(move->stat_affected, "attack") == 0) {
            attacker->attack += move->stat_boost;
        } else if (strcmp(move->stat_affected, "defense") == 0) {
            attacker->defense += move->stat_boost;
        } else if (strcmp(move->stat_affected, "evasion") == 0) {
            attacker->evasion += move->stat_boost;
        }
    }
}

void display_battle_status(Supemon *enemy, Supemon *player_supemon) {
    printf("Your turn...\n\n");
    
    // Affichage du Supemon ennemi
    printf("%s (enemy)\n", enemy->name);
    printf("--------------------------------\n");
    printf("HP: %d/%d\tLvl: %d\n", enemy->hp, enemy->max_hp, enemy->level);
    printf("Atk: %d\tDef: %d\n", enemy->attack, enemy->defense);
    printf("Acc: %d\tEva: %d\n", enemy->accuracy, enemy->evasion);
    printf("--------------------------------\n");
    
    // Affichage du Supemon du joueur
    printf("%s\n", player_supemon->name);
    printf("HP: %d/%d\tLvl: %d\n", player_supemon->hp, player_supemon->max_hp, player_supemon->level);
    printf("Atk: %d\tDef: %d\n", player_supemon->attack, player_supemon->defense);
    printf("Acc: %d\tEva: %d\n", player_supemon->accuracy, player_supemon->evasion);
    printf("--------------------------------\n\n");
}

void display_battle_menu(void) {
    printf("+--------------------------------+\n");
    printf("|What will you do?               |\n");
    printf("| 1 - Move                       |\n");
    printf("| 2 - Change Supémon             |\n");
    printf("| 3 - Use item                   |\n");
    printf("| 4 - Capture                    |\n");
    printf("| 5 - Run away                   |\n");
    printf("+--------------------------------+\n");
    printf("1, 2, 3, 4 or 5: ");
}

void display_move_menu(void) {
    printf("1 - Scratch\n");
    printf("2 - Grawl\n");
    printf("3 - Cancel\n");
    printf("1, 2 or 3: ");
}