#include "../include/rogue.h"

void save_map(Position *position)
{
    char map_file_name[50] = "maps/";
    strcat(map_file_name, player->username);
    map_file = fopen(map_file_name, "w");

    fprintf(map_file, "%d\n", score_multiplier);
    fprintf(map_file, "%d %d\n", position->x, position->y);
    fprintf(map_file, "%d\n", FLOORS);
    for (int i = 0; i < FLOORS; i++)
    {
        Floor *floor = &floors[i];
        fprintf(map_file, "%d %d\n", floor->has_down_stair, floor->has_up_stair);
        fprintf(map_file, "%d %d\n", floor->down_stair.position.x, floor->down_stair.position.y);
        fprintf(map_file, "%d %d\n", floor->up_stair.position.x, floor->up_stair.position.y);
        fprintf(map_file, "%d\n", floor->rooms_count);
        for (int j = 0; j < floor->rooms_count; j++)
        {
            Room *room = floor->rooms[j];
            fprintf(map_file, "%d %d %d %d %d %d %d\n", room->position.x, room->position.y, room->block.x, room->block.y, room->width, room->height, room->type);
            for (int k = 0; k < 4; k++)
                fprintf(map_file, "%d %d %d %d\n", room->doors[k].position.x, room->doors[k].position.y, room->doors[k].exists, room->doors[k].secret);
        }
    }

    fprintf(map_file, "%d\n", traps_count);
    for (int i = 0; i < traps_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d\n", traps[i].position.x, traps[i].position.y, traps[i].floor_index, traps[i].room_index, traps[i].damage);
    }

    fprintf(map_file, "%d\n", golds_count);
    for (int i = 0; i < golds_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d\n", golds[i].position.x, golds[i].position.y, golds[i].floor_index, golds[i].room_index, golds[i].value);
    }

    fprintf(map_file, "%d\n", black_golds_count);
    for (int i = 0; i < black_golds_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d\n", black_golds[i].position.x, black_golds[i].position.y, black_golds[i].floor_index, black_golds[i].room_index, black_golds[i].value);
    }

    fprintf(map_file, "%d\n", foods_count);
    for (int i = 0; i < foods_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d\n", foods[i].position.x, foods[i].position.y, foods[i].floor_index, foods[i].room_index, foods[i].value, foods[i].type);
    }

    fprintf(map_file, "%d\n", weapons_count);
    for (int i = 0; i < weapons_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d %d\n", weapons[i].position.x, weapons[i].position.y, weapons[i].floor_index, weapons[i].room_index, weapons[i].damage, weapons[i].type, weapons[i].count);
    }

    fprintf(map_file, "%d\n", potions_count);
    for (int i = 0; i < potions_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d\n", potions[i].position.x, potions[i].position.y, potions[i].floor_index, potions[i].room_index, potions[i].type);
    }

    fprintf(map_file, "%d\n", daemons_count);
    for (int i = 0; i < daemons_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d\n", daemons[i].position.x, daemons[i].position.y, daemons[i].floor_index, daemons[i].room_index, daemons[i].damage, daemons[i].health);
    }

    fprintf(map_file, "%d\n", fire_monsters_count);
    for (int i = 0; i < fire_monsters_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d\n", fire_monsters[i].position.x, fire_monsters[i].position.y, fire_monsters[i].floor_index, fire_monsters[i].room_index, fire_monsters[i].damage, fire_monsters[i].health);
    }

    fprintf(map_file, "%d\n", snakes_count);
    for (int i = 0; i < snakes_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d\n", snakes[i].position.x, snakes[i].position.y, snakes[i].floor_index, snakes[i].room_index, snakes[i].damage, snakes[i].health);
    }

    fprintf(map_file, "%d\n", giants_count);
    for (int i = 0; i < giants_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d\n", giants[i].position.x, giants[i].position.y, giants[i].floor_index, giants[i].room_index, giants[i].damage, giants[i].health);
    }

    fprintf(map_file, "%d\n", undeeds_count);
    for (int i = 0; i < undeeds_count; i++)
    {
        fprintf(map_file, "%d %d %d %d %d %d\n", undeeds[i].position.x, undeeds[i].position.y, undeeds[i].floor_index, undeeds[i].room_index, undeeds[i].damage, undeeds[i].health);
    }

    fclose(map_file);
}

