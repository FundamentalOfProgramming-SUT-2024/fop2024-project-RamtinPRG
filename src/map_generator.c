#include "../include/rogue.h"

Room **rooms;
int rooms_count;

void generate_map()
{
    rooms_count = nrandom(MIN_ROOMS, MAX_ROOMS);
    rooms = (Room **)malloc(sizeof(Room *) * rooms_count);
    for (int i = 0; i < rooms_count; i++)
        rooms[i] = NULL;

    int y = rand() % MAP_HEIGHT, x = rand() % MAP_WIDTH;
    rooms[0] = (Room *)malloc(sizeof(Room));
    rooms[0]->block.x = x;
    rooms[0]->block.y = y;
    rooms[0]->width = nrandom(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
    rooms[0]->height = nrandom(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
    rooms[0]->position.x = nrandom(0, MAX_ROOM_WIDTH - rooms[0]->width);
    rooms[0]->position.y = nrandom(0, MAX_ROOM_HEIGHT - rooms[0]->height);

    for (int i = 1; i < rooms_count; i++)
    {
        int total, room_index;
        Position blocks[4];
        do
        {
            room_index = rand() % i;
            total = empty_adjacent_blocks(rooms[room_index], blocks);
        } while (total == 0);
        int block_index = rand() % total;
        rooms[i] = (Room *)malloc(sizeof(Room));
        rooms[i]->block = blocks[block_index];

        rooms[i]->width = nrandom(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
        rooms[i]->height = nrandom(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
        rooms[i]->position.x = nrandom(0, MAX_ROOM_WIDTH - rooms[i]->width);
        rooms[i]->position.y = nrandom(0, MAX_ROOM_HEIGHT - rooms[i]->height);

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
}

bool exists_room(int y, int x)
{
    for (int i = 0; i < rooms_count; i++)
        if (rooms[i] != NULL && rooms[i]->block.x == x && rooms[i]->block.y == y)
            return true;
    return false;
}

Room *get_room(int y, int x)
{
    for (int i = 0; i < rooms_count; i++)
        if (rooms[i] != NULL && rooms[i]->block.x == x && rooms[i]->block.y == y)
            return rooms[i];
    return NULL;
}

int empty_adjacent_blocks(Room *room, Position blocks[4])
{
    int total = 0;
    if (room->block.x + 1 < MAP_WIDTH && !exists_room(room->block.y, room->block.x + 1))
    {
        blocks[total].x = room->block.x + 1;
        blocks[total].y = room->block.y;
        total++;
    }
    if (room->block.y - 1 >= 0 && !exists_room(room->block.y - 1, room->block.x))
    {
        blocks[total].x = room->block.x;
        blocks[total].y = room->block.y - 1;
        total++;
    }
    if (room->block.x - 1 >= 0 && !exists_room(room->block.y, room->block.x - 1))
    {
        blocks[total].x = room->block.x - 1;
        blocks[total].y = room->block.y;
        total++;
    }
    if (room->block.y + 1 < MAP_HEIGHT && !exists_room(room->block.y + 1, room->block.x))
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