#include "save.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "cJSON.h"
#include "supemon.h" 

// saves complete player data to JSON file:
// - reads existing save file if present
// - updates or creates player entry
// - saves: supcoins, supemons (with stats), items
void save_player(const Player *player) {
    // try to read existing save file
    FILE *file = fopen("save.json", "r");
    cJSON *json = NULL;
    cJSON *old_json = NULL;

    if (file) {
        // read and parse existing save data
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';
        json = cJSON_Parse(data);
        old_json = cJSON_Parse(data);
        free(data);
    }

    if (!json) {
        json = cJSON_CreateObject();
    }

    // create player JSON object
    cJSON *player_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(player_json, "supcoins", player->supcoins);

    // save supemons with their stats
    cJSON *supemons_array = cJSON_CreateArray();
    
    // try to preserve existing supemon stats
    cJSON *old_player_json = NULL;
    cJSON *old_supemons_array = NULL;
    if (old_json) {
        old_player_json = cJSON_GetObjectItem(old_json, player->name);
        if (old_player_json) {
            old_supemons_array = cJSON_GetObjectItem(old_player_json, "supemons");
        }
    }

    // save each supemon's data
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        cJSON *supemon_json = cJSON_CreateObject();
        
        if (old_supemons_array) {
            cJSON *old_supemon;
            cJSON_ArrayForEach(old_supemon, old_supemons_array) {
                if (strcmp(cJSON_GetObjectItem(old_supemon, "name")->valuestring, player->supemons[i]) == 0) {
                    // Copier toutes les stats existantes
                    cJSON_AddStringToObject(supemon_json, "name", cJSON_GetObjectItem(old_supemon, "name")->valuestring);
                    cJSON_AddNumberToObject(supemon_json, "level", cJSON_GetObjectItem(old_supemon, "level")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "exp", cJSON_GetObjectItem(old_supemon, "exp")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "hp", cJSON_GetObjectItem(old_supemon, "hp")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "max_hp", cJSON_GetObjectItem(old_supemon, "max_hp")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "attack", cJSON_GetObjectItem(old_supemon, "attack")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "defense", cJSON_GetObjectItem(old_supemon, "defense")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "evasion", cJSON_GetObjectItem(old_supemon, "evasion")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "accuracy", cJSON_GetObjectItem(old_supemon, "accuracy")->valueint);
                    cJSON_AddNumberToObject(supemon_json, "speed", cJSON_GetObjectItem(old_supemon, "speed")->valueint);
                    break;
                }
            }
        }
        
        if (cJSON_GetObjectItem(supemon_json, "name") == NULL) {
            Supemon* supemon = create_supemon_by_name(player->supemons[i]);
            if (supemon) {
                cJSON_AddStringToObject(supemon_json, "name", supemon->name);
                cJSON_AddNumberToObject(supemon_json, "level", supemon->level);
                cJSON_AddNumberToObject(supemon_json, "exp", supemon->exp);
                cJSON_AddNumberToObject(supemon_json, "hp", supemon->hp);
                cJSON_AddNumberToObject(supemon_json, "max_hp", supemon->max_hp);
                cJSON_AddNumberToObject(supemon_json, "attack", supemon->attack);
                cJSON_AddNumberToObject(supemon_json, "defense", supemon->defense);
                cJSON_AddNumberToObject(supemon_json, "evasion", supemon->evasion);
                cJSON_AddNumberToObject(supemon_json, "accuracy", supemon->accuracy);
                cJSON_AddNumberToObject(supemon_json, "speed", supemon->speed);
                free(supemon);
            }
        }
        
        cJSON_AddItemToArray(supemons_array, supemon_json);
    }
    cJSON_AddItemToObject(player_json, "supemons", supemons_array);

    // save selected supemon
    if (player->selected_supemon[0] != '\0') {
        cJSON_AddStringToObject(player_json, "selected_supemon", player->selected_supemon);
    }

    // save inventory items
    cJSON *items_array = cJSON_CreateArray();
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->items[i].quantity > 0) {
            cJSON *item_json = cJSON_CreateObject();
            cJSON_AddStringToObject(item_json, "name", player->items[i].name);
            cJSON_AddNumberToObject(item_json, "type", player->items[i].type);
            cJSON_AddNumberToObject(item_json, "price", player->items[i].price);
            cJSON_AddNumberToObject(item_json, "quantity", player->items[i].quantity);
            cJSON_AddStringToObject(item_json, "description", player->items[i].description);
            cJSON_AddNumberToObject(item_json, "effect_value", player->items[i].effect_value);
            cJSON_AddItemToArray(items_array, item_json);
        }
    }
    cJSON_AddItemToObject(player_json, "items", items_array);

    // update or add player data to save file
    if (cJSON_GetObjectItem(json, player->name)) {
        cJSON_ReplaceItemInObject(json, player->name, player_json);
    } else {
        cJSON_AddItemToObject(json, player->name, player_json);
    }

    // write updated save file
    file = fopen("save.json", "w");
    if (file) {
        char *json_string = cJSON_Print(json);
        fprintf(file, "%s", json_string);
        fclose(file);
        free(json_string);
    } else {
        printf("Error saving player data.\n");
    }

    if (old_json) {
        cJSON_Delete(old_json);
    }
    cJSON_Delete(json);
}

