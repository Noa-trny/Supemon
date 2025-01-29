#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

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
    
    if (cJSON_GetObjectItem(json, player->name)) {
        cJSON_ReplaceItemInObject(json, player->name, player_json);
    } else {
        cJSON_AddItemToObject(json, player->name, player_json);
    }

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