#include "../include/rogue.h"

void save_map(Position *position)
{
    char map_file_name[50] = "maps/";
    strcat(map_file_name, player->username);
    map_file = fopen(map_file_name, "w");

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
            fprintf(map_file, "%d %d %d %d %d %d %d\n", room->position.x, room->position.y, room->block.x, room->block.y, room->width, room->height, room->visible);
            for (int k = 0; k < 4; k++)
                fprintf(map_file, "%d %d %d\n", room->doors[k].position.x, room->doors[k].position.y, room->doors[k].exists);
        }
    }

    fclose(map_file);
}

void load_map(Position *position)
{
    int floors_count;
    char map_file_name[50] = "maps/";
    strcat(map_file_name, player->username);
    map_file = fopen(map_file_name, "r");

    fscanf(map_file, "%d %d", &position->x, &position->y);
    fscanf(map_file, "%d", &floors_count);
    for (int i = 0; i < FLOORS; i++)
    {
        Floor *floor = &floors[i];
        fscanf(map_file, "%d %d", &floor->has_down_stair, &floor->has_up_stair);
        fscanf(map_file, "%d %d", &floor->down_stair.position.x, &floor->down_stair.position.y);
        fscanf(map_file, "%d %d", &floor->up_stair.position.x, &floor->up_stair.position.y);
        fscanf(map_file, "%d", &floor->rooms_count);

        floor->rooms = (Room **)calloc(floor->rooms_count, sizeof(Room *));

        for (int j = 0; j < floor->rooms_count; j++)
        {
            floor->rooms[j] = (Room *)calloc(1, sizeof(Room));
            Room *room = floor->rooms[j];
            fscanf(map_file, "%d %d %d %d %d %d %d", &room->position.x, &room->position.y, &room->block.x, &room->block.y, &room->width, &room->height, &room->visible);
            for (int k = 0; k < 4; k++)
                fscanf(map_file, "%d %d %d", &room->doors[k].position.x, &room->doors[k].position.y, &room->doors[k].exists);
        }

        floor->down_stair.room = floor->rooms[0];
        floor->up_stair.room = floor->rooms[floor->rooms_count - 1];
    }

    fclose(map_file);
}