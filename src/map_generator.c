#include "../include/rogue.h"

Room **rooms;
int rooms_count;

bool exists_room(int y, int x);

void generate_map()
{
    rooms_count = nrandom(MIN_ROOMS, MAX_ROOMS);
    rooms = (Room **)malloc(sizeof(Room *) * rooms_count);
    for (int i = 0; i < rooms_count; i++)
        rooms[i] = NULL;

    for (int i = 0; i < rooms_count; i++)
    {
        int y = rand() % MAP_HEIGHT, x = rand() % MAP_WIDTH;
        while (exists_room(y, x))
        {
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;
        }
        rooms[i] = (Room *)malloc(sizeof(Room));
        rooms[i]->block.x = x;
        rooms[i]->block.y = y;
        rooms[i]->width = nrandom(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
        rooms[i]->height = nrandom(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
        rooms[i]->position.x = nrandom(0, MAX_ROOM_WIDTH - rooms[i]->width);
        rooms[i]->position.y = nrandom(0, MAX_ROOM_HEIGHT - rooms[i]->height);
    }
}

bool exists_room(int y, int x)
{
    for (int i = 0; i < rooms_count; i++)
        if (rooms[i] != NULL && rooms[i]->block.x == x && rooms[i]->block.y == y)
            return true;
    return false;
}