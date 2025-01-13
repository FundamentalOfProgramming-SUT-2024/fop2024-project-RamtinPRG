#include "../include/rogue.h"

void draw_room(Room *room);
void draw_rooms();
void draw_corridors();

bool handle_game()
{
    erase();
    draw_rooms();
    draw_corridors();
    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
            return false;
        else if (ch == 'r')
        {
            generate_map();
            erase();
            draw_rooms();
            draw_corridors();
        }
    }
    return false;
}

void draw_room(Room *room)
{
    int y, x, height = room->height + 2, width = room->width + 2;
    x = SCREEN_OFFSET + room->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + room->position.x;
    y = SCREEN_OFFSET + room->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + room->position.y;
    for (int i = 0; i < height; i++)
    {
        move(i + y, x);
        for (int j = 0; j < width; j++)
        {
            if (0 < i && i < height - 1 && 0 < j && j < width - 1)
            {
                attron(A_DIM);
                printw("•");
                attroff(A_DIM);
            }
            else if (i == 0 && j == 0)
            {
                attron(COLOR_PAIR(COLOR_WALL));
                printw("┌");
                attroff(COLOR_PAIR(COLOR_WALL));
            }
            else if (i == 0 && j == width - 1)
            {
                attron(COLOR_PAIR(COLOR_WALL));
                printw("┐");
                attroff(COLOR_PAIR(COLOR_WALL));
            }
            else if (i == height - 1 && j == width - 1)
            {
                attron(COLOR_PAIR(COLOR_WALL));
                printw("┘");
                attroff(COLOR_PAIR(COLOR_WALL));
            }
            else if (i == height - 1 && j == 0)
            {
                attron(COLOR_PAIR(COLOR_WALL));
                printw("└");
                attroff(COLOR_PAIR(COLOR_WALL));
            }
            else if (i == 0 || i == height - 1)
            {
                attron(COLOR_PAIR(COLOR_WALL));
                printw("─");
                attroff(COLOR_PAIR(COLOR_WALL));
            }
            else if (j == 0 || j == width - 1)
            {
                attron(COLOR_PAIR(COLOR_WALL));
                printw("│");
                attroff(COLOR_PAIR(COLOR_WALL));
            }
        }
    }

    attron(COLOR_PAIR(COLOR_DOOR));
    for (int i = 0; i < 4; i++)
        if (room->doors[i].exists)
            mvprintw(y + room->doors[i].position.y, x + room->doors[i].position.x, "+");
    attroff(COLOR_PAIR(COLOR_DOOR));
}

void draw_rooms()
{
    for (int i = 0; i < rooms_count; i++)
        draw_room(rooms[i]);
}

void draw_corridors()
{
    // attron(COLOR_PAIR(COLOR_DOOR));
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH - 1; j++)
        {
            Room *a = get_room(i, j);
            if (a != NULL && a->doors[0].exists)
            {
                Room *b = get_room(i, j + 1);
                Position door_a, door_b;
                door_a.x = SCREEN_OFFSET + a->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + a->position.x + a->doors[0].position.x + 1;
                door_a.y = SCREEN_OFFSET + a->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + a->position.y + a->doors[0].position.y;
                door_b.x = SCREEN_OFFSET + b->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + b->position.x + b->doors[2].position.x - 1;
                door_b.y = SCREEN_OFFSET + b->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + b->position.y + b->doors[2].position.y;
                int first_half = (door_b.x - door_a.x + 1) / 2;
                int second_half = door_b.x - door_a.x + 1 - first_half;
                move(door_a.y, door_a.x);
                for (int k = 0; k < first_half; k++)
                {
                    printw("░");
                }
                if (door_b.y > door_a.y)
                    for (int k = 0; k < door_b.y - door_a.y; k++)
                    {
                        mvprintw(door_a.y + k + 1, door_a.x + first_half - 1, "░");
                    }
                else if (door_b.y < door_a.y)
                    for (int k = 0; k < door_a.y - door_b.y; k++)
                    {
                        mvprintw(door_a.y - k - 1, door_a.x + first_half - 1, "░");
                    }
                move(door_b.y, door_a.x + first_half);
                for (int k = 0; k < second_half; k++)
                {
                    printw("░");
                }
            }
        }
    }

    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT - 1; j++)
        {
            Room *a = get_room(j, i);
            if (a != NULL && a->doors[3].exists)
            {
                Room *b = get_room(j + 1, i);
                Position door_a, door_b;
                door_a.x = SCREEN_OFFSET + a->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + a->position.x + a->doors[3].position.x;
                door_a.y = SCREEN_OFFSET + a->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + a->position.y + a->doors[3].position.y + 1;
                door_b.x = SCREEN_OFFSET + b->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + b->position.x + b->doors[1].position.x;
                door_b.y = SCREEN_OFFSET + b->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + b->position.y + b->doors[1].position.y - 1;
                int first_half = (door_b.y - door_a.y + 1) / 2;
                int second_half = door_b.y - door_a.y + 1 - first_half;
                for (int k = 0; k < first_half; k++)
                {
                    mvprintw(door_a.y + k, door_a.x, "░");
                }
                if (door_b.x > door_a.x)
                    for (int k = 0; k < door_b.x - door_a.x; k++)
                    {
                        mvprintw(door_a.y + first_half - 1, door_a.x + k + 1, "░");
                    }
                else if (door_b.x < door_a.x)
                    for (int k = 0; k < door_a.x - door_b.x; k++)
                    {
                        mvprintw(door_a.y + first_half - 1, door_a.x - k - 1, "░");
                    }
                for (int k = 0; k < second_half; k++)
                {
                    mvprintw(door_a.y + first_half + k, door_b.x, "░");
                }
            }
        }
    }
    // attroff(COLOR_PAIR(COLOR_DOOR));
}