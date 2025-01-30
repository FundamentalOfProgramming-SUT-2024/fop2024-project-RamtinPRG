#include "../include/rogue.h"

Floor floors[FLOORS];
Trap *traps;
int traps_count;

void generate_map()
{
    floors[0].has_down_stair = false;
    floors[0].has_up_stair = true;
    generate_floor(&floors[0], NULL);
    for (int i = 1; i < FLOORS - 1; i++)
    {
        floors[i].has_down_stair = true;
        floors[i].has_up_stair = true;
        generate_floor(&floors[i], &floors[i - 1]);
    }
    floors[FLOORS - 1].has_down_stair = true;
    floors[FLOORS - 1].has_up_stair = false;
    generate_floor(&floors[FLOORS - 1], &floors[FLOORS - 2]);
    floors[FLOORS - 1].rooms[floors[FLOORS - 1].rooms_count - 1]->type = TREASURE_ROOM;

    generate_traps();
}

void generate_traps()
{
    int total_rooms_count = 0;
    for (int i = 0; i < FLOORS; i++)
        total_rooms_count += floors[i].rooms_count;

    int temp_traps_count = total_rooms_count * TRAPS_TO_ROOMS_RATIO;
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
            trap.is_discovered = false;
        } while (exists_trap(trap.floor, trap.room, trap.position) || exists_trap(trap.floor, trap.room, trap.position));
        traps[i] = trap;
        traps_count++;
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