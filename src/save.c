#include "save.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "cJSON.h"
#include "supemon.h" 

void save_player(const Player *player) {
    FILE *file = fopen("save.json", "r");
    cJSON *json = NULL;

    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';
        json = cJSON_Parse(data);
        free(data);
    }

    if (!json) {
        json = cJSON_CreateObject();
    }

    cJSON *player_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(player_json, "supcoins", player->supcoins);

    // Create array for Supemons
    cJSON *supemons_array = cJSON_CreateArray();
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        Supemon* supemon = create_supemon_by_name(player->supemons[i]);
        if (supemon) {
            cJSON *supemon_json = cJSON_CreateObject();
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
            
            cJSON_AddItemToArray(supemons_array, supemon_json);
            free(supemon);
        }
    }
    cJSON_AddItemToObject(player_json, "supemons", supemons_array);

    // Add selected Supemon
    if (player->selected_supemon[0] != '\0') {
        cJSON_AddStringToObject(player_json, "selected_supemon", player->selected_supemon);
    }

    // Add items to player_json
    cJSON *items_array = cJSON_CreateArray();
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->items[i].quantity > 0) {
            cJSON *item_json = cJSON_CreateObject();
            cJSON_AddStringToObject(item_json, "name", player->items[i].name);
            cJSON_AddNumberToObject(item_json, "price", player->items[i].price);
            cJSON_AddNumberToObject(item_json, "quantity", player->items[i].quantity);
            cJSON_AddStringToObject(item_json, "description", player->items[i].description);
            cJSON_AddItemToArray(items_array, item_json);
        }
    }
    cJSON_AddItemToObject(player_json, "items", items_array);

    // Update or add player data in JSON
    if (cJSON_GetObjectItem(json, player->name)) {
        cJSON_ReplaceItemInObject(json, player->name, player_json);
    } else {
        cJSON_AddItemToObject(json, player->name, player_json);
    }

    // Write JSON back to file
    file = fopen("save.json", "w");
    if (file) {
        char *json_string = cJSON_Print(json);
        fprintf(file, "%s", json_string);
        fclose(file);
        free(json_string);
    } else {
        printf("Error saving player data.\n");
    }

    cJSON_Delete(json);
}

void load_player(Player *player, const char *name) {
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
            cJSON *player_json = cJSON_GetObjectItem(json, name);
            if (player_json) {
                snprintf(player->name, sizeof(player->name), "%s", name);
                player->supcoins = cJSON_GetObjectItem(player_json, "supcoins")->valueint;

                // Load Supemons
                cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                if (supemons_array) {
                    int i = 0;
                    cJSON *supemon_json;
                    cJSON_ArrayForEach(supemon_json, supemons_array) {
                        if (i < MAX_SUPEMON) {
                            const char *supemon_name = cJSON_GetObjectItem(supemon_json, "name")->valuestring;
                            strncpy(player->supemons[i], supemon_name, sizeof(player->supemons[i]) - 1);
                            player->supemons[i][sizeof(player->supemons[i]) - 1] = '\0';
                            i++;
                        }
                    }
                    // Clear remaining slots
                    for (; i < MAX_SUPEMON; i++) {
                        player->supemons[i][0] = '\0';
                    }
                }

                // Load selected Supemon
                cJSON *selected_supemon = cJSON_GetObjectItem(player_json, "selected_supemon");
                if (selected_supemon && selected_supemon->valuestring) {
                    strncpy(player->selected_supemon, selected_supemon->valuestring, sizeof(player->selected_supemon) - 1);
                    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
                }

                // Load items
                cJSON *items_array = cJSON_GetObjectItem(player_json, "items");
                if (items_array) {
                    int i = 0;
                    cJSON *item_json;
                    cJSON_ArrayForEach(item_json, items_array) {
                        if (i < MAX_ITEMS) {
                            const char *item_name = cJSON_GetObjectItem(item_json, "name")->valuestring;
                            strncpy(player->items[i].name, item_name, sizeof(player->items[i].name) - 1);
                            player->items[i].name[sizeof(player->items[i].name) - 1] = '\0';
                            player->items[i].price = cJSON_GetObjectItem(item_json, "price")->valueint;
                            player->items[i].quantity = cJSON_GetObjectItem(item_json, "quantity")->valueint;
                            strncpy(player->items[i].description, cJSON_GetObjectItem(item_json, "description")->valuestring, sizeof(player->items[i].description) - 1);
                            player->items[i].description[sizeof(player->items[i].description) - 1] = '\0';
                            i++;
                        }
                    }
                    // Clear remaining slots
                    for (; i < MAX_ITEMS; i++) {
                        player->items[i].quantity = 0; // Reset quantity
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

void save_game_state(Player *player) {
    printf("Saving game...\n");
    save_player(player);
}