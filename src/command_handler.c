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
            Position position = get_absolute_position(floors[current_floor_index].down_stair.room);
            position.x += floors[current_floor_index].down_stair.position.x;
            position.y += floors[current_floor_index].down_stair.position.y;
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
            Position position = get_absolute_position(floors[current_floor_index].up_stair.room);
            position.x += floors[current_floor_index].up_stair.position.x;
            position.y += floors[current_floor_index].up_stair.position.y;
            place_character(position);
            return true;
        }
        return false;
    }
}

void replay_commands()
{
    char file_name[50] = "logs/";
    strcat(file_name, player->username);
    log_file = fopen(file_name, "r");

    char line[100];
    while (fgets(line, 100, log_file) != NULL)
    {
        char command[50];
        sscanf(line, "%s", command);

        if (strcmp(command, "move") == 0)
        {
            int direction;
            sscanf(line, "move %d", &direction);
            move_character(direction);
        }
        
        else if (strcmp(command, "ascend") == 0)
        {
            if (ascend_character())
            {
                setup_floor();
                Position position = get_absolute_position(floors[current_floor_index].down_stair.room);
                position.x += floors[current_floor_index].down_stair.position.x;
                position.y += floors[current_floor_index].down_stair.position.y;
                place_character(position);
            }
        }

        else if (strcmp(command, "descend") == 0)
        {
            if (descend_character())
            {
                setup_floor();
                Position position = get_absolute_position(floors[current_floor_index].up_stair.room);
                position.x += floors[current_floor_index].up_stair.position.x;
                position.y += floors[current_floor_index].up_stair.position.y;
                place_character(position);
            }
        }

        refresh();
        usleep(50000);
    }

    fclose(log_file);
}