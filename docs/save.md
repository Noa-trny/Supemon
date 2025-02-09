# Save System Documentation

## Functions

1. `save_player(const Player *player)`
   - Main save function that handles all player data persistence
   - Manages save file in JSON format for easy data structure
   - Preserves all player progress and stats
   - Called after important events (battles, purchases, healing, etc.)
   - Ensures player progress is not lost

2. `load_player(Player *player, const char *name)`
   - Loads complete player data from save file
   - Restores player state, owned Supemons, and inventory
   - Handles missing or corrupted save data
   - Initializes new player if save not found

3. `update_player_supemon_stats(Player* player, Supemon* supemon)`
   - Updates specific Supemon stats in save file
   - Preserves experience and level progress
   - Maintains consistency between memory and save file

## Save File Structure

The game uses a JSON file (`save.json`) to store all player data:

```json
{
    "PlayerName": {
        "supcoins": 1000,
        "supemons": [
            {
                "name": "Supmander",
                "level": 5,
                "exp": 1200,
                "hp": 15,
                "max_hp": 15,
                "attack": 8,
                "defense": 6,
                "evasion": 4,
                "accuracy": 7,
                "speed": 5
            }
        ],
        "selected_supemon": "Supmander",
        "items": [
            {
                "name": "Potion",
                "type": 0,
                "price": 100,
                "quantity": 3,
                "description": "Restores 5 HP",
                "effect_value": 5
            }
        ]
    }
}
```

## Save System Usage

- Use `save_player()` after important events
- Update Supemon stats using `update_player_supemon_stats()`
- When editing `save/load functions` you need to delete your json save file.