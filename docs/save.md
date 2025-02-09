# Save System Documentation

## Overview

The Supemon game uses a JSON-based save system to persist player data, including their Supemons, inventory, and progress.

## Core Functions

### Save Functions

1. `save_player(const Player *player)`
   - Main saving function that handles all player data persistence
   - Creates or updates the save file in JSON format
   - Preserves existing data when updating
   - Handles player info, Supemons, and inventory

2. `load_player(Player *player, const char *name)`
   - Loads complete player data from save file
   - Restores player state, owned Supemons, and inventory
   - Handles missing or corrupted save data
   - Initializes new player if save not found

3. `save_game_state(Player *player)`
   - Quick save function called after important events
   - Used after battles, purchases, healing, etc.
   - Ensures player progress is not lost

4. `update_player_supemon_stats(Player* player, Supemon* supemon)`
   - Updates specific Supemon stats in save file
   - Preserves experience and level progress
   - Maintains consistency between memory and save file

## Save File Structure

The save file (`save.json`) uses the following JSON structure:

```json
{
    "save_name": {
        "supcoins": 1000,
        "selected_supemon": "Supmander",
        "supemons": [
            {
                "name": "Supmander",
                "level": 5,
                "exp": 1200,
                "hp": 15,
                "max_hp": 15,
                "attack": 3,
                "defense": 2,
                "evasion": 1,
                "accuracy": 2,
                "speed": 2
            },
            {
                "name": "Supirtle",
                "level": 3,
                "exp": 800,
                "hp": 12,
                "max_hp": 12,
                "attack": 2,
                "defense": 3,
                "evasion": 2,
                "accuracy": 1,
                "speed": 2
            }
        ],
        "items": [
            {
                "name": "Potion",
                "type": 0,
                "price": 100,
                "quantity": 5,
                "description": "Restores 5 HP",
                "effect_value": 5
            },
            {
                "name": "Rare Candy",
                "type": 2,
                "price": 700,
                "quantity": 1,
                "description": "Increases Supemon's level by 1",
                "effect_value": 1
            }
        ]
    }
}
```

# Working
- Use `save_game_state()` after important events
- Update Supemon stats using `update_player_supemon_stats()`
- When editing `save/load functions` you need to delete your json save file.