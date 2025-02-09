#include "battle.h"
#include "save.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "utils.h"

static void apply_move_effects(Supemon* attacker, Supemon* defender, Move* move) {
    float dodge_chance = (float)defender->evasion / (attacker->accuracy + defender->evasion);
    if ((float)rand() / RAND_MAX < dodge_chance) {
        printf("%s dodged the attack!\n", defender->name);
        return;
    }

    if (move->damage > 0) {
        int base_damage = move->damage * attacker->attack;
        int damage = base_damage / (defender->defense / 2 + 1);
        if (damage < 1) damage = 1;
        defender->hp -= damage;
        if (defender->hp < 0) defender->hp = 0;
        printf("%s took %d damage!\n", defender->name, damage);
    }

    if (move->stat_boost > 0) {
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

static void enemy_turn(Battle* battle) {
    int move_index = rand() % 2;
    Move* selected_move = &battle->enemy_supemon->moves[move_index];
    
    printf("\nEnemy %s used %s!\n", battle->enemy_supemon->name, selected_move->name);
    apply_move_effects(battle->enemy_supemon, battle->player_supemon, selected_move);
    battle->is_player_turn = 1;
}

static void display_battle_status(Battle *battle) {
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

static void display_battle_menu(void) {
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

static void battle_move(Battle *battle) {
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
    
    Move *selected_move = &battle->player_supemon->moves[choice - 1];
    apply_move_effects(battle->player_supemon, battle->enemy_supemon, selected_move);
    battle->is_player_turn = 0;
}

static int battle_run_away(Battle *battle) {
    float chance = (float)battle->player_supemon->speed / 
                  (battle->player_supemon->speed + battle->enemy_supemon->speed);
    
    if ((float)rand() / RAND_MAX < chance) {
        printf("Got away safely!\n");
        update_player_supemon_stats(battle->player, battle->player_supemon);
        return 1;
    }
    printf("Can't escape!\n");
    battle->is_player_turn = 0;
    return 0;
}

static int battle_capture(Battle *battle, Player *player) {
    float chance = (float)(battle->enemy_supemon->max_hp - battle->enemy_supemon->hp) / 
                  battle->enemy_supemon->max_hp - 0.5f;
    
    if ((float)rand() / RAND_MAX < chance) {
        int slot = 0;
        while (slot < MAX_SUPEMON && player->supemons[slot][0] != '\0') {
            slot++;
        }
        
        if (slot < MAX_SUPEMON) {
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

static void calculate_rewards(Player* player, Battle* battle) {
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

static void load_supemon_stats(Supemon* supemon, cJSON* supemon_json) {
    supemon->level = cJSON_GetObjectItem(supemon_json, "level")->valueint;
    supemon->exp = cJSON_GetObjectItem(supemon_json, "exp")->valueint;
    supemon->hp = cJSON_GetObjectItem(supemon_json, "hp")->valueint;
    supemon->max_hp = cJSON_GetObjectItem(supemon_json, "max_hp")->valueint;
    supemon->attack = cJSON_GetObjectItem(supemon_json, "attack")->valueint;
    supemon->defense = cJSON_GetObjectItem(supemon_json, "defense")->valueint;
    supemon->evasion = cJSON_GetObjectItem(supemon_json, "evasion")->valueint;
    supemon->accuracy = cJSON_GetObjectItem(supemon_json, "accuracy")->valueint;
    supemon->speed = cJSON_GetObjectItem(supemon_json, "speed")->valueint;
}

static void initialize_battle(Battle* battle, Player* player) {
    battle->player = player;
    battle->player_supemon = NULL;
    
    FILE *file = fopen("save.json", "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';

        cJSON *json = cJSON_Parse(data);
        free(data);
        
        if (json) {
            cJSON *player_json = cJSON_GetObjectItem(json, player->name);
            if (player_json) {
                cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                if (supemons_array) {
                    cJSON *supemon_json;
                    cJSON_ArrayForEach(supemon_json, supemons_array) {
                        const char *supemon_name = cJSON_GetObjectItem(supemon_json, "name")->valuestring;
                        if (strcmp(supemon_name, player->selected_supemon) == 0) {
                            battle->player_supemon = create_supemon_by_name(supemon_name);
                            if (battle->player_supemon) {
                                load_supemon_stats(battle->player_supemon, supemon_json);
                            }
                            break;
                        }
                    }
                }
            }
            cJSON_Delete(json);
        }
    }
    
    if (!battle->player_supemon) {
        battle->player_supemon = create_supemon_by_name(player->selected_supemon);
    }
    
    int enemy_choice = rand() % 3;
    switch (enemy_choice) {
        case 0: battle->enemy_supemon = create_supemon_copy(&SUPMANDER); break;
        case 1: battle->enemy_supemon = create_supemon_copy(&SUPASAUR); break;
        case 2: battle->enemy_supemon = create_supemon_copy(&SUPIRTLE); break;
    }
    
    while (battle->enemy_supemon->level < battle->player_supemon->level) {
        level_up_supemon(battle->enemy_supemon);
    }
    
    battle->is_player_turn = battle->player_supemon->speed != battle->enemy_supemon->speed ?
                            battle->player_supemon->speed > battle->enemy_supemon->speed :
                            (rand() % 2) == 0;
}

void start_battle(Player *player) {
    printf("\nBefore battle:");
    display_all_supemons_hp(player);
    
    Battle battle;
    initialize_battle(&battle, player);
    printf("\nA wild %s appeared!\n", battle.enemy_supemon->name);
    
    while (1) {
        if (battle.player_supemon->hp <= 0) {
            printf("%s fainted! You lost!\n", battle.player_supemon->name);
            update_player_supemon_stats(player, battle.player_supemon);
            break;
        }
        
        if (battle.enemy_supemon->hp <= 0) {
            printf("Enemy %s fainted! You won!\n", battle.enemy_supemon->name);
            calculate_rewards(player, &battle);
            update_player_supemon_stats(player, battle.player_supemon);
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
                case 4: 
                    if (battle_capture(&battle, player)) {
                        update_player_supemon_stats(player, battle.player_supemon);
                        free(battle.player_supemon);
                        free(battle.enemy_supemon);
                        printf("\nAfter battle:");
                        display_all_supemons_hp(player);
                        return;
                    }
                    break;
                case 5: 
                    if (battle_run_away(&battle)) {
                        free(battle.player_supemon);
                        free(battle.enemy_supemon);
                        printf("\nAfter battle:");
                        display_all_supemons_hp(player);
                        return;
                    }
                    break;
                default:
                    printf("Invalid choice!\n");
                    continue;
            }
        } else {
            enemy_turn(&battle);
        }

        update_player_supemon_stats(player, battle.player_supemon);
    }
    
    free(battle.player_supemon);
    free(battle.enemy_supemon);
    
    printf("\nAfter battle:");
    display_all_supemons_hp(player);
}

void battle_change_supemon(Battle *battle, Player *player) {
    printf("\nChoose a Supemon:\n");
    int available_count = 0;
    
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
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
    
    int current_index = 0;
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        if (strcmp(player->supemons[i], battle->player_supemon->name) != 0) {
            current_index++;
            if (current_index == choice) {
                strncpy(player->selected_supemon, player->supemons[i], sizeof(player->selected_supemon) - 1);
                player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
                
                free(battle->player_supemon);
                
                FILE *file = fopen("save.json", "r");
                if (file) {
                    fseek(file, 0, SEEK_END);
                    long length = ftell(file);
                    fseek(file, 0, SEEK_SET);
                    
                    char *data = malloc(length + 1);
                    fread(data, 1, length, file);
                    fclose(file);
                    data[length] = '\0';

                    cJSON *json = cJSON_Parse(data);
                    free(data);
                    
                    if (json) {
                        cJSON *player_json = cJSON_GetObjectItem(json, player->name);
                        if (player_json) {
                            cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                            if (supemons_array) {
                                cJSON *supemon_json;
                                cJSON_ArrayForEach(supemon_json, supemons_array) {
                                    const char *supemon_name = cJSON_GetObjectItem(supemon_json, "name")->valuestring;
                                    if (strcmp(supemon_name, player->selected_supemon) == 0) {
                                        battle->player_supemon = create_supemon_by_name(supemon_name);
                                        if (battle->player_supemon) {
                                            load_supemon_stats(battle->player_supemon, supemon_json);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        cJSON_Delete(json);
                    }
                }
                
                if (!battle->player_supemon) {
                    battle->player_supemon = create_supemon_by_name(player->supemons[i]);
                }
                
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