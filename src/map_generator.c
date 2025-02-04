#include "../include/rogue.h"

Floor floors[FLOORS];
Trap *traps;
Gold *golds;
BlackGold *black_golds;
Food *foods;
Weapon *weapons;
Daemon *daemons;
FireMonster *fire_monsters;
Snake *snakes;
Giant *giants;
Undeed *undeeds;

int traps_count = 0;
int golds_count = 0;
int black_golds_count = 0;
int foods_count = 0;
int weapons_count = 0;
int daemons_count = 0;
int fire_monsters_count = 0;
int snakes_count = 0;
int giants_count = 0;
int undeeds_count = 0;

int traps_to_rooms_ratio;
int rooms_to_traps_ratio;

int foods_to_rooms_ratio;
int rooms_to_foods_ratio;

int weapons_to_rooms_ratio;
int rooms_to_weapons_ratio;

int daemons_to_rooms_ratio;
int rooms_to_daemons_ratio;

int fire_monsters_to_rooms_ratio;
int rooms_to_fire_monsters_ratio;

int snakes_to_rooms_ratio;
int rooms_to_snakes_ratio;

int giants_to_rooms_ratio;
int rooms_to_giants_ratio;

int undeeds_to_rooms_ratio;
int rooms_to_undeeds_ratio;

int score_multiplier;

void generate_map()
{
    if (settings->difficulty == EASY)
    {
        traps_to_rooms_ratio = 3;
        rooms_to_traps_ratio = 2;
    }
    if (settings->difficulty == MEDUIM)
    {
        traps_to_rooms_ratio = 6;
        rooms_to_traps_ratio = 2;
    }
    if (settings->difficulty == HARD)
    {
        traps_to_rooms_ratio = 9;
        rooms_to_traps_ratio = 2;
    }

    foods_to_rooms_ratio = 1;
    rooms_to_foods_ratio = 3;

    weapons_to_rooms_ratio = 1;
    rooms_to_weapons_ratio = 4;

    daemons_to_rooms_ratio = 1;
    rooms_to_daemons_ratio = 5;

    fire_monsters_to_rooms_ratio = 1;
    rooms_to_fire_monsters_ratio = 6;

    giants_to_rooms_ratio = 1;
    rooms_to_giants_ratio = 7;

    snakes_to_rooms_ratio = 1;
    rooms_to_snakes_ratio = 8;

    undeeds_to_rooms_ratio = 1;
    rooms_to_undeeds_ratio = 9;

    score_multiplier = settings->difficulty + 1;

    floors[0].has_down_stair = false;
    floors[0].has_up_stair = true;
    generate_floor(&floors[0], NULL);
    for (int i = 1; i < FLOORS; i++)
    {
        floors[i].has_down_stair = true;
        floors[i].has_up_stair = true;
        generate_floor(&floors[i], &floors[i - 1]);
    }
    floors[FLOORS - 1].rooms[floors[FLOORS - 1].rooms_count - 1]->type = TREASURE_ROOM;

    generate_traps();
    generate_golds();
    generate_black_golds();
    generate_foods();
    generate_weapons();
    generate_daemons();
    generate_fire_monsters();
    generate_snakes();
    generate_giants();
    generate_undeeds();
}

void generate_traps()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_traps_count = total_rooms_count * traps_to_rooms_ratio / rooms_to_traps_ratio;
    traps = (Trap *)calloc(temp_traps_count, sizeof(Trap));

    for (int i = 0; i < temp_traps_count; i++)
    {
        Trap trap;
        do
        {
            trap.floor_index = rand() % FLOORS;
            trap.floor = &floors[trap.floor_index];
            trap.room_index = rand() % trap.floor->rooms_count;
            trap.room = trap.floor->rooms[trap.room_index];
            trap.position.x = nrandom(1, trap.room->width);
            trap.position.y = nrandom(1, trap.room->height);
        } while (exists_trap(trap.floor, trap.room, trap.position) || exists_stair(trap.floor, trap.room, trap.position));
        trap.is_discovered = false;
        trap.damage = nrandom(MIN_TRAP_DAMAGE, MAX_TRAP_DAMAGE);
        traps[i] = trap;
        traps_count++;
    }
}

