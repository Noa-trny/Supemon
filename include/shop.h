#ifndef SHOP_H
#define SHOP_H

#include "item.h"
#include "player.h"

// shop system functions for purchasing and managing items:
// - display_shop: shows available items and their prices
// - buy_item: handles item purchase and inventory management
void display_shop(Player *player);     // displays available items in the shop
void buy_item(int item_index, Player *player);  // processes item purchase using supcoins

#endif // SHOP_H