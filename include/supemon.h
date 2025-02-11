#ifndef SUPEMON_H
#define SUPEMON_H
#include "move.h"

// supemon structure representing a creature with:
// - basic info: name, level, experience points
// - battle stats: hp, attack, defense, evasion, accuracy, speed
// - base stats: used for stat calculations and level ups
// - moves: array of 2 battle moves (attack and status effects)
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

// functions for supemon management
Supemon* create_supemon_copy(const Supemon* source);    // creates a deep copy of a supemon
Supemon* create_supemon_by_name(const char* name);      // creates a new supemon instance by its name
void level_up_supemon(Supemon* supemon);               // increases supemon level and updates stats

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

static const Supemon THUNDEROX = {
    .name = "Thunderox",    
    .level = 1,
    .exp = 0,
    .hp = 8,
    .max_hp = 8,
    .attack = 1,
    .base_attack = 1,
    .defense = 1,
    .base_defense = 1,
    .evasion = 2,
    .base_evasion = 2,
    .accuracy = 2,
    .base_accuracy = 2,
    .speed = 3,
    .moves = {
        {"Thunder", 4, 0, ""},
        {"Agility", 0, 1, "speed"}
    }
};

static const Supemon ROCKBEAR = {
    .name = "Rockbear",    
    .level = 1,
    .exp = 0,
    .hp = 12,
    .max_hp = 12,
    .attack = 1,
    .base_attack = 1,
    .defense = 3,
    .base_defense = 3,
    .evasion = 1,
    .base_evasion = 1,
    .accuracy = 1,
    .base_accuracy = 1,
    .speed = 1,
    .moves = {
        {"Rock Slam", 3, 0, ""},
        {"Harden", 0, 1, "defense"}
    }
};

static const Supemon PSYCAT = {
    .name = "Psycat",    
    .level = 1,
    .exp = 0,
    .hp = 9,
    .max_hp = 9,
    .attack = 1,
    .base_attack = 1,
    .defense = 1,
    .base_defense = 1,
    .evasion = 2,
    .base_evasion = 2,
    .accuracy = 3,
    .base_accuracy = 3,
    .speed = 2,
    .moves = {
        {"Mind Blast", 3, 0, ""},
        {"Focus", 0, 1, "accuracy"}
    }
};

static const Supemon AQUAFIN = {
    .name = "Aquafin",    
    .level = 1,
    .exp = 0,
    .hp = 10,
    .max_hp = 10,
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
        {"Water Jet", 3, 0, ""},
        {"Mist", 0, 1, "evasion"}
    }
};

static const Supemon LEAFOX = {
    .name = "Leafox",    
    .level = 1,
    .exp = 0,
    .hp = 9,
    .max_hp = 9,
    .attack = 1,
    .base_attack = 1,
    .defense = 2,
    .base_defense = 2,
    .evasion = 2,
    .base_evasion = 2,
    .accuracy = 2,
    .base_accuracy = 2,
    .speed = 2,
    .moves = {
        {"Leaf Blade", 3, 0, ""},
        {"Growth", 0, 1, "attack"}
    }
};

#endif // SUPEMON_H