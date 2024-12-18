#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "player.h"
typedef enum {
    ITEM_SWORD,
    ITEM_HEALTH_POTION,
    ITEM_UNKNOWN
} ItemType;



void pickup_item(Player *player, char *item_name);
// Command Processing
int process_command(Player *player, char *command);

// Game Actions
void save_game(Player *player);
void load_game(Player *player);
void move_player(Player *player, char *direction);
void look_around(Player *player);
void pickup_item(Player *player, char *item_name);
void display_inventory(Player *player);
void attack_creature(Player *player);

#endif // GAME_H