#include "shop.h"
#include "item.h"
#include "save.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

ShopItem shop_items[MAX_ITEMS];

void initialize_shop_items() {
    strcpy(shop_items[0].name, "Potion");
    shop_items[0].price = 100;
    shop_items[0].quantity = 5;
    strcpy(shop_items[0].description, "Restaure 5 HP.");

    strcpy(shop_items[1].name, "Super Potion");
    shop_items[1].price = 300;
    shop_items[1].quantity = 3;
    strcpy(shop_items[1].description, "Restaure 10 HP.");

    strcpy(shop_items[2].name, "Rare Candy");
    shop_items[2].price = 700;
    shop_items[2].quantity = 2;
    strcpy(shop_items[2].description, "Ajoute un niveau a votre Supemon.");
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
        printf("You do not have enough Supcoins to buy this item.\n");
        return;
    }

    // Deduct Supcoins and update item quantity
    player->supcoins -= item->price;
    item->quantity--;

    // Add item to player's inventory
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(player->items[i].name, item->name) == 0) {
            player->items[i].quantity++;
            printf("You bought a %s! (Quantity: %d)\n", item->name, player->items[i].quantity);
            return; // Item already exists, just update quantity
        }
    }

    // If item does not exist, add it to the inventory
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->items[i].quantity == 0) { // Find an empty slot
            strcpy(player->items[i].name, item->name);
            player->items[i].price = item->price;
            player->items[i].quantity = 1;
            strcpy(player->items[i].description, item->description);
            printf("You bought a %s!\n", item->name);
            break;
        }
    }

    // Save player data after purchase
    save_player(player);
}

void display_shop(Player *player) {
    printf("Bienvenue dans la boutique !\n");
    printf("Vous avez %d Supcoins.\n", player->supcoins);
    printf("Objets disponibles :\n");

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (shop_items[i].quantity > 0) {
            printf("%d. %s - %d Supcoins (Quantite: %d) - %s\n",
                   i + 1, shop_items[i].name, shop_items[i].price, 
                   shop_items[i].quantity, shop_items[i].description);
        }
    }

    int item_index;
    while (1) {
        printf("Entrez le numero de l'objet que vous souhaitez acheter (0 pour quitter) : ");
        scanf("%d", &item_index);

        if (item_index == 0) {
            printf("Vous avez quitte la boutique.\n");
            break; // Sortir de la boucle
        }

        buy_item(item_index - 1, player); // Appel à la fonction d'achat
    }
}
