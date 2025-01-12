#include "../include/rogue.h"

void draw_room(Room *room);
void draw_rooms();

bool handle_game()
{
    erase();
    draw_rooms();
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
                printw(".");
            else if (i == 0 && j == 0)
                printw("┌");
            else if (i == 0 && j == width - 1)
                printw("┐");
            else if (i == height - 1 && j == width - 1)
                printw("┘");
            else if (i == height - 1 && j == 0)
                printw("└");
            else if (i == 0 || i == height - 1)
                printw("─");
            else if (j == 0 || j == width - 1)
                printw("│");
        }
    }
}

void draw_rooms()
{
    for (int i = 0; i < rooms_count; i++)
        draw_room(rooms[i]);
}