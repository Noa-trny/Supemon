#include "item.h"
#include "supemon.h"
#include <string.h>
#include <stdio.h>

// global shop inventory and battle usage counter
ShopItem shop_items[MAX_ITEMS];
static int items_used_in_battle = 0;

// applies item effects to target supemon:
// - checks battle usage limit
// - handles different item types:
//   * potions: restore HP
//   * rare candy: level up
// - updates item quantity and usage counter
void use_item(ShopItem* item, Supemon* target) {
    // check battle usage limit
    if (items_used_in_battle >= MAX_ITEMS_PER_BATTLE) {
        printf("You can't use any more items in this battle!\n");
        return;
    }

    // apply item effects based on type
    switch (item->type) {
        case ITEM_POTION:
            // restore 5 HP, not exceeding max HP
            target->hp = (target->hp + 5 > target->max_hp) ? 
                         target->max_hp : target->hp + 5;
            printf("%s recovered 5 HP!\n", target->name);
            break;

        case ITEM_SUPER_POTION:
            // restore 10 HP, not exceeding max HP
            target->hp = (target->hp + 10 > target->max_hp) ? 
                         target->max_hp : target->hp + 10;
            printf("%s recovered 10 HP!\n", target->name);
            break;

        case ITEM_RARE_CANDY:
            // increase supemon level
            level_up_supemon(target);
            printf("%s gained a level!\n", target->name);
            break;

        default:
            printf("Unknown item type!\n");
            return;
    }
    
    // update item quantity and battle usage
    item->quantity--;
    items_used_in_battle++;
}

// calculates resale value of items (half of purchase price)
int get_item_sell_price(const ShopItem* item) {
    return item->price / 2;
}

// returns number of items used in current battle
int get_items_used_in_battle(void) {
    return items_used_in_battle;
}

// resets battle item usage counter (called at battle end)
void reset_items_used_in_battle(void) {
    items_used_in_battle = 0;
}