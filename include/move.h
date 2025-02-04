#ifndef MOVE_H
#define MOVE_H

typedef struct {
    char name[50];
    int damage;
    int stat_boost;
    char stat_affected[20];
} Move;

#endif // MOVE_H