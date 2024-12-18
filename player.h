#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

typedef struct Player {
    int health;
    int strength;
    int inventory_capacity;
    Item *inventory;
    Room *current_room;
} Player;

void initialize_player(Player *player, Room *start_room);
void free_player(Player *player);
void initialize_game(Player *player, Room *start_room);

#endif // PLAYER_H