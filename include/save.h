#ifndef SAVE_H
#define SAVE_H

#include "player.h"

void save_player(const Player *player);
void load_player(Player *player, const char *filename);

#endif // SAVE_H