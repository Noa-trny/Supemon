#include "battle.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void apply_move_effects(Supemon* attacker, Supemon* defender, Move* move) {
    // calculate dodge chance based on accuracy and evasion
    float dodge_chance = (float)defender->evasion / (attacker->accuracy + defender->evasion);
    // check if the defender dodges the attack
    if ((float)rand() / RAND_MAX < dodge_chance) {
        printf("%s dodged the attack!\n", defender->name);
        return;
    }

    // handle damage moves
    if (move->damage > 0) {
        // calculate damage based on attack and defense
        int damage = (move->damage * attacker->attack) / defender->defense;
        defender->hp -= damage;
        if (defender->hp < 0) defender->hp = 0;
        printf("%s took %d damage!\n", defender->name, damage);
    }

    // handle stat boost moves
    if (move->stat_boost > 0) {
        // increase attack, defense, or evasion based on the move
        if (strcmp(move->stat_affected, "attack") == 0) {
            attacker->attack += move->stat_boost;
        } else if (strcmp(move->stat_affected, "defense") == 0) {
            attacker->defense += move->stat_boost;
        } else if (strcmp(move->stat_affected, "evasion") == 0) {
            attacker->evasion += move->stat_boost;
        }
        printf("%s's %s increased!\n", attacker->name, move->stat_affected);
    }
}

void enemy_turn(Battle* battle) {
    // randomly select a move for the enemy
    int move_index = rand() % 2;
    Move* selected_move = &battle->enemy_supemon->moves[move_index];
    
    printf("\nEnemy %s used %s!\n", battle->enemy_supemon->name, selected_move->name);
    apply_move_effects(battle->enemy_supemon, battle->player_supemon, selected_move);
    battle->is_player_turn = 1;
}

void display_battle_status(Battle *battle) {
    // display enemy status
    printf("\nYour turn...\n\n");
    printf("%s (enemy)\n", battle->enemy_supemon->name);
    printf("--------------------------------\n");
    printf("HP: %d/%d\tLvl: %d\n", 
           battle->enemy_supemon->hp, 
           battle->enemy_supemon->max_hp, 
           battle->enemy_supemon->level);
    printf("Atk: %d\tDef: %d\n", 
           battle->enemy_supemon->attack, 
           battle->enemy_supemon->defense);
    printf("Acc: %d\tEva: %d\n", 
           battle->enemy_supemon->accuracy, 
           battle->enemy_supemon->evasion);
    printf("--------------------------------\n");
    
    // display player status
    printf("%s (%s)\n", 
           battle->player_supemon->name,
           battle->player->name);
    printf("HP: %d/%d\tLvl: %d\n", 
           battle->player_supemon->hp, 
           battle->player_supemon->max_hp, 
           battle->player_supemon->level);
    printf("Atk: %d\tDef: %d\n", 
           battle->player_supemon->attack, 
           battle->player_supemon->defense);
    printf("Acc: %d\tEva: %d\n", 
           battle->player_supemon->accuracy, 
           battle->player_supemon->evasion);
    printf("--------------------------------\n");
}

void display_battle_menu(void) {
    printf("+-----------------------------+\n");
    printf("|What will you do?           |\n");
    printf("| 1 - Move                   |\n");
    printf("| 2 - Change Supemon         |\n");
    printf("| 3 - Use item               |\n");
    printf("| 4 - Capture                |\n");
    printf("| 5 - Run away               |\n");
    printf("+-----------------------------+\n");
    printf("1, 2, 3, 4 or 5: ");
}

void battle_move(Battle *battle) {
    // prompt user to choose a move
    printf("\nChoose your move:\n");
    printf("1 - %s\n", battle->player_supemon->moves[0].name);
    printf("2 - %s\n", battle->player_supemon->moves[1].name);
    printf("3 - Cancel\n");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 3) return;
    if (choice != 1 && choice != 2) {
        printf("Invalid choice!\n");
        return;
    }
    
    // apply the selected move effects
    Move *selected_move = &battle->player_supemon->moves[choice - 1];
    apply_move_effects(battle->player_supemon, battle->enemy_supemon, selected_move);
    battle->is_player_turn = 0;
}

int battle_run_away(Battle *battle) {
    float chance = (float)battle->player_supemon->speed / 
                  (battle->player_supemon->speed + battle->enemy_supemon->speed);
    
    if ((float)rand() / RAND_MAX < chance) {
        printf("Got away safely!\n");
        return 1;
    }
    printf("Can't escape!\n");
    battle->is_player_turn = 0;
    return 0;
}

int battle_capture(Battle *battle, Player *player) {
    // calculate capture chance based on enemy's HP
    float chance = (float)(battle->enemy_supemon->max_hp - battle->enemy_supemon->hp) / 
                  battle->enemy_supemon->max_hp - 0.5;
    
    if ((float)rand() / RAND_MAX < chance) {
        int slot = 0;
        // find an empty slot to store the captured Supemon
        while (slot < MAX_SUPEMON && player->supemons[slot][0] != '\0') {
            slot++;
        }
        
        if (slot < MAX_SUPEMON) {
            // store the captured Supemon's name
            strncpy(player->supemons[slot], 
                   battle->enemy_supemon->name, 
                   sizeof(player->supemons[slot]) - 1);
            player->supemons[slot][sizeof(player->supemons[slot]) - 1] = '\0';
            printf("%s was caught!\n", battle->enemy_supemon->name);
            return 1;
        } else {
            printf("No room for more Supemon!\n");
        }
    } else {
        printf("The wild %s broke free!\n", battle->enemy_supemon->name);
        battle->is_player_turn = 0;
    }
    return 0;
}

