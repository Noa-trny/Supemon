#ifndef ITEM_H
#define ITEM_H

#include "supemon.h"

// maximum number of different items in shop and items usable per battle
#define MAX_ITEMS 10
#define MAX_ITEMS_PER_BATTLE 4

// available item types in the game:
// - potions: restore supemon hp
// - rare candy: increases supemon level
typedef enum {
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_RARE_CANDY,
    ITEM_COUNT
} ItemType;

// shop item structure containing:
// - basic info: name, type, price
// - quantity: number owned by player
// - description: item effect description
// - effect_value: numerical value of the effect (healing amount, etc.)
typedef struct {
    char name[50];
    ItemType type;
    int price;
    int quantity;
    char description[100];
    int effect_value;
} ShopItem;

// global array of available items in the shop
extern ShopItem shop_items[MAX_ITEMS];

// item management functions
void initialize_shop_items(void);                      // sets up initial shop inventory with default items
void use_item(ShopItem* item, Supemon* target);       // applies item effect and increments usage counter
int get_item_sell_price(const ShopItem* item);        // returns item sell price (half of buy price)
int get_items_used_in_battle(void);                   // returns current number of items used in battle
void reset_items_used_in_battle(void);                // resets battle item counter to 0

#endif // ITEM_H