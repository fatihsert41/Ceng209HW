#include <stdio.h>
#include <stdlib.h>
#include "player.h"

void initialize_player(Player *player, Room *start_room) {
    player->health = 100;
    player->strength = 25;
    player->inventory_capacity = 5;
    player->inventory = NULL;
    player->current_room = start_room;
}

void free_player(Player *player) {
    Item *current = player->inventory;
    while (current) {
        Item *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

void initialize_game(Player *player, Room *start_room) {
    initialize_player(player, start_room);
}