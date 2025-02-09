#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player.h"
#include "battle.h"
#include "save.h"
#include <cJSON.h>
#include "shop.h"
#include "center.h"
#define MAX_ITEMS 10

void initialize_player(Player *player);
void choose_save(Player *player);
void add_starter_supemon(Player *player, int choice);
void display_menu(Player *player);

void initialize_player(Player *player) {
    memset(player->name, 0, sizeof(player->name));
    player->supcoins = 0;
    memset(player->supemons, 0, sizeof(player->supemons));
    memset(player->selected_supemon, 0, sizeof(player->selected_supemon));
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
            starter_name = "Supmander";
            break;
    }
    
    strncpy(player->supemons[0], starter_name, sizeof(player->supemons[0]) - 1);
    player->supemons[0][sizeof(player->supemons[0]) - 1] = '\0';
    strncpy(player->selected_supemon, starter_name, sizeof(player->selected_supemon) - 1);
    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
}

void display_menu(Player *player) {
    int choice;
    printf("+-----------------------------+\n");
    printf("| Where do you want to go?    |\n");
    printf("| 1 - Into the Wild           |\n");
    printf("| 2 - In the shop             |\n");
    printf("| 3 - In the Supemon Center   |\n");
    printf("| 4 - Leave the Game          |\n");
    printf("+-----------------------------+\n");
    printf("1, 2, 3 or 4: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Going Into the Wild...\n");
            start_battle(player);
            break;
        case 2:
            printf("Entering the shop...\n");
            display_shop(player);
            break;
        case 3:
            printf("Entering the Supemon Center...\n");
            heal_supemons(player);
            break;
        case 4:
            printf("Leaving the game...\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    save_game_state(player);
    display_menu(player);
}

int main(void) {
    Player player;
    char player_pseudo[50];
    int supemon_choice;

    initialize_player(&player);
    initialize_shop_items();
    choose_save(&player);

    if (strlen(player.name) == 0) {
        printf("Enter your name: "); 
        scanf("%49s", player_pseudo);
        strncpy(player.name, player_pseudo, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';

        printf("Hello %s!\nWelcome in Supemon World!\n", player.name); 

        printf("+-----------------------------+\n"); 
        printf("| Choose your starter Supemon:|\n"); 
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
            printf("%d - %s\n", i + 1, player.supemons[i]);
        }
    }

    display_menu(&player);
    printf("Saving player data...\n");
    save_player(&player);

    return 0;
}