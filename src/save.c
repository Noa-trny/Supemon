#include "save.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "cJSON.h"
#include "supemon.h" 

// main save function that handles all player data persistence:
// - manages save file in JSON format for easy data structure
// - preserves all player progress and stats
// - handles both new saves and updates to existing saves
void save_player(const Player *player) {
    // create root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        printf("Error creating JSON object!\n");
        return;
    }

    // attempt to read existing save file for data preservation
    FILE *file = fopen("save.json", "r");
    cJSON *json = NULL;      // will contain new save data
    cJSON *old_json = NULL;  // keeps existing data for reference
    
    // if save file exists, read and parse its content
    if (file) {
        // get file size for memory allocation
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        // read entire file into memory
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';
        
        // parse file content into JSON objects
        json = cJSON_Parse(data);
        old_json = cJSON_Parse(data);  // keep a second copy for reference
        free(data);
    }

    // create new JSON object if no save file exists
    if (!json) {
        json = cJSON_CreateObject();
    }

    // create player data structure
    cJSON *player_json = cJSON_CreateObject();
    // save basic player info (supcoins)
    cJSON_AddNumberToObject(player_json, "supcoins", player->supcoins);

    // prepare array for supemon data
    cJSON *supemons_array = cJSON_CreateArray();
    
    // try to preserve existing supemon stats from old save
    cJSON *old_player_json = NULL;
    cJSON *old_supemons_array = NULL;
    if (old_json) {
        old_player_json = cJSON_GetObjectItem(old_json, player->name);
        if (old_player_json) {
            old_supemons_array = cJSON_GetObjectItem(old_player_json, "supemons");
        }
    }

    // save each supemon's complete data
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        cJSON *supemon_json = cJSON_CreateObject();
        
        // try to preserve existing stats from old save
        if (old_supemons_array) {
            cJSON *old_supemon;
            cJSON_ArrayForEach(old_supemon, old_supemons_array) {
                // if supemon found in old save, keep its stats
                if (strcmp(cJSON_GetObjectItem(old_supemon, "name")->valuestring, player->supemons[i]) == 0) {
                    // copy all stats: level, exp, hp, and battle stats
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
        
        // if supemon not found in old save, create new entry with default stats
        if (cJSON_GetObjectItem(supemon_json, "name") == NULL) {
            Supemon* supemon = create_supemon_by_name(player->supemons[i]);
            if (supemon) {
                // save all supemon stats
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
        
        // add supemon to array
        cJSON_AddItemToArray(supemons_array, supemon_json);
    }
    // add all supemons to player data
    cJSON_AddItemToObject(player_json, "supemons", supemons_array);

    // save currently selected supemon
    if (player->selected_supemon[0] != '\0') {
        cJSON_AddStringToObject(player_json, "selected_supemon", player->selected_supemon);
    }

    // save inventory items with all properties
    cJSON *items_array = cJSON_CreateArray();
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->items[i].quantity > 0) {
            cJSON *item_json = cJSON_CreateObject();
            // save complete item data: name, type, price, quantity, etc.
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

    // update existing player data or add new player
    if (cJSON_GetObjectItem(json, player->name)) {
        cJSON_ReplaceItemInObject(json, player->name, player_json);
    } else {
        cJSON_AddItemToObject(json, player->name, player_json);
    }

    // write complete save data to file
    file = fopen("save.json", "w");
    if (file) {
        char *json_string = cJSON_Print(json);
        fprintf(file, "%s", json_string);
        fclose(file);
        free(json_string);
    } else {
        printf("Error saving player data.\n");
    }

    // cleanup JSON objects
    if (old_json) {
        cJSON_Delete(old_json);
    }
    cJSON_Delete(json);
}

// loads complete player data from save file:
// - restores all player progress and stats
// - handles missing or corrupted save data
// - initializes new player if save not found
void load_player(Player *player, const char *name) {
    // attempt to open save file
    FILE *file = fopen("save.json", "r");
    if (file) {
        // read entire save file
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
            // find player's save data
            cJSON *player_json = cJSON_GetObjectItem(json, name);
            if (player_json) {
                // restore basic player info
                snprintf(player->name, sizeof(player->name), "%s", name);
                player->supcoins = cJSON_GetObjectItem(player_json, "supcoins")->valueint;

                // restore all supemons and their stats
                cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                if (supemons_array) {
                    int i = 0;
                    cJSON *supemon_json;
                    cJSON_ArrayForEach(supemon_json, supemons_array) {
                        if (i < MAX_SUPEMON) {
                            // load each supemon's complete data
                            const char *supemon_name = cJSON_GetObjectItem(supemon_json, "name")->valuestring;
                            Supemon* supemon = create_supemon_by_name(supemon_name);
                            if (supemon) {
                                // restore all stats
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
                            // add to player's team
                            strncpy(player->supemons[i], supemon_name, sizeof(player->supemons[i]) - 1);
                            player->supemons[i][sizeof(player->supemons[i]) - 1] = '\0';
                            i++;
                        }
                    }
                    // clear remaining supemon slots
                    for (; i < MAX_SUPEMON; i++) {
                        player->supemons[i][0] = '\0';
                    }
                }

                // restore selected supemon
                cJSON *selected_supemon = cJSON_GetObjectItem(player_json, "selected_supemon");
                if (selected_supemon && selected_supemon->valuestring) {
                    strncpy(player->selected_supemon, selected_supemon->valuestring, sizeof(player->selected_supemon) - 1);
                    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
                }

                // restore inventory items
                cJSON *items_array = cJSON_GetObjectItem(player_json, "items");
                if (items_array) {
                    int i = 0;
                    cJSON *item_json;
                    cJSON_ArrayForEach(item_json, items_array) {
                        if (i < MAX_ITEMS) {
                            // restore complete item data
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
                    // clear remaining item slots
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

// updates a specific supemon's stats in the save file:
// - called after battles, healing, or level ups
// - preserves experience and level progress
// - updates current stats (HP, attack, defense, etc.)
// - maintains consistency between memory and save file
void update_player_supemon_stats(Player* player, Supemon* supemon) {
    // read existing save file
    FILE *file = fopen("save.json", "r");
    cJSON *json = NULL;
    cJSON *old_json = NULL;

    if (file) {
        // load and parse save data
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

    // create new save if none exists
    if (!json) {
        json = cJSON_CreateObject();
    }

    // get or create player data
    cJSON *player_json = cJSON_GetObjectItem(json, player->name);
    if (!player_json) {
        player_json = cJSON_CreateObject();
        cJSON_AddItemToObject(json, player->name, player_json);
    }

    // update basic player info
    cJSON_AddNumberToObject(player_json, "supcoins", player->supcoins);

    // prepare for supemon data update
    cJSON_DeleteItemFromObject(player_json, "supemons");
    cJSON *supemons_array = cJSON_CreateArray();
    cJSON_AddItemToObject(player_json, "supemons", supemons_array);

    // get existing supemon data
    cJSON *old_player_json = NULL;
    cJSON *old_supemons_array = NULL;
    if (old_json) {
        old_player_json = cJSON_GetObjectItem(old_json, player->name);
        if (old_player_json) {
            old_supemons_array = cJSON_GetObjectItem(old_player_json, "supemons");
        }
    }

    // update each supemon's data
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        cJSON *supemon_json = cJSON_CreateObject();
        
        // update stats for the specified supemon
        if (strcmp(player->supemons[i], supemon->name) == 0) {
            // save all current stats
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
            // preserve existing stats for other supemons
            if (old_supemons_array) {
                cJSON *old_supemon;
                cJSON_ArrayForEach(old_supemon, old_supemons_array) {
                    if (strcmp(cJSON_GetObjectItem(old_supemon, "name")->valuestring, player->supemons[i]) == 0) {
                        // copy all stats from old save
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
            
            // create new entry if supemon not found in old save
            if (cJSON_GetObjectItem(supemon_json, "name") == NULL) {
                Supemon* new_supemon = create_supemon_by_name(player->supemons[i]);
                if (new_supemon) {
                    // save default stats
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

    // update selected supemon
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

    // write updated save file
    file = fopen("save.json", "w");
    if (file) {
        char *json_string = cJSON_Print(json);
        fprintf(file, "%s", json_string);
        fclose(file);
        free(json_string);
    }

    // cleanup JSON objects
    if (old_json) {
        cJSON_Delete(old_json);
    }
    cJSON_Delete(json);
}