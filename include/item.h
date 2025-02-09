#ifndef ITEM_H
#define ITEM_H

#include "supemon.h"

#define MAX_ITEMS 10
#define MAX_ITEMS_PER_BATTLE 4

typedef enum {
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_RARE_CANDY,
    ITEM_COUNT
} ItemType;

typedef struct {
    char name[50];
    ItemType type;
    int price;
    int quantity;
    char description[100];
    int effect_value;
} ShopItem;

extern ShopItem shop_items[MAX_ITEMS];

void initialize_shop_items(void);
void use_item(ShopItem* item, Supemon* target);
int get_item_sell_price(const ShopItem* item);
int get_items_used_in_battle(void);
void reset_items_used_in_battle(void);

#endif // ITEM_H