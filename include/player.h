#ifndef PLAYER_H
#define PLAYER_H

#include "item.h"
// maximum number of supemons and items a player can have
#define MAX_SUPEMON 10
#define MAX_ITEMS 10

// player structure containing all player-related data:
// - name: player's username
// - supemons: array of owned supemon names
// - selected_supemon: currently active supemon for battles
// - supcoins: in-game currency for shop purchases
// - items: inventory of items owned by the player
typedef struct {
    char name[50];
    char supemons[MAX_SUPEMON][50]; 
    char selected_supemon[50]; 
    int supcoins; 
    ShopItem items[MAX_ITEMS];
} Player;

#endif // PLAYER_H