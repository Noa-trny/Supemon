# Supemon Game Documentation

## Overview

Supemon is a console-based monster collecting and battling game inspired by classic RPGs. Players can collect creatures called Supemons, battle with them, use items, and manage their team.

## Core Features

### 1. Starter Selection
When starting a new game, players can choose from three starter Supemons:
- Supmander (balanced with high attack)
- Supasaur (agile with high evasion)
- Supirtle (defensive with high HP)

Example display:
```
+-----------------------------+
| Choose your starter Supemon:|
| 1 - Supmander              |
| 2 - Supasaur               |
| 3 - Supirtle               |
+-----------------------------+
1, 2 or 3:
```

### 2. Battle System

#### Wild Encounters
Players can encounter wild Supemons with options to:
- Fight using moves
- Switch Supemons
- Use items
- Attempt capture
- Run away

Battle menu example:
```
+-----------------------------+
|What will you do?           |
| 1 - Move                   |
| 2 - Change Supemon         |
| 3 - Use item               |
| 4 - Capture                |
| 5 - Run away               |
+-----------------------------+
1, 2, 3, 4 or 5:
```

#### Battle Status Display
```
Supmander (enemy)
--------------------------------
HP: 10/10    Lvl: 1
Atk: 1       Def: 1
Acc: 2       Eva: 1
--------------------------------
Supirtle (Player)
HP: 11/11    Lvl: 1
Atk: 1       Def: 2
Acc: 1       Eva: 2
--------------------------------
```

### 3. Supemon Center
- Heal all Supemons to full HP
- Free service
- Available anytime from main menu

Example interaction:
```
Welcome to the Supemon Center!
Your Supemons:
1 - Supmander (HP: 7/10)
2 - Supirtle (HP: 5/11)

All your Supemons have been healed to full health!
We hope to see you again!
```

### 4. Shop System
Players can buy items using Supcoins:
- Potion (100 coins): Restores 5 HP
- Super Potion (300 coins): Restores 10 HP
- Rare Candy (700 coins): Increases level by 1

Shop interface example:
```
Welcome to the shop!
You have 1000 Supcoins.
Available items:
1. Potion - 100 Supcoins (Stock: 5) - Restores 5 HP
2. Super Potion - 300 Supcoins (Stock: 3) - Restores 10 HP
3. Rare Candy - 700 Supcoins (Stock: 2) - Increases Supemon's level by 1
Enter item number to buy (0 to exit):
```

### 5. Main Game Menu
```
+-----------------------------+
| Where do you want to go?    |
| 1 - Into the Wild          |
| 2 - In the shop            |
| 3 - In the Supemon Center  |
| 4 - Leave the Game         |
+-----------------------------+
1, 2, 3 or 4:
```

## Game Mechanics

### Supemon Stats
Each Supemon has the following attributes:
- HP (Health Points)
- Attack (damage dealing)
- Defense (damage reduction)
- Evasion (dodge chance)
- Accuracy (hit chance)
- Speed (turn order)

### Battle Mechanics
- Turn-based combat system
- Speed determines who goes first
- Dodge chance based on evasion/accuracy ratio
- Damage calculation: `damage = (attack * move_damage) / (defense / 2 + 1)`
- Maximum 4 items can be used per battle

### Level System
- Gain experience from battles
- Stats increase by ~30% per level
- Random variation in stat increases
- Rare Candy provides instant level up

### Capture System
- Chance increases as enemy HP decreases
- Base formula: `chance = (max_hp - current_hp) / max_hp - 0.5`
- Maximum 10 Supemons can be owned

## Save System
See `./docs/save.md`

## Controls 
The game uses simple numeric inputs as a regular CLI:
- Enter numbers to select menu options
- 0 typically exits or cancels
- Follow on-screen prompts for navigation

## Technical Notes
- Written fully in C
- Uses cJSON for save file handling
- Console-based interface
- Modular code structure with separate systems for:
  - Battles
  - Shop
  - Healing
  - Save/Load
  - Display
  - Item management 