void generate_golds()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_golds_count = total_rooms_count * GOLDS_TO_ROOMS_RATIO / ROOMS_TO_GOLDS_RATIO;
    golds = (Gold *)calloc(temp_golds_count, sizeof(Gold));

    for (int i = 0; i < temp_golds_count; i++)
    {
        Gold gold;
        do
        {
            gold.floor_index = rand() % FLOORS;
            gold.floor = &floors[gold.floor_index];
            gold.room_index = rand() % gold.floor->rooms_count;
            gold.room = gold.floor->rooms[gold.room_index];
            gold.position.x = nrandom(1, gold.room->width);
            gold.position.y = nrandom(1, gold.room->height);
        } while (exists_gold(gold.floor, gold.room, gold.position) || exists_trap(gold.floor, gold.room, gold.position) || exists_stair(gold.floor, gold.room, gold.position));
        gold.is_discovered = false;
        gold.value = nrandom(MIN_GOLD_VALUE, MAX_GOLD_VALUE);
        golds[i] = gold;
        golds_count++;
    }
}

void generate_black_golds()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_black_golds_count = total_rooms_count * BLACK_GOLDS_TO_ROOMS_RATIO / ROOMS_TO_BLACK_GOLDS_RATIO;
    black_golds = (BlackGold *)calloc(temp_black_golds_count, sizeof(BlackGold));

    for (int i = 0; i < temp_black_golds_count; i++)
    {
        BlackGold gold;
        do
        {
            gold.floor_index = rand() % FLOORS;
            gold.floor = &floors[gold.floor_index];
            gold.room_index = rand() % gold.floor->rooms_count;
            gold.room = gold.floor->rooms[gold.room_index];
            gold.position.x = nrandom(1, gold.room->width);
            gold.position.y = nrandom(1, gold.room->height);
        } while (exists_black_gold(gold.floor, gold.room, gold.position) || exists_gold(gold.floor, gold.room, gold.position) || exists_trap(gold.floor, gold.room, gold.position) || exists_stair(gold.floor, gold.room, gold.position));
        gold.is_discovered = false;
        gold.value = nrandom(MIN_BLACK_GOLD_VALUE, MAX_BLACK_GOLD_VALUE);
        black_golds[i] = gold;
        black_golds_count++;
    }
}

void generate_foods()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_foods_count = total_rooms_count * foods_to_rooms_ratio / rooms_to_foods_ratio;
    foods = (Food *)calloc(temp_foods_count, sizeof(Food));

    for (int i = 0; i < temp_foods_count; i++)
    {
        Food food;
        do
        {
            food.floor_index = rand() % FLOORS;
            food.floor = &floors[food.floor_index];
            food.room_index = rand() % food.floor->rooms_count;
            food.room = food.floor->rooms[food.room_index];
            food.position.x = nrandom(1, food.room->width);
            food.position.y = nrandom(1, food.room->height);
        } while (exists_gold(food.floor, food.room, food.position) || exists_trap(food.floor, food.room, food.position) || exists_stair(food.floor, food.room, food.position) || exists_food(food.floor, food.room, food.position));
        food.is_picked = false;
        food.is_eaten = false;
        food.value = nrandom(MIN_FOOD_VALUE, MAX_FOOD_VALUE);
        foods[i] = food;
        foods_count++;
    }
}

