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

void initialize_player(Player *player) {
    memset(player->name, 0, sizeof(player->name));
    player->supcoins = 0;
    memset(player->supemons, 0, sizeof(player->supemons));
    memset(player->selected_supemon, 0, sizeof(player->selected_supemon));
}

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
            scanf("%49s", choice);
            if (strcmp(choice, "new") == 0) {
                printf("Creating a new game...\n");
                initialize_player(player);
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
        initialize_player(player);
    }
}

void add_starter_supemon(Player *player, int choice) {
    const char* starter_name;
    switch(choice) {
        case 1:
            starter_name = "Supmander";
            break;
        case 2:
            starter_name = "Supasaur";
            break;
        case 3:
            starter_name = "Supirtle";
            break;
        default:
            starter_name = "Supmander"; // invalid choice so by default supmander
            break;
    }
    
    strncpy(player->supemons[0], starter_name, sizeof(player->supemons[0]) - 1);
    player->supemons[0][sizeof(player->supemons[0]) - 1] = '\0';
    
    strncpy(player->selected_supemon, starter_name, sizeof(player->selected_supemon) - 1);
    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
}

int main(void) {
    Player player; 
    char player_pseudo[50];
    int supemon_choice;

    choose_save(&player);

    if (strlen(player.name) == 0) {
        printf("Enter your name: ");
        scanf("%49s", player_pseudo); 
        strncpy(player.name, player_pseudo, sizeof(player.name) - 1); 
        player.name[sizeof(player.name) - 1] = '\0'; 

        printf("Hello %s!\nWelcome in Supémon World!\n", player.name);

        printf("+-----------------------------+\n");
        printf("| Choose your starter Supémon:|\n");
        printf("| 1 - Supmander               |\n");
        printf("| 2 - Supasaur                |\n");
        printf("| 3 - Supirtle                |\n");
        printf("+-----------------------------+\n");
        printf("1, 2 or 3: ");
        scanf("%d", &supemon_choice);
        add_starter_supemon(&player, supemon_choice);
    } else {
        printf("Welcome back, %s!\n", player.name);
        printf("\nYour Supemons:\n");
        for (int i = 0; i < MAX_SUPEMON && player.supemons[i][0] != '\0'; i++) {
            int valid = 1;
            for (size_t j = 0; player.supemons[i][j] != '\0'; j++) {
                if ((unsigned char)player.supemons[i][j] < 32 || (unsigned char)player.supemons[i][j] > 126) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                printf("%d - %s\n", i + 1, player.supemons[i]);
            }
        }
    }

    printf("Saving player data...\n");
    save_player(&player);

    return 0;
}
