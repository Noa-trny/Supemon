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
    
    cJSON *supemons_array = cJSON_CreateArray();
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        cJSON_AddItemToArray(supemons_array, cJSON_CreateString(player->supemons[i]));
    }
    cJSON_AddItemToObject(player_json, "supemons", supemons_array);
    
    if (player->selected_supemon[0] != '\0') {
        cJSON_AddStringToObject(player_json, "selected_supemon", player->selected_supemon);
    }
    
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
        if (data == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }
        
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';

        cJSON *json = cJSON_Parse(data);
        free(data);
        
        if (json == NULL) {
            printf("Error parsing JSON data: %s\n", cJSON_GetErrorPtr());
            return;
        }
        
        if (json) {
            cJSON *player_json = cJSON_GetObjectItem(json, name);
            if (player_json) {
                snprintf(player->name, sizeof(player->name), "%s", name);
                player->supcoins = cJSON_GetObjectItem(player_json, "supcoins")->valueint;
                
                cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                if (supemons_array) {
                    int i = 0;
                    cJSON *supemon;
                    cJSON_ArrayForEach(supemon, supemons_array) {
                        if (i < MAX_SUPEMON) {
                            const char *supemon_name = supemon->valuestring;
                            int valid = 1;
                            for (size_t j = 0; supemon_name[j] != '\0'; j++) {
                                if ((unsigned char)supemon_name[j] < 32 || (unsigned char)supemon_name[j] > 126) {
                                    valid = 0;
                                    break;
                            }
                            
                            if (valid) {
                                strncpy(player->supemons[i], supemon_name, sizeof(player->supemons[i]) - 1);
                                player->supemons[i][sizeof(player->supemons[i]) - 1] = '\0';
                                i++;
                            }
                        }
                    }
                    for (; i < MAX_SUPEMON; i++) {
                        player->supemons[i][0] = '\0';
                    }
                }
                
                cJSON *selected_supemon = cJSON_GetObjectItem(player_json, "selected_supemon");
                if (selected_supemon && selected_supemon->valuestring) {
                    const char *selected_name = selected_supemon->valuestring;
                    int valid = 1;
                    for (size_t j = 0; selected_name[j] != '\0'; j++) {
                        if ((unsigned char)selected_name[j] < 32 || (unsigned char)selected_name[j] > 126) {
                            valid = 0;
                            break;
                        }
                    }
                    
                    if (valid) {
                        strncpy(player->selected_supemon, selected_name, sizeof(player->selected_supemon) - 1);
                        player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
                    } else {
                        player->selected_supemon[0] = '\0';
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