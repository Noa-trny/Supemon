#ifndef SHOP_H
#define SHOP_H

#include "item.h"
#include "player.h"

// shop system functions for purchasing and managing items:
// - display_shop: shows available items and their prices
// - buy_item: handles item purchase and inventory management
// - sell_item: handles item sale for supcoins
// - display_sell_menu: displays player's items for selling
void initialize_shop_items(void);      // initializes shop inventory
void initialize_player_items(Player *player);  // initializes player inventory
void display_shop(Player *player);     // displays available items in the shop
void buy_item(int item_index, Player *player);  // processes item purchase using supcoins
void sell_item(int item_index, Player *player); // processes item sale for supcoins
void display_sell_menu(Player *player); // displays player's items for selling

#endif // SHOP_H