// loads player data from save file:
// - reads and parses save file
// - loads player info, supemons, and items
// - initializes player struct with loaded data
void load_player(Player *player, const char *name) {
    // open and read save file
    FILE *file = fopen("save.json", "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';

        // parse save data
        cJSON *json = cJSON_Parse(data);
        free(data);
        
        if (json) {
            cJSON *player_json = cJSON_GetObjectItem(json, name);
            if (player_json) {
                // load basic player info
                snprintf(player->name, sizeof(player->name), "%s", name);
                player->supcoins = cJSON_GetObjectItem(player_json, "supcoins")->valueint;

                // load supemons and their stats
                cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                if (supemons_array) {
                    int i = 0;
                    cJSON *supemon_json;
                    cJSON_ArrayForEach(supemon_json, supemons_array) {
                        if (i < MAX_SUPEMON) {
                            const char *supemon_name = cJSON_GetObjectItem(supemon_json, "name")->valuestring;
                            Supemon* supemon = create_supemon_by_name(supemon_name);
                            if (supemon) {
                                supemon->level = cJSON_GetObjectItem(supemon_json, "level")->valueint;
                                supemon->exp = cJSON_GetObjectItem(supemon_json, "exp")->valueint;
                                supemon->hp = cJSON_GetObjectItem(supemon_json, "hp")->valueint;
                                supemon->max_hp = cJSON_GetObjectItem(supemon_json, "max_hp")->valueint;
                                supemon->attack = cJSON_GetObjectItem(supemon_json, "attack")->valueint;
                                supemon->defense = cJSON_GetObjectItem(supemon_json, "defense")->valueint;
                                supemon->evasion = cJSON_GetObjectItem(supemon_json, "evasion")->valueint;
                                supemon->accuracy = cJSON_GetObjectItem(supemon_json, "accuracy")->valueint;
                                supemon->speed = cJSON_GetObjectItem(supemon_json, "speed")->valueint;
                                free(supemon);
                            }
                            strncpy(player->supemons[i], supemon_name, sizeof(player->supemons[i]) - 1);
                            player->supemons[i][sizeof(player->supemons[i]) - 1] = '\0';
                            i++;
                        }
                    }
                    for (; i < MAX_SUPEMON; i++) {
                        player->supemons[i][0] = '\0';
                    }
                }

                // load selected supemon
                cJSON *selected_supemon = cJSON_GetObjectItem(player_json, "selected_supemon");
                if (selected_supemon && selected_supemon->valuestring) {
                    strncpy(player->selected_supemon, selected_supemon->valuestring, sizeof(player->selected_supemon) - 1);
                    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
                }

                // load inventory items
                cJSON *items_array = cJSON_GetObjectItem(player_json, "items");
                if (items_array) {
                    int i = 0;
                    cJSON *item_json;
                    cJSON_ArrayForEach(item_json, items_array) {
                        if (i < MAX_ITEMS) {
                            strncpy(player->items[i].name, 
                                    cJSON_GetObjectItem(item_json, "name")->valuestring,
                                    sizeof(player->items[i].name) - 1);
                            player->items[i].type = cJSON_GetObjectItem(item_json, "type")->valueint;
                            player->items[i].price = cJSON_GetObjectItem(item_json, "price")->valueint;
                            player->items[i].quantity = cJSON_GetObjectItem(item_json, "quantity")->valueint;
                            strncpy(player->items[i].description,
                                    cJSON_GetObjectItem(item_json, "description")->valuestring,
                                    sizeof(player->items[i].description) - 1);
                            player->items[i].effect_value = cJSON_GetObjectItem(item_json, "effect_value")->valueint;
                            i++;
                        }
                    }
                    for (; i < MAX_ITEMS; i++) {
                        player->items[i].quantity = 0;
                    }
                }
            } else {
                printf("Player not found: %s\n", name);
            }
            cJSON_Delete(json);
        } else {
            printf("Error parsing JSON data.\n");
        }
    } else {
        printf("Error loading player data.\n");
    }
}

// quick save of current game state
void save_game_state(Player *player) {
    printf("Saving game...\n");
    save_player(player);
}

