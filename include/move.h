#ifndef MOVE_H
#define MOVE_H

// move structure representing a supemon's attack or ability:
// - name: identifier of the move
// - damage: amount of damage dealt (0 for status moves)
// - stat_boost: amount to modify the affected stat
// - stat_affected: which stat to boost ("attack", "defense", "evasion", etc.)
typedef struct {
    char name[50];
    int damage;
    int stat_boost;
    char stat_affected[20];
} Move;

#endif // MOVE_H