void generate_weapons()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_weapons_count = total_rooms_count * weapons_to_rooms_ratio / rooms_to_weapons_ratio;
    weapons = (Weapon *)calloc(temp_weapons_count * 20, sizeof(Weapon));

    weapons[0].floor_index = 0;
    weapons[0].floor = &floors[weapons[0].floor_index];
    weapons[0].room_index = 0;
    weapons[0].room = weapons[0].floor->rooms[weapons[0].room_index];
    weapons[0].position.x = 0;
    weapons[0].position.y = 0;
    weapons[0].is_picked = true;
    weapons[0].in_hand = true;
    weapons[0].damage = 5;
    weapons[0].type = MACE;
    weapons[0].count = 1;
    weapons_count++;

    for (int i = 1; i < temp_weapons_count; i++)
    {
        Weapon weapon;
        do
        {
            weapon.floor_index = rand() % FLOORS;
            weapon.floor = &floors[weapon.floor_index];
            weapon.room_index = rand() % weapon.floor->rooms_count;
            weapon.room = weapon.floor->rooms[weapon.room_index];
            weapon.position.x = nrandom(1, weapon.room->width);
            weapon.position.y = nrandom(1, weapon.room->height);
        } while (exists_gold(weapon.floor, weapon.room, weapon.position) || exists_trap(weapon.floor, weapon.room, weapon.position) || exists_stair(weapon.floor, weapon.room, weapon.position) || exists_food(weapon.floor, weapon.room, weapon.position) || exists_weapon(weapon.floor, weapon.room, weapon.position));
        weapon.type = nrandom(1, 4);
        weapon.is_picked = false;
        weapon.in_hand = false;
        // weapon.damage = nrandom(MIN_GOLD_VALUE, MAX_GOLD_VALUE);
        // weapon.damage = 10;
        if (weapon.type == DAGGER)
        {
            weapon.damage = 12;
            weapon.count = 10;
        }
        if (weapon.type == WAND)
        {
            weapon.damage = 15;
            weapon.count = 8;
        }
        if (weapon.type == ARROW)
        {
            weapon.damage = 5;
            weapon.count = 20;
        }
        if (weapon.type == SWORD)
        {
            weapon.damage = 10;
            weapon.count = 1;
        }
        weapons[i] = weapon;
        weapons_count++;
    }
}

void generate_daemons()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_daemons_count = total_rooms_count * daemons_to_rooms_ratio / rooms_to_daemons_ratio;
    daemons = (Daemon *)calloc(temp_daemons_count, sizeof(Daemon));

    for (int i = 0; i < temp_daemons_count; i++)
    {
        Daemon daemon;
        do
        {
            daemon.floor_index = rand() % FLOORS;
            daemon.floor = &floors[daemon.floor_index];
            daemon.room_index = rand() % daemon.floor->rooms_count;
            daemon.room = daemon.floor->rooms[daemon.room_index];
            daemon.position.x = nrandom(1, daemon.room->width);
            daemon.position.y = nrandom(1, daemon.room->height);
        } while (exists_gold(daemon.floor, daemon.room, daemon.position) || exists_trap(daemon.floor, daemon.room, daemon.position) || exists_stair(daemon.floor, daemon.room, daemon.position) || exists_food(daemon.floor, daemon.room, daemon.position) || exists_daemon(daemon.floor, daemon.room, daemon.position));
        daemon.is_alive = true;
        daemon.health = 5;
        daemon.damage = 5;
        daemons[i] = daemon;
        daemons_count++;
    }
}

void generate_fire_monsters()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_fire_monsters_count = total_rooms_count * fire_monsters_to_rooms_ratio / rooms_to_fire_monsters_ratio;
    fire_monsters = (FireMonster *)calloc(temp_fire_monsters_count, sizeof(FireMonster));

    for (int i = 0; i < temp_fire_monsters_count; i++)
    {
        FireMonster fire_monster;
        do
        {
            fire_monster.floor_index = rand() % FLOORS;
            fire_monster.floor = &floors[fire_monster.floor_index];
            fire_monster.room_index = rand() % fire_monster.floor->rooms_count;
            fire_monster.room = fire_monster.floor->rooms[fire_monster.room_index];
            fire_monster.position.x = nrandom(1, fire_monster.room->width);
            fire_monster.position.y = nrandom(1, fire_monster.room->height);
        } while (exists_gold(fire_monster.floor, fire_monster.room, fire_monster.position) || exists_trap(fire_monster.floor, fire_monster.room, fire_monster.position) || exists_stair(fire_monster.floor, fire_monster.room, fire_monster.position) || exists_food(fire_monster.floor, fire_monster.room, fire_monster.position) || exists_daemon(fire_monster.floor, fire_monster.room, fire_monster.position) || exists_fire_monster(fire_monster.floor, fire_monster.room, fire_monster.position));
        fire_monster.is_alive = true;
        fire_monster.health = 10;
        fire_monster.damage = 10;
        fire_monsters[i] = fire_monster;
        fire_monsters_count++;
    }
}

