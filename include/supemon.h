#ifndef SUPEMON_H
#define SUPEMON_H

typedef struct {
    char name[50];
    int damage;
    int stat_boost;
    char stat_affected[20];
} Move;

typedef struct {
    char name[50];
    int level;
    int exp;
    int hp;
    int max_hp;
    int attack;
    int base_attack;
    int defense;
    int base_defense;
    int evasion;
    int base_evasion;
    int accuracy;
    int base_accuracy;
    int speed;
    Move moves[2];
} Supemon;


#endif // SUPEMON_H