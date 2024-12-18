#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "player.h"
#include "game.h"

void display_initial_menu();
void display_player_stats(Player *player);

int main() {
    Player player;

    // Create rooms
    Room *room1 = create_room(1, "This is the starting room.");
    Room *room2 = create_room(2, "This is dark room");
    Room *room3 = create_room(3, "This is dark room");
    Room *room4 = create_room(4, "This is dark room");
    Room *room5 = create_room(5, "This is dark room");
    Room *room6 = create_room(6, "Final Room");

    // Connect rooms
    connect_rooms(room1, room2, "right");
    connect_rooms(room1, room3, "left");
    connect_rooms(room1, room4, "up");
    connect_rooms(room1, room5, "down");
    connect_rooms(room2, room6, "right");

    // Add creatures
    room2->creature = (Creature *)malloc(sizeof(Creature));
    room2->creature->name = strdup("Goblin");
    room2->creature->health = 100;
    room2->creature->strength = 10;

    room4->creature = (Creature *)malloc(sizeof(Creature));
    room4->creature->name = strdup("Orc");
    room4->creature->health = 100;
    room4->creature->strength = 15;

    room6->creature = (Creature *)malloc(sizeof(Creature));
    room6->creature->name = strdup("Troll");
    room6->creature->health = 100;
    room6->creature->strength = 20;

    // Add items
    room3->items = (Item *)malloc(sizeof(Item));
    room3->items->name = strdup("Sword");
    room3->items->next = NULL;

    room5->items = (Item *)malloc(sizeof(Item));
    room5->items->name = strdup("Health Potion");
    room5->items->next = NULL;

    // Initialize the player in the starting room
    initialize_game(&player, room1);

    display_initial_menu();

    char command[256];
    int action_taken;

    printf("Enter a command: ");
    while (fgets(command, sizeof(command), stdin)) {
        action_taken = process_command(&player, command);  // Process command

        if (action_taken) {
            display_player_stats(&player);  // Show stats only if meaningful action is taken
        }

        // Check if all creatures are defeated
        int all_creatures_defeated = 1;
        Room *rooms[] = {room2, room4, room6};
        for (int i = 0; i < 3; i++) {
            if (rooms[i]->creature) {
                all_creatures_defeated = 0;
                break;
            }
        }

        if (all_creatures_defeated) {
            printf("Congratulations! You have defeated all creatures and won the game!\n");
            break;
        }

        printf("\nEnter a command: ");
    }

    free_rooms(room1);
    free_player(&player);
    return 0;
}

void display_initial_menu() {
    printf("\n--- Map ---\n");
    printf(
        "         +---+\n"
        "         | 4 |\n"
        "         +---+\n"
        "           |\n"
        "+---+---+---+---+\n"
        "| 3 | 1 | 2 | 6 |\n"
        "+---+---+---+---+\n"
        "           |\n"
        "         +---+\n"
        "         | 5 |\n"
        "         +---+\n"
    );

    printf("\n--- Menu ---\n");
    printf("Commands:\n");
    printf("  move <direction> : Move to a different room.\n");
    printf("  look             : Look around the room.\n");
    printf("  take <item>      : Pick up an item.\n");
    printf("  display          : Display your inventory.\n");
    printf("  attack           : Attack a creature.\n");
    printf("  exit             : Exit the game.\n");
    printf("  save             : Save the game.\n");
    printf("  load             : Load the game.\n");
    }

void display_player_stats(Player *player) {
    printf("\n--- Player Stats ---\n");
    printf("Health: %d\n", player->health);
    printf("Strength: %d\n", player->strength);
}

