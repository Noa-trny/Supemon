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

// starter supemon templates with initial stats and moves
static const Supemon SUPMANDER = {
    .name = "Supmander",   
    .level = 1,
    .exp = 0,
    .hp = 10,              // balanced hp
    .max_hp = 10,
    .attack = 1,           // high attack potential
    .base_attack = 1,
    .defense = 1,
    .base_defense = 1,
    .evasion = 1,
    .base_evasion = 1,
    .accuracy = 2,
    .base_accuracy = 2,
    .speed = 1,
    .moves = {
        {"Scratch", 3, 0, ""},          // damage move
        {"Grawl", 0, 1, "attack"}       // attack boost
    }
};

static const Supemon SUPASAUR = {
    .name = "Supasaur",     
    .level = 1,
    .exp = 0,
    .hp = 9,               // lower hp but high evasion
    .max_hp = 9,
    .attack = 1,
    .base_attack = 1,
    .defense = 1,
    .base_defense = 1,
    .evasion = 3,          // highest evasion
    .base_evasion = 3,
    .accuracy = 2,
    .base_accuracy = 2,
    .speed = 2,
    .moves = {
        {"Pound", 2, 0, ""},            // damage move
        {"Foliage", 0, 1, "evasion"}    // evasion boost
    }
};

static const Supemon SUPIRTLE = {
    .name = "Supirtle",    
    .level = 1,
    .exp = 0,
    .hp = 11,              // highest hp and defense
    .max_hp = 11,
    .attack = 1,
    .base_attack = 1,
    .defense = 2,          // high defense
    .base_defense = 2,
    .evasion = 2,
    .base_evasion = 2,
    .accuracy = 1,
    .base_accuracy = 1,
    .speed = 2,
    .moves = {
        {"Pound", 2, 0, ""},            // damage move
        {"Shell", 0, 1, "defense"}      // defense boost
    }
};

#endif // SUPEMON_H