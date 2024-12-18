#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

Room *create_room(int id, const char *description) {
    Room *room = (Room *)malloc(sizeof(Room));
    if (!room) {
        perror("Failed to allocate memory for room");
        exit(EXIT_FAILURE);
    }
    room->id = id;
    room->description = strdup(description);
    room->up = room->down = room->left = room->right = NULL;
    room->items = NULL;
    room->creature = NULL;
    return room;
}

void connect_rooms(Room *room1, Room *room2, const char *direction) {
    if (strcmp(direction, "up") == 0) {
        room1->up = room2;
        room2->down = room1;
    } else if (strcmp(direction, "down") == 0) {
        room1->down = room2;
        room2->up = room1;
    } else if (strcmp(direction, "left") == 0) {
        room1->left = room2;
        room2->right = room1;
    } else if (strcmp(direction, "right") == 0) {
        room1->right = room2;
        room2->left = room1;
    }
}

void free_rooms(Room *room) {
    if (!room) return;
    free_rooms(room->up);
    free_rooms(room->down);
    free_rooms(room->left);
    free_rooms(room->right);
    free(room->description);
    free(room);
}