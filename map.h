#ifndef MAP_H
#define MAP_H

typedef struct Item {
    char *name;
    struct Item *next;
} Item;

typedef struct Creature {
    char *name;
    int health;
    int strength;
} Creature;

typedef struct Room {
    int id;
    char *description;
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
    Item *items;
    Creature *creature;
} Room;

// Room Functions
Room *create_room(int id, const char *description);
void connect_rooms(Room *room1, Room *room2, const char *direction);
void free_rooms(Room *room);

#endif // MAP_H