// updates specific supemon's stats in save file:
// - preserves experience and level progress
// - updates HP and other battle stats
void update_player_supemon_stats(Player* player, Supemon* supemon) {
    FILE *file = fopen("save.json", "r");
    cJSON *json = NULL;
    cJSON *old_json = NULL;

    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';
        old_json = cJSON_Parse(data);
        json = cJSON_Parse(data);
        free(data);
    }

    if (!json) {
        json = cJSON_CreateObject();
    }

    cJSON *player_json = cJSON_GetObjectItem(json, player->name);
    if (!player_json) {
        player_json = cJSON_CreateObject();
        cJSON_AddItemToObject(json, player->name, player_json);
    }

    cJSON_AddNumberToObject(player_json, "supcoins", player->supcoins);

    cJSON_DeleteItemFromObject(player_json, "supemons");
    cJSON *supemons_array = cJSON_CreateArray();
    cJSON_AddItemToObject(player_json, "supemons", supemons_array);

    cJSON *old_player_json = NULL;
    cJSON *old_supemons_array = NULL;
    if (old_json) {
        old_player_json = cJSON_GetObjectItem(old_json, player->name);
        if (old_player_json) {
            old_supemons_array = cJSON_GetObjectItem(old_player_json, "supemons");
        }
    }

    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        cJSON *supemon_json = cJSON_CreateObject();
        
        if (strcmp(player->supemons[i], supemon->name) == 0) {
            cJSON_AddStringToObject(supemon_json, "name", supemon->name);
            cJSON_AddNumberToObject(supemon_json, "level", supemon->level);
            cJSON_AddNumberToObject(supemon_json, "exp", supemon->exp);
            cJSON_AddNumberToObject(supemon_json, "hp", supemon->hp);
            cJSON_AddNumberToObject(supemon_json, "max_hp", supemon->max_hp);
            cJSON_AddNumberToObject(supemon_json, "attack", supemon->attack);
            cJSON_AddNumberToObject(supemon_json, "defense", supemon->defense);
            cJSON_AddNumberToObject(supemon_json, "evasion", supemon->evasion);
            cJSON_AddNumberToObject(supemon_json, "accuracy", supemon->accuracy);
            cJSON_AddNumberToObject(supemon_json, "speed", supemon->speed);
        } else {
            if (old_supemons_array) {
                cJSON *old_supemon;
                cJSON_ArrayForEach(old_supemon, old_supemons_array) {
                    if (strcmp(cJSON_GetObjectItem(old_supemon, "name")->valuestring, player->supemons[i]) == 0) {
                        cJSON_AddStringToObject(supemon_json, "name", cJSON_GetObjectItem(old_supemon, "name")->valuestring);
                        cJSON_AddNumberToObject(supemon_json, "level", cJSON_GetObjectItem(old_supemon, "level")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "exp", cJSON_GetObjectItem(old_supemon, "exp")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "hp", cJSON_GetObjectItem(old_supemon, "hp")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "max_hp", cJSON_GetObjectItem(old_supemon, "max_hp")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "attack", cJSON_GetObjectItem(old_supemon, "attack")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "defense", cJSON_GetObjectItem(old_supemon, "defense")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "evasion", cJSON_GetObjectItem(old_supemon, "evasion")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "accuracy", cJSON_GetObjectItem(old_supemon, "accuracy")->valueint);
                        cJSON_AddNumberToObject(supemon_json, "speed", cJSON_GetObjectItem(old_supemon, "speed")->valueint);
                        break;
                    }
                }
            }
            
            if (cJSON_GetObjectItem(supemon_json, "name") == NULL) {
                Supemon* new_supemon = create_supemon_by_name(player->supemons[i]);
                if (new_supemon) {
                    cJSON_AddStringToObject(supemon_json, "name", new_supemon->name);
                    cJSON_AddNumberToObject(supemon_json, "level", new_supemon->level);
                    cJSON_AddNumberToObject(supemon_json, "exp", new_supemon->exp);
                    cJSON_AddNumberToObject(supemon_json, "hp", new_supemon->hp);
                    cJSON_AddNumberToObject(supemon_json, "max_hp", new_supemon->max_hp);
                    cJSON_AddNumberToObject(supemon_json, "attack", new_supemon->attack);
                    cJSON_AddNumberToObject(supemon_json, "defense", new_supemon->defense);
                    cJSON_AddNumberToObject(supemon_json, "evasion", new_supemon->evasion);
                    cJSON_AddNumberToObject(supemon_json, "accuracy", new_supemon->accuracy);
                    cJSON_AddNumberToObject(supemon_json, "speed", new_supemon->speed);
                    free(new_supemon);
                }
            }
        }
        cJSON_AddItemToArray(supemons_array, supemon_json);
    }

    if (player->selected_supemon[0] != '\0') {
        cJSON_AddStringToObject(player_json, "selected_supemon", player->selected_supemon);
    }

    cJSON *items_array = cJSON_CreateArray();
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->items[i].quantity > 0) {
            cJSON *item_json = cJSON_CreateObject();
            cJSON_AddStringToObject(item_json, "name", player->items[i].name);
            cJSON_AddNumberToObject(item_json, "type", player->items[i].type);
            cJSON_AddNumberToObject(item_json, "price", player->items[i].price);
            cJSON_AddNumberToObject(item_json, "quantity", player->items[i].quantity);
            cJSON_AddStringToObject(item_json, "description", player->items[i].description);
            cJSON_AddNumberToObject(item_json, "effect_value", player->items[i].effect_value);
            cJSON_AddItemToArray(items_array, item_json);
        }
    }
    cJSON_AddItemToObject(player_json, "items", items_array);

    file = fopen("save.json", "w");
    if (file) {
        char *json_string = cJSON_Print(json);
        fprintf(file, "%s", json_string);
        fclose(file);
        free(json_string);
    }

    if (old_json) {
        cJSON_Delete(old_json);
    }
    cJSON_Delete(json);
}