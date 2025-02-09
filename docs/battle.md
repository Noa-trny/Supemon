# Battle System Documentation

## Overview
The battle system is turn-based, featuring wild Supemon encounters. Players can use moves, items, switch Supemons, attempt captures, or try to run away.

## Battle Flow

### 1. Battle Start
- Wild Supemon appears with level similar to player's
- Speed determines first turn
- Battle status displayed

Example encounter:
```
A wild Supmander appeared!

Supmander (enemy)
--------------------------------
HP: 10/10    Lvl: 1
Atk: 1       Def: 1
Acc: 2       Eva: 1
--------------------------------
```

### 2. Battle Menu
```
+-----------------------------+
|What will you do?           |
| 1 - Move                   |
| 2 - Change Supemon         |
| 3 - Use item               |
| 4 - Capture                |
| 5 - Run away               |
+-----------------------------+
```

## Battle Actions

### 1. Using Moves
- Each Supemon has 2 moves:
  - Damage move
  - Status boost move
- Damage calculation: `damage = (attack * move_damage) / (defense / 2 + 1)`
- Dodge chance: `probability = evasion / (accuracy + evasion)`

### 2. Changing Supemon
- Switch to any other Supemon in team
- Counts as your turn
- Shows available Supemons and their HP

### 3. Using Items
- Limited to 4 items per battle
- Only healing items usable
- Counts as your turn
- Shows inventory with quantities

### 4. Capturing
- Success chance increases with lower enemy HP
- Formula: `chance = (max_hp - current_hp) / max_hp - 0.5`
- Ends battle if successful
- Fails if team is full (max 10)

### 5. Running
- Success based on speed comparison
- Formula: `chance = player_speed / (player_speed + enemy_speed)`
- Ends battle if successful

## Battle Rewards

### Victory Rewards
- Experience points: `exp = (100-500) * enemy_level`
- Supcoins: Random 100-500
- Automatic save after battle

### Capture Rewards
- New Supemon added to team
- No experience gained
- Automatic save after capture
