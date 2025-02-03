#include "save.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "cJSON.h" 

void save_player(const Player *player) {
    FILE *file = fopen("save.json", "r"); // open save file
    cJSON *json = NULL; // JSON object

    if (file) { // if file opened
        fseek(file, 0, SEEK_END); // go to end
        long length = ftell(file); // get length
        fseek(file, 0, SEEK_SET); // go back to start
        
        char *data = malloc(length + 1); // allocate memory
        fread(data, 1, length, file); // read data
        fclose(file); // close file
        data[length] = '\0'; 
        json = cJSON_Parse(data); // parse JSON
        free(data); // free memory
    }

    if (!json) { // if parsing failed
        json = cJSON_CreateObject(); // create new JSON object
    }

    cJSON *player_json = cJSON_CreateObject(); // player JSON object
    cJSON_AddNumberToObject(player_json, "supcoins", player->supcoins); // add supcoins

    cJSON *supemons_array = cJSON_CreateArray(); // Supemons array
    for (int i = 0; i < MAX_SUPEMON && player->supemons[i][0] != '\0'; i++) { // loop through Supemons
        cJSON_AddItemToArray(supemons_array, cJSON_CreateString(player->supemons[i])); // add Supemon
    }
    cJSON_AddItemToObject(player_json, "supemons", supemons_array); // add array to player JSON

    if (player->selected_supemon[0] != '\0') { // if selected Supemon exists
        cJSON_AddStringToObject(player_json, "selected_supemon", player->selected_supemon); // add selected Supemon
    }

    if (cJSON_GetObjectItem(json, player->name)) { // if player exists in JSON
        cJSON_ReplaceItemInObject(json, player->name, player_json); // replace data
    } else {
        cJSON_AddItemToObject(json, player->name, player_json); // add new data
    }

    file = fopen("save.json", "w"); // open save file for writing
    if (file) { // if file opened
        char *json_string = cJSON_Print(json); // convert JSON to string
        fprintf(file, "%s", json_string); // write to file
        fclose(file); // close file
        free(json_string); // free string
    } else {
        printf("Error saving player data.\n"); // error message
    }

    cJSON_Delete(json); // free JSON
}

void load_player(Player *player, const char *name) {
    FILE *file = fopen("save.json", "r"); // open save file
    if (file) { // if file opened
        fseek(file, 0, SEEK_END); // go to end
        long length = ftell(file); // get length
        fseek(file, 0, SEEK_SET); // go back to start
        
        char *data = malloc(length + 1); // allocate memory
        if (data == NULL) { // check allocation
            printf("Memory allocation failed.\n"); // error message
            fclose(file); // close file
            return; // exit
        }
        
        fread(data, 1, length, file); // read data
        fclose(file); // close file
        data[length] = '\0'; // null-terminate

        cJSON *json = cJSON_Parse(data); // parse JSON
        free(data); // free memory
        
        if (json == NULL) { // if parsing failed
            printf("Error parsing JSON data: %s\n", cJSON_GetErrorPtr()); // error message
            return; // exit
        }
        
        if (json) { // if valid JSON
            cJSON *player_json = cJSON_GetObjectItem(json, name); // get player data
            if (player_json) { // if data exists
                snprintf(player->name, sizeof(player->name), "%s", name); // copy name
                player->supcoins = cJSON_GetObjectItem(player_json, "supcoins")->valueint; // load supcoins
                
                cJSON *supemons_array = cJSON_GetObjectItem(player_json, "supemons"); // get Supemons array
                if (supemons_array) { // if array exists
                    int i = 0; // index
                    cJSON *supemon; // Supemon variable
                    cJSON_ArrayForEach(supemon, supemons_array) { // iterate Supemons
                        if (i < MAX_SUPEMON) { // if within bounds
                            const char *supemon_name = supemon->valuestring; // get name
                            strncpy(player->supemons[i], supemon_name, sizeof(player->supemons[i]) - 1); // copy name
                            player->supemons[i][sizeof(player->supemons[i]) - 1] = '\0'; // null-terminate
                            i++; // increment index
                        }
                    }
                    for (; i < MAX_SUPEMON; i++) { // clear remaining Supemons
                        player->supemons[i][0] = '\0'; // set to empty
                    }
                }
                
                cJSON *selected_supemon = cJSON_GetObjectItem(player_json, "selected_supemon"); // get selected Supemon
                if (selected_supemon && selected_supemon->valuestring) { // if exists
                    const char *selected_name = selected_supemon->valuestring; // get name
                    strncpy(player->selected_supemon, selected_name, sizeof(player->selected_supemon) - 1); // copy name
                    player->selected_supemon[sizeof(player->selected_supemon) - 1] = '\0'; // null-terminate
                }
            } else {
                printf("Player not found: %s\n", name); // error if not found
            }
            cJSON_Delete(json); // free JSON
        } else {
            printf("Error parsing JSON data.\n"); // error message
        }
    } else {
        printf("Error loading player data.\n"); // error if file can't be opened
    }
}