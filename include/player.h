#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SUPEMON 10
#define MAX_ITEMS 10

typedef struct {
    char name[50];
    char supemons[MAX_SUPEMON][50]; 
    char selected_supemon[50]; 
    int supcoins; 
    char items[MAX_ITEMS][50]; 
} Player;

#endif // PLAYER_H