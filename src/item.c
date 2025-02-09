#include "item.h"
#include "supemon.h"
#include <string.h>
#include <stdio.h>

ShopItem shop_items[MAX_ITEMS];
static int items_used_in_battle = 0;

void use_item(ShopItem* item, Supemon* target) {
    if (items_used_in_battle >= MAX_ITEMS_PER_BATTLE) {
        printf("You can't use any more items in this battle!\n");
        return;
    }

    switch (item->type) {
        case ITEM_POTION:
            target->hp = (target->hp + 5 > target->max_hp) ? 
                         target->max_hp : target->hp + 5;
            printf("%s recovered 5 HP!\n", target->name);
            break;

        case ITEM_SUPER_POTION:
            target->hp = (target->hp + 10 > target->max_hp) ? 
                         target->max_hp : target->hp + 10;
            printf("%s recovered 10 HP!\n", target->name);
            break;

        case ITEM_RARE_CANDY:
            level_up_supemon(target);
            printf("%s gained a level!\n", target->name);
            break;

        default:
            printf("Unknown item type!\n");
            return;
    }
    
    item->quantity--;
    items_used_in_battle++;
}

int get_item_sell_price(const ShopItem* item) {
    return item->price / 2;
}

int get_items_used_in_battle(void) {
    return items_used_in_battle;
}

void reset_items_used_in_battle(void) {
    items_used_in_battle = 0;
}