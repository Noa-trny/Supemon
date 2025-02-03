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
    memset(player->name, 0, sizeof(player->name)); // clear name
    player->supcoins = 0; // reset supcoins
    memset(player->supemons, 0, sizeof(player->supemons)); // clear Supemons
    memset(player->selected_supemon, 0, sizeof(player->selected_supemon)); // clear selected Supemon
}

void choose_save(Player *player) {
    FILE *file = fopen("save.json", "r"); // open save file
    if (file) { // if file opened
        cJSON *json; // JSON object
        fseek(file, 0, SEEK_END); // go to end
        long length = ftell(file); // get length
        fseek(file, 0, SEEK_SET); // go back to start
        
        char *data = malloc(length + 1); // allocate memory
        fread(data, 1, length, file); // read data
        fclose(file); // close file
        data[length] = '\0'; // null-terminate

        json = cJSON_Parse(data); // parse JSON
        free(data); 
        
        if (json) { // if valid JSON
            printf("Available saves:\n");
            cJSON *player_json; // player JSON object
            cJSON_ArrayForEach(player_json, json) { // iterate saves
                printf("- %s\n", player_json->string); // print save names
            }

            char choice[50]; // choice buffer
            printf("Enter the name of the save to load (or type 'new' to create a new game): ");
            scanf("%49s", choice); // read input
            if (strcmp(choice, "new") == 0) { // check for new game
                printf("Creating a new game...\n");
                initialize_player(player); // initialize player
            } else {
                load_player(player, choice); // load player data
                printf("Loaded player: %s with %d Supcoins.\n", player->name, player->supcoins);
                cJSON_Delete(json); // free JSON
                return; // exit
            }
            cJSON_Delete(json); // free JSON
        }
    } else {
        printf("No saves found. Creating a new game...\n"); // no saves found
        initialize_player(player); // initialize player
    }
}

void add_starter_supemon(Player *player, int choice) {
    const char* starter_name; // starter name
    switch(choice) { // determine starter
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
            starter_name = "Supmander"; // this is default choice in case choice is wrong and not 1,2,3
            break;
    }
    
    strncpy(player->supemons[0], starter_name, sizeof(player->supemons[0]) - 1); // copy name
    player->supemons[0][sizeof(player->supemons[0]) - 1] = '\0'; // null-terminate
    strncpy(player->selected_supemon, starter_name, sizeof(player->selected_supemon) - 1); // copy selected name
    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0'; // null-terminate
}

int main(void) {
    Player player; // player variable
    char player_pseudo[50]; // buffer for name
    int supemon_choice; // Supemon choice

    choose_save(&player); // choose save or new game

    if (strlen(player.name) == 0) { // if name is empty
        printf("Enter your name: "); 
        scanf("%49s", player_pseudo); // read name
        strncpy(player.name, player_pseudo, sizeof(player.name) - 1); // copy name
        player.name[sizeof(player.name) - 1] = '\0';

        printf("Hello %s!\nWelcome in Supémon World!\n", player.name); 

        printf("+-----------------------------+\n"); 
        printf("| Choose your starter Supémon:|\n"); 
        printf("| 1 - Supmander               |\n"); 
        printf("| 2 - Supasaur                |\n");
        printf("| 3 - Supirtle                |\n"); 
        printf("+-----------------------------+\n"); 
        printf("1, 2 or 3: "); 
        scanf("%d", &supemon_choice); // player supemon starter choice
        add_starter_supemon(&player, supemon_choice); // add starter
    } else {
        printf("Welcome back, %s!\n", player.name); 
        printf("\nYour Supemons:\n");
        // print Supemons of the player (from save)
        for (int i = 0; i < MAX_SUPEMON && player.supemons[i][0] != '\0'; i++) {
            printf("%d - %s\n", i + 1, player.supemons[i]);
        }
    }

    printf("Saving player data...\n"); // saving message
    save_player(&player); // save data

    return 0; // exit
}