void calculate_rewards(Player* player, Battle* battle) {
    int supcoins = 100 + (rand() % 401);
    player->supcoins += supcoins;
    printf("You earned %d Supcoins!\n", supcoins);
    
    int exp_multiplier = 100 + (rand() % 401);
    int exp_gained = exp_multiplier * battle->enemy_supemon->level;
    battle->player_supemon->exp += exp_gained;
    printf("%s gained %d experience!\n", 
           battle->player_supemon->name, 
           exp_gained);
}

void initialize_battle(Battle* battle, Player* player) {
    battle->player = player;
    battle->player_supemon = create_supemon_by_name(player->selected_supemon);
    
    int enemy_choice = rand() % 3;
    switch (enemy_choice) {
        case 0: battle->enemy_supemon = create_supemon_copy(&SUPMANDER); break;
        case 1: battle->enemy_supemon = create_supemon_copy(&SUPASAUR); break;
        case 2: battle->enemy_supemon = create_supemon_copy(&SUPIRTLE); break;
    }
    
    while (battle->enemy_supemon->level < battle->player_supemon->level) {
        level_up_supemon(battle->enemy_supemon);
    }
    
    battle->is_player_turn = (battle->player_supemon->speed > battle->enemy_supemon->speed) ? 1 :
                            (battle->player_supemon->speed < battle->enemy_supemon->speed) ? 0 :
                            (rand() % 2);
}

void start_battle(Player *player) {
    Battle battle;
    initialize_battle(&battle, player);
    
    // announce the appearance of the enemy Supemon
    printf("\nA wild %s appeared!\n", battle.enemy_supemon->name);
    
    while (1) {
        // check if player's Supemon fainted
        if (battle.player_supemon->hp <= 0) {
            printf("%s fainted! You lost!\n", battle.player_supemon->name);
            break;
        }
        
        // check if enemy's Supemon fainted
        if (battle.enemy_supemon->hp <= 0) {
            printf("Enemy %s fainted! You won!\n", battle.enemy_supemon->name);
            calculate_rewards(player, &battle);
            break;
        }
        
        if (battle.is_player_turn) {
            display_battle_status(&battle);
            display_battle_menu();
            
            int choice;
            scanf("%d", &choice);
            
            switch (choice) {
                case 1: battle_move(&battle); break;
                case 2: battle_change_supemon(&battle, player); break;
                case 3: battle_use_item(&battle); break;
                case 4: if (battle_capture(&battle, player)) return;
                    break;
                case 5: if (battle_run_away(&battle)) return;
                    break;
                default:
                    printf("Invalid choice!\n");
                    continue;
            }
        } else {
            enemy_turn(&battle);
        }
    }
    
    // free allocated memory for Supemons
    free(battle.player_supemon);
    free(battle.enemy_supemon);
}

void battle_change_supemon(Battle *battle, Player *player) {
    printf("\nChoose a Supemon:\n");
    int available_count = 0;
    
    // display available Supemons
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        // skip currently active Supemon
        if (strcmp(player->supemons[i], battle->player_supemon->name) != 0) {
            printf("%d - %s\n", ++available_count, player->supemons[i]);
        }
    }
    
    if (available_count == 0) {
        printf("No other Supemons available!\n");
        return;
    }
    
    printf("%d - Cancel\n", available_count + 1);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == available_count + 1) return;
    if (choice < 1 || choice > available_count) {
        printf("Invalid choice!\n");
        return;
    }
    
    // find and switch to the chosen Supemon
    int current_index = 0;
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        if (strcmp(player->supemons[i], battle->player_supemon->name) != 0) {
            current_index++;
            if (current_index == choice) {
                // update selected Supemon
                strncpy(player->selected_supemon, player->supemons[i], sizeof(player->selected_supemon) - 1);
                player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
                
                // free current Supemon and create new one
                free(battle->player_supemon);
                battle->player_supemon = create_supemon_by_name(player->supemons[i]);
                
                printf("Go %s!\n", battle->player_supemon->name);
                battle->is_player_turn = 0;
                return;
            }
        }
    }
}

void battle_use_item(Battle* battle) {
    printf("\nChoose an item:\n");
    int available_count = 0;
    
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (battle->player->items[i].quantity > 0) {
            printf("%d - %s (x%d)\n", ++available_count, 
                   battle->player->items[i].name,
                   battle->player->items[i].quantity);
        }
    }
    
    if (available_count == 0) {
        printf("No items available!\n");
        return;
    }
    
    printf("%d - Cancel\n", available_count + 1);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == available_count + 1) return;
    if (choice < 1 || choice > available_count) {
        printf("Invalid choice!\n");
        return;
    }
    
    int item_index = -1;
    int current_index = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (battle->player->items[i].quantity > 0) {
            current_index++;
            if (current_index == choice) {
                item_index = i;
                break;
            }
        }
    }
    
    if (item_index >= 0) {
        use_item(&battle->player->items[item_index], battle->player_supemon);
        battle->is_player_turn = 0;
    }
}