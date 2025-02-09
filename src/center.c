#include "center.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "player.h"
#include "supemon.h"
#include "save.h"

void heal_supemons(Player *player) {
    printf("\nWelcome to the Supemon Center!\n");
    printf("Your Supemons:\n");
    
    int has_supemons = 0;
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
    
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) {
        Supemon* supemon = create_supemon_by_name(player->supemons[i]);
        if (supemon) {
            has_supemons = 1;
            
            if (json) {
                cJSON *player_json = cJSON_GetObjectItem(json, player->name);
                if (player_json) {
                    cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons");
                    if (supemons_array) {
                        cJSON *supemon_json;
                        cJSON_ArrayForEach(supemon_json, supemons_array) {
                            if (strcmp(cJSON_GetObjectItem(supemon_json, "name")->valuestring, supemon->name) == 0) {
                                supemon->level = cJSON_GetObjectItem(supemon_json, "level")->valueint;
                                supemon->exp = cJSON_GetObjectItem(supemon_json, "exp")->valueint;
                                supemon->max_hp = cJSON_GetObjectItem(supemon_json, "max_hp")->valueint;
                                supemon->attack = cJSON_GetObjectItem(supemon_json, "attack")->valueint;
                                supemon->defense = cJSON_GetObjectItem(supemon_json, "defense")->valueint;
                                supemon->evasion = cJSON_GetObjectItem(supemon_json, "evasion")->valueint;
                                supemon->accuracy = cJSON_GetObjectItem(supemon_json, "accuracy")->valueint;
                                supemon->speed = cJSON_GetObjectItem(supemon_json, "speed")->valueint;
                                break;
                            }
                        }
                    }
                }
            }
            
            printf("%d - %s (HP: %d/%d)\n", i + 1, supemon->name, supemon->hp, supemon->max_hp);
            supemon->hp = supemon->max_hp;
            update_player_supemon_stats(player, supemon);
            free(supemon);
        }
    }
    
    if (json) {
        cJSON_Delete(json);
    }
    
    if (!has_supemons) {
        printf("You don't have any Supemons!\n");
        return;
    }
    
    printf("\nAll your Supemons have been healed to full health!\n");
    printf("We hope to see you again!\n");
} 