#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "player.h"
#include "supemon.h"

void display_all_supemons_hp(const Player* player) {
    printf("\nSupemons HP Status:\n");
    printf("--------------------------------\n");
    
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
                        const char *name = cJSON_GetObjectItem(supemon_json, "name")->valuestring;
                        int hp = cJSON_GetObjectItem(supemon_json, "hp")->valueint;
                        int max_hp = cJSON_GetObjectItem(supemon_json, "max_hp")->valueint;
                        printf("%s: %d/%d HP", name, hp, max_hp);
                        if (strcmp(name, player->selected_supemon) == 0) {
                            printf(" (Selected)");
                        }
                        printf("\n");
                    }
                }
            }
            cJSON_Delete(json);
        }
    }
    printf("--------------------------------\n");
}