#ifndef SHOP_H
#define SHOP_H

#include "item.h"
#include "player.h"

void display_shop(Player *player);
void buy_item(int item_index, Player *player);

#endif // SHOP_H