# Items System Documentation

## Available Items

### Healing Items
1. **Potion**
   - Price: 100 Supcoins
   - Sell Price: 50 Supcoins
   - Effect: Restores 5 HP
   - Use: In and out of battle
   - Description: Basic healing item for early game

2. **Super Potion**
   - Price: 300 Supcoins
   - Sell Price: 150 Supcoins
   - Effect: Restores 10 HP
   - Use: In and out of battle
   - Description: More efficient healing for tougher battles

### Enhancement Items
3. **Rare Candy**
   - Price: 700 Supcoins
   - Sell Price: 350 Supcoins
   - Effect: Increases Supemon level by 1
   - Use: Out of battle only
   - Description: Instantly levels up your Supemon

## Shop System

### Main Shop Menu
When entering the shop, you'll see:
```
Welcome to the shop!
You have X Supcoins.

What would you like to do?
1. Buy items
2. Sell items
3. Exit shop
```

### Buying Items
1. Select "Buy items" from the main shop menu
2. View available items and their prices
3. Select item number to purchase
4. Items are added to your inventory
5. Supcoins are automatically deducted

Example buy interface:
```
Available items:
1. Potion - 100 Supcoins (Stock: 5) - Restores 5 HP
2. Super Potion - 300 Supcoins (Stock: 3) - Restores 10 HP
3. Rare Candy - 700 Supcoins (Stock: 2) - Increases Supemon's level by 1
Enter item number to buy (0 to exit):
```

### Selling Items
1. Select "Sell items" from the main shop menu
2. View your items with their sell prices
3. Select item number to sell
4. Item is removed from inventory
5. Supcoins are automatically added

Example sell interface:
```
Your items (sell prices):
1. Potion - 50 Supcoins (Quantity: 2)
2. Super Potion - 150 Supcoins (Quantity: 1)
Enter item number to sell (0 to exit):
```

## Using Items

### In Battle
- Limited to 4 items per battle
- Access through battle menu (Option 3)
- Only healing items can be used
- Counts as your turn when used

Example battle item use:
```
Choose an item:
1 - Potion (x3)
2 - Super Potion (x1)
3 - Cancel
```

### Outside Battle
- Access through main menu
- No usage limits
- All items available
- Useful for preparation before battles

## Item Limitations
- Maximum 10 different items in inventory
- Cannot use items on fainted Supemons
- Items are consumed upon use
- Selling price is always half of buying price
- Items are automatically removed from inventory when quantity reaches 0