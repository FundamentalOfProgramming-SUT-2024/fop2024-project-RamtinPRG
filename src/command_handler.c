#include "../include/rogue.h"

bool register_command(char *command, int num, ...)
{
    if (strcmp(command, "move") == 0)
    {
        va_list args;
        va_start(args, num);
        int direction = va_arg(args, int);
        fprintf(log_file, "move %d\n", direction);
        move_character(direction);
        va_end(args);
        return true;
    }
    if (strcmp(command, "ascend") == 0)
    {
        fprintf(log_file, "ascend\n");
        if (ascend_character())
        {
            setup_floor();
            Position position = get_absolute_position(current_floor->down_stair.room);
            position.x += current_floor->down_stair.position.x;
            position.y += current_floor->down_stair.position.y;
            place_character(position);
            return true;
        }
        return false;
    }
    if (strcmp(command, "descend") == 0)
    {
        fprintf(log_file, "descend\n");
        if (descend_character())
        {
            setup_floor();
            Position position = get_absolute_position(current_floor->up_stair.room);
            position.x += current_floor->up_stair.position.x;
            position.y += current_floor->up_stair.position.y;
            place_character(position);
            return true;
        }
        return false;
    }
}