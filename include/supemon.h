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


static const Supemon SUPMANDER = {
    .name = "Supmander",
    .level = 1,
    .exp = 0,
    .hp = 10,
    .max_hp = 10,
    .attack = 1,
    .base_attack = 1,
    .defense = 1,
    .base_defense = 1,
    .evasion = 1,
    .base_evasion = 1,
    .accuracy = 2,
    .base_accuracy = 2,
    .speed = 1,
    .moves = {
        {"Scratch", 3, 0, ""},
        {"Grawl", 0, 1, "attack"}
    }
};

static const Supemon SUPASAUR = {
    .name = "Supasaur",
    .level = 1,
    .exp = 0,
    .hp = 9,
    .max_hp = 9,
    .attack = 1,
    .base_attack = 1,
    .defense = 1,
    .base_defense = 1,
    .evasion = 3,
    .base_evasion = 3,
    .accuracy = 2,
    .base_accuracy = 2,
    .speed = 2,
    .moves = {
        {"Pound", 2, 0, ""},
        {"Foliage", 0, 1, "evasion"}
    }
};

static const Supemon SUPIRTLE = {
    .name = "Supirtle",
    .level = 1,
    .exp = 0,
    .hp = 11,
    .max_hp = 11,
    .attack = 1,
    .base_attack = 1,
    .defense = 2,
    .base_defense = 2,
    .evasion = 2,
    .base_evasion = 2,
    .accuracy = 1,
    .base_accuracy = 1,
    .speed = 2,
    .moves = {
        {"Pound", 2, 0, ""},
        {"Shell", 0, 1, "defense"}
    }
};

#endif // SUPEMON_H