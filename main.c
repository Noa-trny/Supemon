#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player.h"
//#include "supemon.h"
//#include "move.h"
//#include "battle.h"
//#include "shop.h"
#include "save.h"
//#include "utils.h"
//#include "item.h"
#include <dirent.h>
#include <cJSON.h>

void choose_save(Player *player) {
    FILE *file = fopen("save.json", "r");
    if (file) {
        cJSON *json;
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        fclose(file);
        data[length] = '\0';

        json = cJSON_Parse(data);
        free(data);
        
        if (json) {
            printf("Available saves:\n");
            cJSON *player_json;
            cJSON_ArrayForEach(player_json, json) {
                printf("- %s\n", player_json->string); 
            }

            char choice[50];
            printf("Enter the name of the save to load (or type 'new' to create a new game): ");
            scanf("%s", choice);
            if (strcmp(choice, "new") == 0) {
                printf("Creating a new game...\n");
            } else {
                load_player(player, choice);
                printf("Loaded player: %s with %d Supcoins.\n", player->name, player->supcoins);
                cJSON_Delete(json);
                return;
            }
            cJSON_Delete(json);
        }
    } else {
        printf("No saves found. Creating a new game...\n");
    }
}

int main(void) {
    Player player; 
    char player_pseudo[50];
    int supemon_choice;

    choose_save(&player);

    if (strlen(player.name) == 0) {
        printf("Enter your name: ");
        scanf("%s", player_pseudo);
        snprintf(player.name, sizeof(player.name), "%s", player_pseudo); 

        printf("Hello %s!\nWelcome in Supémon World!\n", player.name);

        printf("+-----------------------------+\n");
        printf("| Choose your starter Supémon:|\n");
        printf("| 1 - Supmander               |\n");
        printf("| 2 - Supasaur                |\n");
        printf("| 3 - Supirtle                |\n");
        printf("+-----------------------------+\n");
        printf("1, 2 or 3: ");
        scanf("%d", &supemon_choice);
    } else {
        printf("Welcome back, %s!\n", player.name);
    }

    printf("Saving player data...\n");
    save_player(&player);

    return 0;
}