void generate_snakes()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_snakes_count = total_rooms_count * snakes_to_rooms_ratio / rooms_to_snakes_ratio;
    snakes = (Snake *)calloc(temp_snakes_count, sizeof(Snake));

    for (int i = 0; i < temp_snakes_count; i++)
    {
        Snake snake;
        do
        {
            snake.floor_index = rand() % FLOORS;
            snake.floor = &floors[snake.floor_index];
            snake.room_index = rand() % snake.floor->rooms_count;
            snake.room = snake.floor->rooms[snake.room_index];
            snake.position.x = nrandom(1, snake.room->width);
            snake.position.y = nrandom(1, snake.room->height);
        } while (exists_gold(snake.floor, snake.room, snake.position) || exists_trap(snake.floor, snake.room, snake.position) || exists_stair(snake.floor, snake.room, snake.position) || exists_food(snake.floor, snake.room, snake.position) || exists_daemon(snake.floor, snake.room, snake.position) || exists_fire_monster(snake.floor, snake.room, snake.position) || exists_snake(snake.floor, snake.room, snake.position));
        snake.is_alive = true;
        snake.health = 20;
        snake.damage = 20;
        snakes[i] = snake;
        snakes_count++;
    }
}

void generate_giants()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_giants_count = total_rooms_count * giants_to_rooms_ratio / rooms_to_giants_ratio;
    giants = (Giant *)calloc(temp_giants_count, sizeof(Giant));

    for (int i = 0; i < temp_giants_count; i++)
    {
        Giant giant;
        do
        {
            giant.floor_index = rand() % FLOORS;
            giant.floor = &floors[giant.floor_index];
            giant.room_index = rand() % giant.floor->rooms_count;
            giant.room = giant.floor->rooms[giant.room_index];
            giant.position.x = nrandom(1, giant.room->width);
            giant.position.y = nrandom(1, giant.room->height);
        } while (exists_gold(giant.floor, giant.room, giant.position) || exists_trap(giant.floor, giant.room, giant.position) || exists_stair(giant.floor, giant.room, giant.position) || exists_food(giant.floor, giant.room, giant.position) || exists_daemon(giant.floor, giant.room, giant.position) || exists_fire_monster(giant.floor, giant.room, giant.position) || exists_snake(giant.floor, giant.room, giant.position) || exists_giant(giant.floor, giant.room, giant.position));
        giant.is_alive = true;
        giant.is_chasing = false;
        giant.health = 15;
        giant.damage = 15;
        giants[i] = giant;
        giants_count++;
    }
}

void generate_undeeds()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_undeeds_count = total_rooms_count * undeeds_to_rooms_ratio / rooms_to_undeeds_ratio;
    undeeds = (Undeed *)calloc(temp_undeeds_count, sizeof(Undeed));

    for (int i = 0; i < temp_undeeds_count; i++)
    {
        Undeed undeed;
        do
        {
            undeed.floor_index = rand() % FLOORS;
            undeed.floor = &floors[undeed.floor_index];
            undeed.room_index = rand() % undeed.floor->rooms_count;
            undeed.room = undeed.floor->rooms[undeed.room_index];
            undeed.position.x = nrandom(1, undeed.room->width);
            undeed.position.y = nrandom(1, undeed.room->height);
        } while (exists_gold(undeed.floor, undeed.room, undeed.position) || exists_trap(undeed.floor, undeed.room, undeed.position) || exists_stair(undeed.floor, undeed.room, undeed.position) || exists_food(undeed.floor, undeed.room, undeed.position) || exists_daemon(undeed.floor, undeed.room, undeed.position) || exists_fire_monster(undeed.floor, undeed.room, undeed.position) || exists_snake(undeed.floor, undeed.room, undeed.position) || exists_giant(undeed.floor, undeed.room, undeed.position) || exists_undeed(undeed.floor, undeed.room, undeed.position));
        undeed.is_alive = true;
        undeed.is_chasing = false;
        undeed.health = 30;
        undeed.damage = 30;
        undeeds[i] = undeed;
        undeeds_count++;
    }
}

