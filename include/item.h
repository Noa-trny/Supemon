#ifndef ITEM_H
#define ITEM_H

#define MAX_ITEMS 10 // Nombre maximum d'items

typedef struct {
    char name[50];
    int price; // Prix en Supcoins
    int quantity; // Quantité disponible
    char description[100]; // Description de l'item
} ShopItem;

extern ShopItem shop_items[MAX_ITEMS];

void initialize_shop_items();

#endif // ITEM_H