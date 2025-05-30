#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// displays battle action menu with options:
// - move: use supemon abilities
// - change: switch active supemon
// - item: use items from inventory
// - capture: attempt to catch wild supemon
// - run: try to escape battle
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

// displays current battle status:
// - shows enemy supemon stats (hp, level, attack, defense, accuracy, evasion)
// - shows player supemon stats
// - formats output in a clear, readable way
void display_battle_status(Battle *battle) {
    printf("\nYour turn...\n\n");
    // display enemy supemon stats
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
    
    // display player supemon stats
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

// displays HP status for all player's supemons:
// - loads current HP values from save file
// - shows current/max HP for each supemon
// - indicates which supemon is currently selected
void display_all_supemons_hp(const Player* player) {
    printf("\nSupemons HP Status:\n");
    
    // load save file to get current HP values
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
        
        // display HP for each supemon
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
} 