bool exists_trap(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < traps_count; i++)
    {
        if (traps[i].floor == floor && traps[i].room == room && traps[i].position.x == position.x && traps[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_stair(Floor *floor, Room *room, Position position)
{
    if (floor->down_stair.room == room && floor->down_stair.position.x == position.x && floor->down_stair.position.y == position.y)
        return true;
    if (floor->up_stair.room == room && floor->up_stair.position.x == position.x && floor->up_stair.position.y == position.y)
        return true;
    return false;
}

bool exists_gold(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < golds_count; i++)
    {
        if (golds[i].floor == floor && golds[i].room == room && golds[i].position.x == position.x && golds[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_black_gold(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < black_golds_count; i++)
    {
        if (black_golds[i].floor == floor && black_golds[i].room == room && black_golds[i].position.x == position.x && black_golds[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_food(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < foods_count; i++)
    {
        if (foods[i].floor == floor && foods[i].room == room && foods[i].position.x == position.x && foods[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_weapon(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < weapons_count; i++)
    {
        if (weapons[i].floor == floor && weapons[i].room == room && weapons[i].position.x == position.x && weapons[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_daemon(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < daemons_count; i++)
    {
        if (daemons[i].is_alive && daemons[i].floor == floor && daemons[i].room == room && daemons[i].position.x == position.x && daemons[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_fire_monster(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < fire_monsters_count; i++)
    {
        if (fire_monsters[i].is_alive && fire_monsters[i].floor == floor && fire_monsters[i].room == room && fire_monsters[i].position.x == position.x && fire_monsters[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_snake(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < snakes_count; i++)
    {
        if (snakes[i].is_alive && snakes[i].floor == floor && snakes[i].room == room && snakes[i].position.x == position.x && snakes[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_giant(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < giants_count; i++)
    {
        if (giants[i].is_alive && giants[i].floor == floor && giants[i].room == room && giants[i].position.x == position.x && giants[i].position.y == position.y)
            return true;
    }
    return false;
}

bool exists_undeed(Floor *floor, Room *room, Position position)
{
    for (int i = 0; i < undeeds_count; i++)
    {
        if (undeeds[i].is_alive && undeeds[i].floor == floor && undeeds[i].room == room && undeeds[i].position.x == position.x && undeeds[i].position.y == position.y)
            return true;
    }
    return false;
}

void generate_floor(Floor *floor, Floor *prev_floor)
{
    floor->rooms_count = nrandom(MIN_ROOMS, MAX_ROOMS);
    floor->rooms = (Room **)calloc(floor->rooms_count, sizeof(Room *));
    Room **rooms = floor->rooms;
    int rooms_count = floor->rooms_count;
    for (int i = 0; i < rooms_count; i++)
        rooms[i] = NULL;

    rooms[0] = (Room *)calloc(1, sizeof(Room));
    if (prev_floor == NULL)
    {
        int y = rand() % MAP_HEIGHT, x = rand() % MAP_WIDTH;
        rooms[0]->block.x = x;
        rooms[0]->block.y = y;
        rooms[0]->width = nrandom(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
        rooms[0]->height = nrandom(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
        rooms[0]->position.x = nrandom(0, MAX_ROOM_WIDTH - rooms[0]->width);
        rooms[0]->position.y = nrandom(0, MAX_ROOM_HEIGHT - rooms[0]->height);
        rooms[0]->visible = false;
        rooms[0]->type = REGULAR_ROOM;
    }
    else
    {
        *rooms[0] = *prev_floor->up_stair.room;
        for (int i = 0; i < 4; i++)
            rooms[0]->doors[i].exists = false;
        floor->down_stair.room = rooms[0];
        floor->down_stair.position = prev_floor->up_stair.position;
    }

    for (int i = 1; i < rooms_count; i++)
    {
        int total, room_index;
        Position blocks[4];
        do
        {
            room_index = rand() % i;
            total = empty_adjacent_blocks(floor, rooms[room_index], blocks);
        } while (total == 0);
        int block_index = rand() % total;
        rooms[i] = (Room *)calloc(1, sizeof(Room));
        rooms[i]->block = blocks[block_index];

        rooms[i]->width = nrandom(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
        rooms[i]->height = nrandom(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
        rooms[i]->position.x = nrandom(0, MAX_ROOM_WIDTH - rooms[i]->width);
        rooms[i]->position.y = nrandom(0, MAX_ROOM_HEIGHT - rooms[i]->height);
        rooms[i]->visible = false;
        rooms[i]->type = REGULAR_ROOM;

        int direction = adjacent_room_direction(rooms[i], rooms[room_index]);
        rooms[i]->doors[direction].exists = true;
        if (direction == 0)
        {
            rooms[i]->doors[direction].position.x = rooms[i]->width + 1;
            rooms[i]->doors[direction].position.y = nrandom(1, rooms[i]->height);
        }
        if (direction == 1)
        {
            rooms[i]->doors[direction].position.x = nrandom(1, rooms[i]->width);
            rooms[i]->doors[direction].position.y = 0;
        }
        if (direction == 2)
        {
            rooms[i]->doors[direction].position.x = 0;
            rooms[i]->doors[direction].position.y = nrandom(1, rooms[i]->height);
        }
        if (direction == 3)
        {
            rooms[i]->doors[direction].position.x = nrandom(1, rooms[i]->width);
            rooms[i]->doors[direction].position.y = rooms[i]->height + 1;
        }

        direction = (direction + 2) % 4;
        rooms[room_index]->doors[direction].exists = true;
        if (direction == 0)
        {
            rooms[room_index]->doors[direction].position.x = rooms[room_index]->width + 1;
            rooms[room_index]->doors[direction].position.y = nrandom(1, rooms[room_index]->height);
        }
        if (direction == 1)
        {
            rooms[room_index]->doors[direction].position.x = nrandom(1, rooms[room_index]->width);
            rooms[room_index]->doors[direction].position.y = 0;
        }
        if (direction == 2)
        {
            rooms[room_index]->doors[direction].position.x = 0;
            rooms[room_index]->doors[direction].position.y = nrandom(1, rooms[room_index]->height);
        }
        if (direction == 3)
        {
            rooms[room_index]->doors[direction].position.x = nrandom(1, rooms[room_index]->width);
            rooms[room_index]->doors[direction].position.y = rooms[room_index]->height + 1;
        }
    }

    if (floor->has_up_stair)
    {
        int stair_room_index = rooms_count - 1;
        Room *stair_room;
        stair_room = rooms[stair_room_index];
        floor->up_stair.room = stair_room;
        floor->up_stair.position.x = nrandom(1, stair_room->width);
        floor->up_stair.position.y = nrandom(1, stair_room->height);
    }
}

bool exists_room(Floor *floor, int y, int x)
{
    for (int i = 0; i < floor->rooms_count; i++)
        if (floor->rooms[i] != NULL && floor->rooms[i]->block.x == x && floor->rooms[i]->block.y == y)
            return true;
    return false;
}

Room *get_room(Floor *floor, int y, int x)
{
    for (int i = 0; i < floor->rooms_count; i++)
        if (floor->rooms[i] != NULL && floor->rooms[i]->block.x == x && floor->rooms[i]->block.y == y)
            return floor->rooms[i];
    return NULL;
}

int empty_adjacent_blocks(Floor *floor, Room *room, Position blocks[4])
{
    int total = 0;
    if (room->block.x + 1 < MAP_WIDTH && !exists_room(floor, room->block.y, room->block.x + 1))
    {
        blocks[total].x = room->block.x + 1;
        blocks[total].y = room->block.y;
        total++;
    }
    if (room->block.y - 1 >= 0 && !exists_room(floor, room->block.y - 1, room->block.x))
    {
        blocks[total].x = room->block.x;
        blocks[total].y = room->block.y - 1;
        total++;
    }
    if (room->block.x - 1 >= 0 && !exists_room(floor, room->block.y, room->block.x - 1))
    {
        blocks[total].x = room->block.x - 1;
        blocks[total].y = room->block.y;
        total++;
    }
    if (room->block.y + 1 < MAP_HEIGHT && !exists_room(floor, room->block.y + 1, room->block.x))
    {
        blocks[total].x = room->block.x;
        blocks[total].y = room->block.y + 1;
        total++;
    }
    return total;
}

int adjacent_room_direction(Room *a, Room *b)
{
    if (a->block.x + 1 == b->block.x)
        return 0;
    if (a->block.y - 1 == b->block.y)
        return 1;
    if (a->block.x - 1 == b->block.x)
        return 2;
    if (a->block.y + 1 == b->block.y)
        return 3;
}