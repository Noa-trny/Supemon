#include "shop.h"
#include "item.h"
#include "save.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

ShopItem shop_items[MAX_ITEMS];

void initialize_shop_items(void) {
    ShopItem items[] = {
        {
            .name = "Potion",
            .type = ITEM_POTION,
            .price = 100,
            .quantity = 5,
            .description = "Restores 5 HP",
            .effect_value = 5
        },
        {
            .name = "Super Potion",
            .type = ITEM_SUPER_POTION,
            .price = 300,
            .quantity = 3,
            .description = "Restores 10 HP",
            .effect_value = 10
        },
        {
            .name = "Rare Candy",
            .type = ITEM_RARE_CANDY,
            .price = 700,
            .quantity = 2,
            .description = "Increases Supemon's level by 1",
            .effect_value = 1
        }
    };

    memcpy(shop_items, items, sizeof(items));
}

void buy_item(int item_index, Player *player) {
    if (item_index < 0 || item_index >= MAX_ITEMS) {
        printf("Invalid item selection.\n");
        return;
    }

    ShopItem *item = &shop_items[item_index];
    if (item->quantity <= 0) {
        printf("Item is out of stock.\n");
        return;
    }

    if (player->supcoins < item->price) {
        printf("Not enough Supcoins.\n");
        return;
    }

    player->supcoins -= item->price;
    item->quantity--;

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(player->items[i].name, item->name) == 0) {
            player->items[i].quantity++;
            printf("Bought %s! (Quantity: %d)\n", item->name, player->items[i].quantity);
            return;
        }
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->items[i].quantity == 0) {
            memcpy(&player->items[i], item, sizeof(ShopItem));
            player->items[i].quantity = 1;
            printf("Bought %s!\n", item->name);
            break;
        }
    }

    save_player(player);
}

void display_shop(Player *player) {
    printf("Welcome to the shop!\n");
    printf("You have %d Supcoins.\n", player->supcoins);
    printf("Available items:\n");

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (shop_items[i].quantity > 0) {
            printf("%d. %s - %d Supcoins (Stock: %d) - %s\n",
                   i + 1, shop_items[i].name, shop_items[i].price, 
                   shop_items[i].quantity, shop_items[i].description);
        }
    }

    int item_index;
    while (1) {
        printf("Enter item number to buy (0 to exit): ");
        scanf("%d", &item_index);

        if (item_index == 0) {
            printf("Thank you for visiting!\n");
            break;
        }

        buy_item(item_index - 1, player);
    }
}
