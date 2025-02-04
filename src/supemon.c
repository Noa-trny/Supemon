#include "supemon.h"
#include <string.h>
#include <stdlib.h>

Supemon* create_supemon_copy(const Supemon* source) {
    Supemon* new_supemon = malloc(sizeof(Supemon));
    memcpy(new_supemon, source, sizeof(Supemon));
    return new_supemon;
}

Supemon* create_supemon_by_name(const char* name) {
    if (strcmp(name, "Supmander") == 0) {
        return create_supemon_copy(&SUPMANDER);
    } else if (strcmp(name, "Supasaur") == 0) {
        return create_supemon_copy(&SUPASAUR);
    } else if (strcmp(name, "Supirtle") == 0) {
        return create_supemon_copy(&SUPIRTLE);
    }
    return NULL;
}

static int increase_stat(int stat) {
    float new_stat = stat * 1.3f;
    return ((float)rand() / RAND_MAX < 0.5f) ? (int)new_stat : (int)(new_stat + 0.5f);
}

void level_up_supemon(Supemon* supemon) {
    supemon->level++;
    
    supemon->max_hp = increase_stat(supemon->max_hp);
    supemon->base_attack = increase_stat(supemon->base_attack);
    supemon->base_defense = increase_stat(supemon->base_defense);
    supemon->base_evasion = increase_stat(supemon->base_evasion);
    supemon->base_accuracy = increase_stat(supemon->base_accuracy);
    supemon->speed = increase_stat(supemon->speed);
    
    supemon->hp = supemon->max_hp;
    supemon->attack = supemon->base_attack;
    supemon->defense = supemon->base_defense;
    supemon->evasion = supemon->base_evasion;
    supemon->accuracy = supemon->base_accuracy;
}

