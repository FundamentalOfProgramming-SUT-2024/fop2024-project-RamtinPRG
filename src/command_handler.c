#include "../include/rogue.h"

bool register_command(char *command, int num, ...)
{
    bool output;

    timeline_counter++;

    if (strcmp(command, "move") == 0)
    {
        va_list args;
        va_start(args, num);
        int direction = va_arg(args, int);
        fprintf(log_file, "move %d\n", direction);
        move_character(direction, game_message);
        va_end(args);
        output = true;
    }
    if (strcmp(command, "ascend") == 0)
    {
        fprintf(log_file, "ascend\n");
        if (ascend_character(game_message))
        {
            if (current_floor_index == FLOORS)
            {
                output = false;
                current_window = VICTORY;
            }
            else
            {
                setup_floor();
                Position position = get_absolute_position(floors[current_floor_index].down_stair.room);
                position.x += floors[current_floor_index].down_stair.position.x;
                position.y += floors[current_floor_index].down_stair.position.y;
                place_character(position);
                output = true;
            }
        }
        else
            output = false;
    }
    if (strcmp(command, "descend") == 0)
    {
        fprintf(log_file, "descend\n");
        if (descend_character(game_message))
        {
            setup_floor();
            Position position = get_absolute_position(floors[current_floor_index].up_stair.room);
            position.x += floors[current_floor_index].up_stair.position.x;
            position.y += floors[current_floor_index].up_stair.position.y;
            place_character(position);
            output = true;
        }
        else
            output = false;
    }
    if (strcmp(command, "pick") == 0)
    {
        fprintf(log_file, "pick\n");
        pick_character(game_message);
        output = true;
    }
    if (strcmp(command, "eat") == 0)
    {
        va_list args;
        va_start(args, num);
        int index = va_arg(args, int);
        fprintf(log_file, "eat %d\n", index);
        eat_character(index, game_message);
        va_end(args);
    }

    for (int i = 0; i < traps_count; i++)
    {
        if (traps[i].floor == &floors[current_floor_index])
        {
            Position position = get_absolute_position(traps[i].room);
            position.x += traps[i].position.x;
            position.y += traps[i].position.y;
            if (character.position.x == position.x && character.position.y == position.y)
            {
                sprintf(game_message, "You stepped on a trap; Health reduced by %d%%!", traps[i].damage);
                traps[i].is_discovered = true;
                character.health -= traps[i].damage;
                character.under = trap_character;
            }
        }
    }

    for (int i = 0; i < golds_count; i++)
    {
        if (!golds[i].is_discovered && golds[i].floor == &floors[current_floor_index])
        {
            Position position = get_absolute_position(golds[i].room);
            position.x += golds[i].position.x;
            position.y += golds[i].position.y;
            if (character.position.x == position.x && character.position.y == position.y)
            {
                sprintf(game_message, "You found some gold; Added %d to your collection!", golds[i].value);
                golds[i].is_discovered = true;
                character.gold += golds[i].value;
                character.score += golds[i].value * score_multiplier;
                character.under = ground_character;
            }
        }
    }

    for (int i = 0; i < black_golds_count; i++)
    {
        if (!black_golds[i].is_discovered && black_golds[i].floor == &floors[current_floor_index])
        {
            Position position = get_absolute_position(black_golds[i].room);
            position.x += black_golds[i].position.x;
            position.y += black_golds[i].position.y;
            if (character.position.x == position.x && character.position.y == position.y)
            {
                sprintf(game_message, "You found some BLACK gold; Added %d to your collection!", black_golds[i].value);
                black_golds[i].is_discovered = true;
                character.gold += black_golds[i].value;
                character.score += black_golds[i].value * score_multiplier;
                character.under = ground_character;
            }
        }
    }

    if (strcmp(command, "eat") != 0)
    {
        if (character.stomach == 0 && timeline_counter % 3 == 0)
            character.health--;
        if (character.health < 100 && character.stomach > 0 && timeline_counter % 3 == 0)
        {
            character.health++;
            character.stomach--;
        }
        if (character.stomach > 0 && timeline_counter % 5 == 0)
            character.stomach--;
    }

    if (character.health <= 0)
    {
        output = false;
        current_window = DEFEAT;
    }

    setup_message_box();
    setup_sidebar(GUIDES);

    return output;
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
            move_character(direction, game_message);
        }

        else if (strcmp(command, "ascend") == 0)
        {
            if (ascend_character(game_message))
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
            if (descend_character(game_message))
            {
                setup_floor();
                Position position = get_absolute_position(floors[current_floor_index].up_stair.room);
                position.x += floors[current_floor_index].up_stair.position.x;
                position.y += floors[current_floor_index].up_stair.position.y;
                place_character(position);
            }
        }

        else if (strcmp(command, "pick") == 0)
        {
            pick_character(game_message);
        }

        else if (strcmp(command, "eat") == 0)
        {
            int index;
            sscanf(line, "eat %d", &index);
            eat_character(index, game_message);
        }

        for (int i = 0; i < traps_count; i++)
        {
            if (traps[i].floor == &floors[current_floor_index])
            {
                Position position = get_absolute_position(traps[i].room);
                position.x += traps[i].position.x;
                position.y += traps[i].position.y;
                if (character.position.x == position.x && character.position.y == position.y)
                {
                    sprintf(game_message, "You stepped on a trap; Health reduced by %d%%!", traps[i].damage);
                    traps[i].is_discovered = true;
                    character.health -= traps[i].damage;
                    character.under = trap_character;
                }
            }
        }

        for (int i = 0; i < golds_count; i++)
        {
            if (!golds[i].is_discovered && golds[i].floor == &floors[current_floor_index])
            {
                Position position = get_absolute_position(golds[i].room);
                position.x += golds[i].position.x;
                position.y += golds[i].position.y;
                if (character.position.x == position.x && character.position.y == position.y)
                {
                    sprintf(game_message, "You found some gold; Added %d to your collection!", golds[i].value);
                    golds[i].is_discovered = true;
                    character.gold += golds[i].value;
                    character.score += golds[i].value * score_multiplier;
                    character.under = ground_character;
                }
            }
        }

        for (int i = 0; i < black_golds_count; i++)
        {
            if (!black_golds[i].is_discovered && black_golds[i].floor == &floors[current_floor_index])
            {
                Position position = get_absolute_position(black_golds[i].room);
                position.x += black_golds[i].position.x;
                position.y += black_golds[i].position.y;
                if (character.position.x == position.x && character.position.y == position.y)
                {
                    sprintf(game_message, "You found some BLACK gold; Added %d to your collection!", black_golds[i].value);
                    black_golds[i].is_discovered = true;
                    character.gold += black_golds[i].value;
                    character.score += black_golds[i].value * score_multiplier;
                    character.under = ground_character;
                }
            }
        }

        if (strcmp(command, "eat") != 0 && character.stomach > 0 && timeline_counter % 5 == 0)
            character.stomach -= 1;

        setup_message_box();
        setup_sidebar(GUIDES);
        refresh();
        usleep(5000);
    }

    fclose(log_file);
}