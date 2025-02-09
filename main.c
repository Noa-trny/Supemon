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

// initializes player data with default values
void initialize_player(Player *player) {
    // clear player name and set initial supcoins to 0
    memset(player->name, 0, sizeof(player->name));
    player->supcoins = 0;
    // clear supemon arrays (owned and selected)
    memset(player->supemons, 0, sizeof(player->supemons));
    memset(player->selected_supemon, 0, sizeof(player->selected_supemon));
    // initialize items array
    initialize_player_items(player);
}

// handles save game selection or creates new game
void choose_save(Player *player) {
    FILE *file = fopen("save.json", "r"); 
    if (file) { 
        // read and parse save file content
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
            // display available save files and handle user choice
            printf("Available saves:\n");
            cJSON *player_json;
            cJSON_ArrayForEach(player_json, json) {
                printf("- %s\n", player_json->string);
            }

            char choice[50];
            printf("Enter the name of the save to load (or type 'new' to create a new game): ");
            scanf("%49s", choice);
            // handle new game or load existing save
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

// adds the chosen starter supemon to player's collection
void add_starter_supemon(Player *player, int choice) {
    // select starter supemon based on player choice
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
    
    // safely copy starter name to player's first supemon slot and selected supemon
    strncpy(player->supemons[0], starter_name, sizeof(player->supemons[0]) - 1);
    player->supemons[0][sizeof(player->supemons[0]) - 1] = '\0';
    strncpy(player->selected_supemon, starter_name, sizeof(player->selected_supemon) - 1);
    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0';
}

// main game menu interface with different location options
void display_menu(Player *player) {
    int choice;
    // display main game locations menu
    printf("+-----------------------------+\n");
    printf("| Where do you want to go?    |\n");
    printf("| 1 - Into the Wild           |\n");
    printf("| 2 - In the shop             |\n");
    printf("| 3 - In the Supemon Center   |\n");
    printf("| 4 - Leave the Game          |\n");
    printf("+-----------------------------+\n");
    printf("1, 2, 3 or 4: ");
    scanf("%d", &choice);

    // handle player location choice
    switch (choice) {
        case 1:
            printf("Going Into the Wild...\n");
            start_battle(player);  // initiate random wild supemon encounter
            break;
        case 2:
            printf("Entering the shop...\n");
            display_shop(player);  // show available items for purchase
            break;
        case 3:
            printf("Entering the Supemon Center...\n");
            heal_supemons(player); // restore supemon health
            break;
        case 4:
            printf("Leaving the game...\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    // save progress after each action and show menu again
    save_player(player);
    display_menu(player);
}

int main(void) {
    // main game variables
    Player player;
    char player_pseudo[50];
    int supemon_choice;

    // initialize game systems and load/create save
    initialize_player(&player);
    initialize_shop_items();
    choose_save(&player);

    // handle new player setup
    if (strlen(player.name) == 0) {
        // get player name and starter supemon choice
        printf("Enter your name: "); 
        scanf("%49s", player_pseudo);
        strncpy(player.name, player_pseudo, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';

        printf("Hello %s!\nWelcome in Supemon World!\n", player.name); 

        // display starter supemon selection menu
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
        // display returning player's supemon collection
        printf("Welcome back, %s!\n", player.name); 
        printf("\nYour Supemons:\n");
        for (int i = 0; i < MAX_SUPEMON && player.supemons[i][0] != '\0'; i++) {
            printf("%d - %s\n", i + 1, player.supemons[i]);
        }
    }

    // start main game loop
    display_menu(&player);
    printf("Saving player data...\n");
    save_player(&player);

    return 0;
}