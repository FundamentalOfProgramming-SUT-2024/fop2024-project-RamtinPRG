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

    if (strcmp(command, "weapon") == 0)
    {
        va_list args;
        va_start(args, num);
        int index = va_arg(args, int);
        fprintf(log_file, "weapon %d\n", index);
        for (int i = 0; i < weapons_count; i++)
            if (weapons[i].in_hand)
            {
                weapons[i].in_hand = false;
                break;
            }
        for (int i = 0; i < weapons_count; i++)
            if (weapons[i].is_picked && weapons[i].type == index)
                weapons[i].in_hand = true;
        char found_weapon[20];
        if (index == MACE)
            strcpy(found_weapon, "MACE");
        if (index == DAGGER)
            strcpy(found_weapon, "DAGGER");
        if (index == WAND)
            strcpy(found_weapon, "WAND");
        if (index == ARROW)
            strcpy(found_weapon, "ARROW");
        if (index == SWORD)
            strcpy(found_weapon, "SWORD");
        sprintf(game_message, "You're now holding a %s!", found_weapon);
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

    for (int i = 0; i < daemons_count; i++)
    {
        if (get_container_room() == daemons[i].room && daemons[i].is_alive)
        {
            Position position = get_absolute_position(daemons[i].room);
            Position position_copy = daemons[i].position;
            position.x += daemons[i].position.x;
            position.y += daemons[i].position.y;
            Position absolute_position_copy = position;
            if (position.x < character.position.x)
            {
                absolute_position_copy.x++;
                position_copy.x++;
            }
            else if (position.x > character.position.x)
            {
                absolute_position_copy.x--;
                position_copy.x--;
            }
            if (position.y < character.position.y)
            {
                absolute_position_copy.y++;
                position_copy.y++;
            }
            else if (position.y > character.position.y)
            {
                absolute_position_copy.y--;
                position_copy.y--;
            }

            if (absolute_position_copy.x != character.position.x || absolute_position_copy.y != character.position.y)
            {
                if (!exists_monster(daemons[i].floor, daemons[i].room, position_copy))
                {
                    mvadd_wch(position.y, position.x, &daemons[i].under);
                    mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &daemons[i].under);
                    mvprintw(absolute_position_copy.y, absolute_position_copy.x, "D");
                    daemons[i].position = position_copy;
                }
            }
        }
    }

    for (int i = 0; i < fire_monsters_count; i++)
    {
        if (get_container_room() == fire_monsters[i].room && fire_monsters[i].is_alive)
        {
            Position position = get_absolute_position(fire_monsters[i].room);
            Position position_copy = fire_monsters[i].position;
            position.x += fire_monsters[i].position.x;
            position.y += fire_monsters[i].position.y;
            Position absolute_position_copy = position;
            if (position.x < character.position.x)
            {
                absolute_position_copy.x++;
                position_copy.x++;
            }
            else if (position.x > character.position.x)
            {
                absolute_position_copy.x--;
                position_copy.x--;
            }
            if (position.y < character.position.y)
            {
                absolute_position_copy.y++;
                position_copy.y++;
            }
            else if (position.y > character.position.y)
            {
                absolute_position_copy.y--;
                position_copy.y--;
            }

            if (absolute_position_copy.x != character.position.x || absolute_position_copy.y != character.position.y)
            {
                if (!exists_monster(fire_monsters[i].floor, fire_monsters[i].room, position_copy))
                {
                    mvadd_wch(position.y, position.x, &fire_monsters[i].under);
                    mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &fire_monsters[i].under);
                    mvprintw(absolute_position_copy.y, absolute_position_copy.x, "F");
                    fire_monsters[i].position = position_copy;
                }
            }
        }
    }

    for (int i = 0; i < snakes_count; i++)
    {
        if (get_container_room() == snakes[i].room && snakes[i].is_alive)
        {
            Position position = get_absolute_position(snakes[i].room);
            Position position_copy = snakes[i].position;
            position.x += snakes[i].position.x;
            position.y += snakes[i].position.y;
            Position absolute_position_copy = position;
            if (position.x < character.position.x)
            {
                absolute_position_copy.x++;
                position_copy.x++;
            }
            else if (position.x > character.position.x)
            {
                absolute_position_copy.x--;
                position_copy.x--;
            }
            if (position.y < character.position.y)
            {
                absolute_position_copy.y++;
                position_copy.y++;
            }
            else if (position.y > character.position.y)
            {
                absolute_position_copy.y--;
                position_copy.y--;
            }

            if (absolute_position_copy.x != character.position.x || absolute_position_copy.y != character.position.y)
            {
                if (!exists_monster(snakes[i].floor, snakes[i].room, position_copy))
                {
                    mvadd_wch(position.y, position.x, &snakes[i].under);
                    mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &snakes[i].under);
                    mvprintw(absolute_position_copy.y, absolute_position_copy.x, "S");
                    snakes[i].position = position_copy;
                }
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

        timeline_counter++;

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

        for (int i = 0; i < daemons_count; i++)
        {
            if (get_container_room() == daemons[i].room && daemons[i].is_alive)
            {
                Position position = get_absolute_position(daemons[i].room);
                Position position_copy = daemons[i].position;
                position.x += daemons[i].position.x;
                position.y += daemons[i].position.y;
                Position absolute_position_copy = position;
                if (position.x < character.position.x)
                {
                    absolute_position_copy.x++;
                    position_copy.x++;
                }
                else if (position.x > character.position.x)
                {
                    absolute_position_copy.x--;
                    position_copy.x--;
                }
                if (position.y < character.position.y)
                {
                    absolute_position_copy.y++;
                    position_copy.y++;
                }
                else if (position.y > character.position.y)
                {
                    absolute_position_copy.y--;
                    position_copy.y--;
                }

                if (absolute_position_copy.x != character.position.x || absolute_position_copy.y != character.position.y)
                {
                    if (!exists_monster(daemons[i].floor, daemons[i].room, position_copy))
                    {
                        mvadd_wch(position.y, position.x, &daemons[i].under);
                        mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &daemons[i].under);
                        mvprintw(absolute_position_copy.y, absolute_position_copy.x, "D");
                        daemons[i].position = position_copy;
                    }
                }
            }
        }

        for (int i = 0; i < fire_monsters_count; i++)
        {
            if (get_container_room() == fire_monsters[i].room && fire_monsters[i].is_alive)
            {
                Position position = get_absolute_position(fire_monsters[i].room);
                Position position_copy = fire_monsters[i].position;
                position.x += fire_monsters[i].position.x;
                position.y += fire_monsters[i].position.y;
                Position absolute_position_copy = position;
                if (position.x < character.position.x)
                {
                    absolute_position_copy.x++;
                    position_copy.x++;
                }
                else if (position.x > character.position.x)
                {
                    absolute_position_copy.x--;
                    position_copy.x--;
                }
                if (position.y < character.position.y)
                {
                    absolute_position_copy.y++;
                    position_copy.y++;
                }
                else if (position.y > character.position.y)
                {
                    absolute_position_copy.y--;
                    position_copy.y--;
                }

                if (absolute_position_copy.x != character.position.x || absolute_position_copy.y != character.position.y)
                {
                    if (!exists_monster(fire_monsters[i].floor, fire_monsters[i].room, position_copy))
                    {
                        mvadd_wch(position.y, position.x, &fire_monsters[i].under);
                        mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &fire_monsters[i].under);
                        mvprintw(absolute_position_copy.y, absolute_position_copy.x, "F");
                        fire_monsters[i].position = position_copy;
                    }
                }
            }
        }

        for (int i = 0; i < snakes_count; i++)
        {
            if (get_container_room() == snakes[i].room && snakes[i].is_alive)
            {
                Position position = get_absolute_position(snakes[i].room);
                Position position_copy = snakes[i].position;
                position.x += snakes[i].position.x;
                position.y += snakes[i].position.y;
                Position absolute_position_copy = position;
                if (position.x < character.position.x)
                {
                    absolute_position_copy.x++;
                    position_copy.x++;
                }
                else if (position.x > character.position.x)
                {
                    absolute_position_copy.x--;
                    position_copy.x--;
                }
                if (position.y < character.position.y)
                {
                    absolute_position_copy.y++;
                    position_copy.y++;
                }
                else if (position.y > character.position.y)
                {
                    absolute_position_copy.y--;
                    position_copy.y--;
                }

                if (absolute_position_copy.x != character.position.x || absolute_position_copy.y != character.position.y)
                {
                    if (!exists_monster(snakes[i].floor, snakes[i].room, position_copy))
                    {
                        mvadd_wch(position.y, position.x, &snakes[i].under);
                        mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &snakes[i].under);
                        mvprintw(absolute_position_copy.y, absolute_position_copy.x, "S");
                        snakes[i].position = position_copy;
                    }
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

        setup_message_box();
        setup_sidebar(GUIDES);
        refresh();
        usleep(5000);
    }

    fclose(log_file);
}

bool exists_monster(Floor *floor, Room *room, Position position)
{
    return exists_daemon(floor, room, position) || exists_fire_monster(floor, room, position) || exists_snake(floor, room, position);
}