void load_map(Position *position)
{
    int floors_count;
    char map_file_name[50] = "maps/";
    strcat(map_file_name, player->username);
    map_file = fopen(map_file_name, "r");

    fscanf(map_file, "%d", &score_multiplier);
    fscanf(map_file, "%d %d", &position->x, &position->y);
    fscanf(map_file, "%d", &floors_count);
    for (int i = 0; i < FLOORS; i++)
    {
        Floor *floor = &floors[i];
        int has_down_stair, has_up_stair;
        fscanf(map_file, "%d %d", &has_down_stair, &has_up_stair);
        fscanf(map_file, "%d %d", &floor->down_stair.position.x, &floor->down_stair.position.y);
        fscanf(map_file, "%d %d", &floor->up_stair.position.x, &floor->up_stair.position.y);
        fscanf(map_file, "%d", &floor->rooms_count);

        floor->has_down_stair = (bool)has_down_stair;
        floor->has_up_stair = (bool)has_up_stair;

        floor->rooms = (Room **)calloc(floor->rooms_count, sizeof(Room *));

        for (int j = 0; j < floor->rooms_count; j++)
        {
            floor->rooms[j] = (Room *)calloc(1, sizeof(Room));
            Room *room = floor->rooms[j];
            // int visible;
            fscanf(map_file, "%d %d %d %d %d %d %d", &room->position.x, &room->position.y, &room->block.x, &room->block.y, &room->width, &room->height, &room->type);
            // room->visible = (bool)visible;

            for (int k = 0; k < 4; k++)
            {
                int exists, secret;
                fscanf(map_file, "%d %d %d %d", &room->doors[k].position.x, &room->doors[k].position.y, &exists, &secret);
                room->doors[k].exists = (bool)exists;
                room->doors[k].secret = (bool)secret;
                room->doors[k].is_discovered = false;
            }
        }

        floor->down_stair.room = floor->rooms[0];
        floor->up_stair.room = floor->rooms[floor->rooms_count - 1];
    }

    fscanf(map_file, "%d", &traps_count);
    traps = (Trap *)calloc(traps_count, sizeof(Trap));
    for (int i = 0; i < traps_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d", &traps[i].position.x, &traps[i].position.y, &traps[i].floor_index, &traps[i].room_index, &traps[i].damage);
        traps[i].floor = &floors[traps[i].floor_index];
        traps[i].room = traps[i].floor->rooms[traps[i].room_index];
        traps[i].is_discovered = false;
    }

    fscanf(map_file, "%d", &golds_count);
    golds = (Gold *)calloc(golds_count, sizeof(Gold));
    for (int i = 0; i < golds_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d", &golds[i].position.x, &golds[i].position.y, &golds[i].floor_index, &golds[i].room_index, &golds[i].value);
        golds[i].floor = &floors[golds[i].floor_index];
        golds[i].room = golds[i].floor->rooms[golds[i].room_index];
        golds[i].is_discovered = false;
    }

    fscanf(map_file, "%d", &black_golds_count);
    black_golds = (BlackGold *)calloc(black_golds_count, sizeof(BlackGold));
    for (int i = 0; i < black_golds_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d", &black_golds[i].position.x, &black_golds[i].position.y, &black_golds[i].floor_index, &black_golds[i].room_index, &black_golds[i].value);
        black_golds[i].floor = &floors[black_golds[i].floor_index];
        black_golds[i].room = black_golds[i].floor->rooms[black_golds[i].room_index];
        black_golds[i].is_discovered = false;
    }

    fscanf(map_file, "%d", &foods_count);
    foods = (Food *)calloc(foods_count, sizeof(Food));
    for (int i = 0; i < foods_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d", &foods[i].position.x, &foods[i].position.y, &foods[i].floor_index, &foods[i].room_index, &foods[i].value, &foods[i].type);
        foods[i].floor = &floors[foods[i].floor_index];
        foods[i].room = foods[i].floor->rooms[foods[i].room_index];
        foods[i].is_picked = false;
        foods[i].is_eaten = false;
    }

    fscanf(map_file, "%d", &weapons_count);
    weapons = (Weapon *)calloc(weapons_count * 20, sizeof(Weapon));
    for (int i = 0; i < weapons_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d %d", &weapons[i].position.x, &weapons[i].position.y, &weapons[i].floor_index, &weapons[i].room_index, &weapons[i].damage, &weapons[i].type, &weapons[i].count);
        weapons[i].floor = &floors[weapons[i].floor_index];
        weapons[i].room = weapons[i].floor->rooms[weapons[i].room_index];
        weapons[i].is_picked = false;
        weapons[i].in_hand = false;
    }
    weapons[0].is_picked = true;
    weapons[0].in_hand = true;

    fscanf(map_file, "%d", &potions_count);
    potions = (Potion *)calloc(potions_count, sizeof(Potion));
    for (int i = 0; i < potions_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d", &potions[i].position.x, &potions[i].position.y, &potions[i].floor_index, &potions[i].room_index, &potions[i].type);
        potions[i].floor = &floors[potions[i].floor_index];
        potions[i].room = potions[i].floor->rooms[potions[i].room_index];
        potions[i].is_picked = false;
        potions[i].is_being_consumed = false;
        potions[i].is_consumed = false;
    }

    fscanf(map_file, "%d", &daemons_count);
    daemons = (Daemon *)calloc(daemons_count, sizeof(Daemon));
    for (int i = 0; i < daemons_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d", &daemons[i].position.x, &daemons[i].position.y, &daemons[i].floor_index, &daemons[i].room_index, &daemons[i].damage, &daemons[i].health);
        daemons[i].floor = &floors[daemons[i].floor_index];
        daemons[i].room = daemons[i].floor->rooms[daemons[i].room_index];
        daemons[i].is_alive = true;
    }

    fscanf(map_file, "%d", &fire_monsters_count);
    fire_monsters = (FireMonster *)calloc(fire_monsters_count, sizeof(FireMonster));
    for (int i = 0; i < fire_monsters_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d", &fire_monsters[i].position.x, &fire_monsters[i].position.y, &fire_monsters[i].floor_index, &fire_monsters[i].room_index, &fire_monsters[i].damage, &fire_monsters[i].health);
        fire_monsters[i].floor = &floors[fire_monsters[i].floor_index];
        fire_monsters[i].room = fire_monsters[i].floor->rooms[fire_monsters[i].room_index];
        fire_monsters[i].is_alive = true;
    }

    fscanf(map_file, "%d", &snakes_count);
    snakes = (Snake *)calloc(snakes_count, sizeof(Snake));
    for (int i = 0; i < snakes_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d", &snakes[i].position.x, &snakes[i].position.y, &snakes[i].floor_index, &snakes[i].room_index, &snakes[i].damage, &snakes[i].health);
        snakes[i].floor = &floors[snakes[i].floor_index];
        snakes[i].room = snakes[i].floor->rooms[snakes[i].room_index];
        snakes[i].is_alive = true;
    }

    fscanf(map_file, "%d", &giants_count);
    giants = (Giant *)calloc(giants_count, sizeof(Giant));
    for (int i = 0; i < giants_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d", &giants[i].position.x, &giants[i].position.y, &giants[i].floor_index, &giants[i].room_index, &giants[i].damage, &giants[i].health);
        giants[i].floor = &floors[giants[i].floor_index];
        giants[i].room = giants[i].floor->rooms[giants[i].room_index];
        giants[i].is_alive = true;
        giants[i].is_chasing = false;
    }

    fscanf(map_file, "%d", &undeeds_count);
    undeeds = (Undeed *)calloc(undeeds_count, sizeof(Undeed));
    for (int i = 0; i < undeeds_count; i++)
    {
        fscanf(map_file, "%d %d %d %d %d %d", &undeeds[i].position.x, &undeeds[i].position.y, &undeeds[i].floor_index, &undeeds[i].room_index, &undeeds[i].damage, &undeeds[i].health);
        undeeds[i].floor = &floors[undeeds[i].floor_index];
        undeeds[i].room = undeeds[i].floor->rooms[undeeds[i].room_index];
        undeeds[i].is_alive = true;
        undeeds[i].is_chasing = false;
    }

    fclose(map_file);
}