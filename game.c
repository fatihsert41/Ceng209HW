#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"

int process_command(Player *player, char *command) {
    if (strncmp(command, "save", 4) == 0) {
    save_game(player);
    } else if(strncmp(command, "load", 4) == 0) {
    load_game(player);
    }else if (strncmp(command, "move", 4) == 0) {
        move_player(player, command + 5); // Sadece yönü al
    } else if (strncmp(command, "look", 4) == 0) {
        look_around(player); // Look komutu ile odanın detaylarını göster
    } else if (strncmp(command, "take", 4) == 0) {
        pickup_item(player, command + 5); // Eşyayı al
        return 1;  // Anlamlı bir eylem
    } else if (strncmp(command, "display", 7) == 0) {
        display_inventory(player); // Envanteri göster
    } else if (strncmp(command, "attack", 6) == 0) {
        attack_creature(player); // Yaratığa saldır
        return 1;  // Anlamlı bir eylem
    } else if (strncmp(command, "exit", 4) == 0) {
        printf("Exiting the game. Goodbye!\n");
        exit(0);
    }
    else {
        printf("Invalid command. Please try again.\n");
    }
    

    return 0;  // Anlamlı olmayan eylem
}

void move_player(Player *player, char *direction) {
    direction[strcspn(direction, "\n")] = '\0'; // Yönü temizle

    Room *new_room = NULL;
    if (strcmp(direction, "up") == 0) {
        new_room = player->current_room->up;
    } else if (strcmp(direction, "down") == 0) {
        new_room = player->current_room->down;
    } else if (strcmp(direction, "left") == 0) {
        new_room = player->current_room->left;
    } else if (strcmp(direction, "right") == 0) {
        new_room = player->current_room->right;
    } else {
        return;
    }

    if (new_room) {
        player->current_room = new_room;
        printf("You moved %s.\n", direction);
        printf("You are now in room %d: %s\n", player->current_room->id, player->current_room->description);
        // Odadaki detayları göstermiyoruz. 'look' komutu ile gösterilecek.
    } else {
        printf("You cannot move %s from here.\n", direction);
    }
}

// look_around fonksiyonu: Yalnızca 'look' komutunda çalışacak.
// game.c
#include "game.h"

void look_around(Player *player) {
    printf("You are in: %s\n", player->current_room->description);

    // Odadaki itemleri yazdır
    if (player->current_room->items) {
        printf("Items in the room:\n");
        Item *current_item = player->current_room->items;
        while (current_item) {
            printf("- %s\n", current_item->name);
            current_item = current_item->next;
        }
    } else {
        printf("There are no items in this room.\n");
    }

    // Odadaki yaratığı yazdır
    if (player->current_room->creature) {
        printf("You see a creature here: %s\n", player->current_room->creature->name);
    }
}

// pickup_item fonksiyonu
void pickup_item(Player *player, char *item_name) {
    item_name[strcspn(item_name, "\n")] = '\0';

    Item *prev = NULL;
    Item *current = player->current_room->items;
    while (current) {
        if (strcmp(current->name, item_name) == 0) {
            int inventory_count = 0;
            Item *inv_item = player->inventory;
            while (inv_item) {
                inventory_count++;
                inv_item = inv_item->next;
            }

            if (inventory_count >= player->inventory_capacity) {
                printf("Your inventory is full! Drop an item before picking up more.\n");
                return;
            }

            if (prev) {
                prev->next = current->next;
            } else {
                player->current_room->items = current->next;
            }

            current->next = player->inventory;
            player->inventory = current;

            // Item türüne göre özellikleri güncelle
            if (strcmp(item_name, "Sword") == 0) {
                // Kılıç alındığında hasarı artır
                printf("You picked up a Sword! Your damage has increased.\n");
                player->strength += 25;  // Kılıç, oyuncunun gücünü 25 artırır (15'ten 40'a)
            } else if (strcmp(item_name, "Health Potion") == 0) {
                // Sağlık iksiri alındığında sağlığı artır
                printf("You picked up a Health Potion! Your health has increased.\n");
                player->health += 50;  // İksir, oyuncunun sağlığını 50 artırır
            }

            printf("You picked up: %s\n", item_name);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("There is no item named '%s' here.\n", item_name);
}

void display_inventory(Player *player) {
    printf("Your inventory:\n");
    if (!player->inventory) {
        printf("- (empty)\n");
        return;
    }

    Item *current = player->inventory;
    while (current) {
        printf("- %s\n", current->name);
        current = current->next;
    }
}

void save_game(Player *player) {
    FILE *file = fopen("savegame.dat", "wb");
    if (file == NULL) {
        printf("Error saving the game.\n");
        return;
    }

    fwrite(player, sizeof(Player), 1, file);
    fclose(file);
    printf("Game saved successfully!\n");
}

void load_game(Player *player) {
    FILE *file = fopen("savegame.dat", "rb");
    if (file == NULL) {
        printf("No saved game found.\n");
        return;
    }

    // Player bilgilerini yükle
    fread(player, sizeof(Player), 1, file);

    // Room bilgisini yükle
    Room *room = malloc(sizeof(Room));
    fread(room, sizeof(Room), 1, file);
    player->current_room = room;

    // Itemları yükle
    Item *item = NULL;
    Item *prev_item = NULL;
    while (fread(&item, sizeof(Item), 1, file)) {
        if (!prev_item) {
            player->current_room->items = item;
        } else {
            prev_item->next = item;
        }
        prev_item = item;
    }

    // Yaratığı yükle
    Creature *creature = malloc(sizeof(Creature));
    if (fread(creature, sizeof(Creature), 1, file)) {
        player->current_room->creature = creature;
    }

    fclose(file);
    printf("Game loaded successfully! Welcome back.\n");
}


void attack_creature(Player *player) {
    if (!player->current_room->creature) {
        printf("There is no creature to attack here.\n");
        return;
    }

    Creature *creature = player->current_room->creature;

    printf("You engage in combat with %s!\n", creature->name);
    while (player->health > 0 && creature->health > 0) {
        printf("You attack %s for %d damage.\n", creature->name, player->strength);
        creature->health -= player->strength;

        if (creature->health <= 0) {
            printf("You defeated %s!\n", creature->name);
            free(creature->name);
            free(creature);
            player->current_room->creature = NULL;
            return;
        }

        printf("%s attacks you for %d damage.\n", creature->name, creature->strength);
        player->health -= creature->strength;

        if (player->health <= 0) {
            printf("You were defeated by %s. Game Over.\n", creature->name);
            exit(0);
        }
    }
}