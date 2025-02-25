#include "../include/rogue.h"

bool register_command(char *command, int num, ...)
{
    bool output;
    char message[500];
    if (speed_potion == 0)
        timeline_counter++;
    for (int i = 0; i < potions_count; i++)
        if (potions[i].type == SPEED_POTION && potions[i].is_being_consumed)
        {
            speed_potion++;
            if (speed_potion > 1)
                speed_potion = 0;
            if (timeline_counter - potions[i].start_time > 20)
            {
                speed_potion = 0;
                potions[i].is_being_consumed = false;
                potions[i].is_consumed = true;
                add_message("You're SPEED potion is expired!");
            }
        }
        else if (potions[i].type == HEALTH_POTION && potions[i].is_being_consumed)
        {
            if (timeline_counter - potions[i].start_time > 40)
            {
                health_potion = 0;
                potions[i].is_being_consumed = false;
                potions[i].is_consumed = true;
                add_message("You're HEALTH potion is expired!");
            }
        }
        else if (potions[i].type == DAMAGE_POTION && potions[i].is_being_consumed)
        {
            if (timeline_counter - potions[i].start_time > 40)
            {
                damage_potion = 0;
                potions[i].is_being_consumed = false;
                potions[i].is_consumed = true;
                add_message("You're DAMAGE potion is expired!");
            }
        }

    if (strcmp(command, "move") == 0)
    {
        va_list args;
        va_start(args, num);
        int direction = va_arg(args, int);
        fprintf(log_file, "move %d\n", direction);
        move_character(direction, message);
        // add_message(message);
        va_end(args);
        output = true;
    }
    if (strcmp(command, "ascend") == 0)
    {
        fprintf(log_file, "ascend\n");
        if (ascend_character(message))
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
        add_message(message);
    }
    if (strcmp(command, "descend") == 0)
    {
        fprintf(log_file, "descend\n");
        if (descend_character(message))
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
        add_message(message);
    }
    if (strcmp(command, "pick") == 0)
    {
        fprintf(log_file, "pick\n");
        pick_character(message);
        add_message(message);
        output = true;
    }
    if (strcmp(command, "eat") == 0)
    {
        va_list args;
        va_start(args, num);
        int index = va_arg(args, int);
        fprintf(log_file, "eat %d\n", index);
        eat_character(index, message);
        add_message(message);
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
        sprintf(message, "You're now holding a %s!", found_weapon);
        add_message(message);
        va_end(args);
    }

    if (strcmp(command, "potion") == 0)
    {
        va_list args;
        va_start(args, num);
        int index = va_arg(args, int);
        fprintf(log_file, "potion %d\n", index);
        for (int i = 0; i < potions_count; i++)
            if (potions[i].is_picked && potions[i].type == index && !potions[i].is_being_consumed && !potions[i].is_consumed)
            {
                potions[i].is_being_consumed = true;
                potions[i].start_time = timeline_counter;
                if (potions[i].type == HEALTH_POTION)
                    health_potion = 1;
                if (potions[i].type == SPEED_POTION)
                    speed_potion = 1;
                if (potions[i].type == DAMAGE_POTION)
                    damage_potion = 1;
                break;
            }
        char found_potion[20];
        if (index == HEALTH_POTION)
            strcpy(found_potion, "HEALTH");
        if (index == SPEED_POTION)
            strcpy(found_potion, "SPEED");
        if (index == DAMAGE_POTION)
            strcpy(found_potion, "DAMAGE");
        sprintf(message, "You're consuming a %s potion!", found_potion);
        add_message(message);
        va_end(args);
    }

    if (strcmp(command, "short-attack") == 0)
    {
        fprintf(log_file, "short-attack\n");
        short_attack_character();
        output = true;
    }

    if (strcmp(command, "long-attack") == 0)
    {
        va_list args;
        va_start(args, num);
        int direction = va_arg(args, int);
        fprintf(log_file, "long-attack %d\n", direction);
        long_attack_character(direction);
        va_end(args);
        output = true;
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
                sprintf(message, "You stepped on a trap; Health reduced by %d%%!", traps[i].damage);
                add_message(message);
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
                sprintf(message, "You found some gold; Added %d to your collection!", golds[i].value);
                add_message(message);
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
                sprintf(message, "You found some BLACK gold; Added %d to your collection!", black_golds[i].value);
                add_message(message);
                black_golds[i].is_discovered = true;
                character.gold += black_golds[i].value;
                character.score += black_golds[i].value * score_multiplier;
                character.under = ground_character;
            }
        }
    }

    if (speed_potion == 0)
    {
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
                else
                {
                    character.health -= daemons[i].damage;
                    sprintf(message, "A nearby Daemon damaged you by %d!", daemons[i].damage);
                    add_message(message);
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
                else
                {
                    character.health -= fire_monsters[i].damage;
                    sprintf(message, "A nearby Fire Breathing Monster damaged you by %d!", fire_monsters[i].damage);
                    add_message(message);
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
                else
                {
                    character.health -= snakes[i].damage;
                    sprintf(message, "A nearby Snake damaged you by %d!", snakes[i].damage);
                    add_message(message);
                }
            }
        }

        for (int i = 0; i < giants_count; i++)
        {
            if (get_container_room() == giants[i].room && giants[i].is_alive)
            {
                Position position = get_absolute_position(giants[i].room);
                Position position_copy = giants[i].position;
                position.x += giants[i].position.x;
                position.y += giants[i].position.y;
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
                    if (giants[i].is_chasing)
                    {
                        if (!exists_monster(giants[i].floor, giants[i].room, position_copy))
                        {
                            mvadd_wch(position.y, position.x, &giants[i].under);
                            mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &giants[i].under);
                            mvprintw(absolute_position_copy.y, absolute_position_copy.x, "G");
                            giants[i].position = position_copy;
                        }
                        if (timeline_counter - giants[i].chasing_start >= 5)
                            giants[i].is_chasing = false;
                    }
                }
                else
                {
                    if (giants[i].is_chasing)
                    {
                        giants[i].chasing_start = timeline_counter;
                        character.health -= giants[i].damage;
                        sprintf(message, "A nearby Giant damaged you by %d!", giants[i].damage);
                        add_message(message);
                    }
                    else
                    {
                        giants[i].is_chasing = true;
                        giants[i].chasing_start = timeline_counter;
                    }
                }
            }
        }

        for (int i = 0; i < undeeds_count; i++)
        {
            if (get_container_room() == undeeds[i].room && undeeds[i].is_alive)
            {
                Position position = get_absolute_position(undeeds[i].room);
                Position position_copy = undeeds[i].position;
                position.x += undeeds[i].position.x;
                position.y += undeeds[i].position.y;
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
                    if (undeeds[i].is_chasing)
                    {
                        if (!exists_monster(undeeds[i].floor, undeeds[i].room, position_copy))
                        {
                            mvadd_wch(position.y, position.x, &undeeds[i].under);
                            mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &undeeds[i].under);
                            mvprintw(absolute_position_copy.y, absolute_position_copy.x, "U");
                            undeeds[i].position = position_copy;
                        }
                        if (timeline_counter - undeeds[i].chasing_start >= 5)
                            undeeds[i].is_chasing = false;
                    }
                }
                else
                {
                    if (undeeds[i].is_chasing)
                    {
                        undeeds[i].chasing_start = timeline_counter;
                        character.health -= undeeds[i].damage;
                        sprintf(message, "A nearby Undeed damaged you by %d!", undeeds[i].damage);
                        add_message(message);
                    }
                    else
                    {
                        undeeds[i].is_chasing = true;
                        undeeds[i].chasing_start = timeline_counter;
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
                character.health += health_potion * 3;
                character.stomach--;
            }
            if (character.stomach > 0 && timeline_counter % 5 == 0)
                character.stomach--;
        }
    }

    if (character.health <= 0)
    {
        output = false;
        current_window = DEFEAT;
    }

    setup_message_box(false);
    setup_sidebar(GUIDES);

    return output;
}

void replay_commands()
{
    char message[500];

    char file_name[50] = "logs/";
    strcat(file_name, player->username);
    log_file = fopen(file_name, "r");

    char line[100];
    while (fgets(line, 100, log_file) != NULL)
    {
        char command[50];
        sscanf(line, "%s", command);

        if (speed_potion == 0)
            timeline_counter++;
        for (int i = 0; i < potions_count; i++)
            if (potions[i].type == SPEED_POTION && potions[i].is_being_consumed)
            {
                speed_potion++;
                if (speed_potion > 1)
                    speed_potion = 0;
                if (timeline_counter - potions[i].start_time > 20)
                {
                    speed_potion = 0;
                    potions[i].is_being_consumed = false;
                    potions[i].is_consumed = true;
                    add_message("You're SPEED potion is expired!");
                }
            }
            else if (potions[i].type == HEALTH_POTION && potions[i].is_being_consumed)
            {
                if (timeline_counter - potions[i].start_time > 40)
                {
                    health_potion = 0;
                    potions[i].is_being_consumed = false;
                    potions[i].is_consumed = true;
                    add_message("You're HEALTH potion is expired!");
                }
            }
            else if (potions[i].type == DAMAGE_POTION && potions[i].is_being_consumed)
            {
                if (timeline_counter - potions[i].start_time > 40)
                {
                    damage_potion = 0;
                    potions[i].is_being_consumed = false;
                    potions[i].is_consumed = true;
                    add_message("You're DAMAGE potion is expired!");
                }
            }

        if (strcmp(command, "move") == 0)
        {
            int direction;
            sscanf(line, "move %d", &direction);
            move_character(direction, message);
            // add_message(message);
        }

        else if (strcmp(command, "ascend") == 0)
        {
            if (ascend_character(message))
            {
                setup_floor();
                Position position = get_absolute_position(floors[current_floor_index].down_stair.room);
                position.x += floors[current_floor_index].down_stair.position.x;
                position.y += floors[current_floor_index].down_stair.position.y;
                place_character(position);
            }
            add_message(message);
        }

        else if (strcmp(command, "descend") == 0)
        {
            if (descend_character(message))
            {
                setup_floor();
                Position position = get_absolute_position(floors[current_floor_index].up_stair.room);
                position.x += floors[current_floor_index].up_stair.position.x;
                position.y += floors[current_floor_index].up_stair.position.y;
                place_character(position);
            }
            add_message(message);
        }

        else if (strcmp(command, "pick") == 0)
        {
            pick_character(message);
            add_message(message);
        }

        else if (strcmp(command, "eat") == 0)
        {
            int index;
            sscanf(line, "eat %d", &index);
            eat_character(index, message);
            add_message(message);
        }

        else if (strcmp(command, "weapon") == 0)
        {
            int index;
            sscanf(line, "weapon %d", &index);
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
            sprintf(message, "You're now holding a %s!", found_weapon);
            add_message(message);
        }

        else if (strcmp(command, "potion") == 0)
        {
            int index;
            sscanf(line, "potion %d", &index);
            for (int i = 0; i < potions_count; i++)
                if (potions[i].is_picked && potions[i].type == index && !potions[i].is_being_consumed && !potions[i].is_consumed)
                {
                    potions[i].is_being_consumed = true;
                    potions[i].start_time = timeline_counter;
                    if (potions[i].type == HEALTH_POTION)
                        health_potion = 1;
                    if (potions[i].type == SPEED_POTION)
                        speed_potion = 1;
                    if (potions[i].type == DAMAGE_POTION)
                        damage_potion = 1;
                    break;
                }
            char found_potion[20];
            if (index == HEALTH_POTION)
                strcpy(found_potion, "HEALTH");
            if (index == SPEED_POTION)
                strcpy(found_potion, "SPEED");
            if (index == DAMAGE_POTION)
                strcpy(found_potion, "DAMAGE");
            sprintf(message, "You're consuming a %s potion!", found_potion);
            add_message(message);
        }

        else if (strcmp(command, "short-attack") == 0)
        {
            short_attack_character();
        }

        else if (strcmp(command, "long-attack") == 0)
        {
            int direction;
            sscanf(line, "long-attack %d", &direction);
            long_attack_character(direction);
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
                    sprintf(message, "You stepped on a trap; Health reduced by %d%%!", traps[i].damage);
                    add_message(message);
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
                    sprintf(message, "You found some gold; Added %d to your collection!", golds[i].value);
                    add_message(message);
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
                    sprintf(message, "You found some BLACK gold; Added %d to your collection!", black_golds[i].value);
                    add_message(message);
                    black_golds[i].is_discovered = true;
                    character.gold += black_golds[i].value;
                    character.score += black_golds[i].value * score_multiplier;
                    character.under = ground_character;
                }
            }
        }

        if (speed_potion == 0)
        {
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
                    else
                    {
                        character.health -= daemons[i].damage;
                        sprintf(message, "A nearby Daemon damaged you by %d!", daemons[i].damage);
                        add_message(message);
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
                    else
                    {
                        character.health -= fire_monsters[i].damage;
                        sprintf(message, "A nearby Fire Breathing Monster damaged you by %d!", fire_monsters[i].damage);
                        add_message(message);
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
                    else
                    {
                        character.health -= snakes[i].damage;
                        sprintf(message, "A nearby Snake damaged you by %d!", snakes[i].damage);
                        add_message(message);
                    }
                }
            }

            for (int i = 0; i < giants_count; i++)
            {
                if (get_container_room() == giants[i].room && giants[i].is_alive)
                {
                    Position position = get_absolute_position(giants[i].room);
                    Position position_copy = giants[i].position;
                    position.x += giants[i].position.x;
                    position.y += giants[i].position.y;
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
                        if (giants[i].is_chasing)
                        {
                            if (!exists_monster(giants[i].floor, giants[i].room, position_copy))
                            {
                                mvadd_wch(position.y, position.x, &giants[i].under);
                                mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &giants[i].under);
                                mvprintw(absolute_position_copy.y, absolute_position_copy.x, "G");
                                giants[i].position = position_copy;
                            }
                            if (timeline_counter - giants[i].chasing_start >= 5)
                                giants[i].is_chasing = false;
                        }
                    }
                    else
                    {
                        if (giants[i].is_chasing)
                        {
                            giants[i].chasing_start = timeline_counter;
                            character.health -= giants[i].damage;
                            sprintf(message, "A nearby Giant damaged you by %d!", giants[i].damage);
                            add_message(message);
                        }
                        else
                        {
                            giants[i].is_chasing = true;
                            giants[i].chasing_start = timeline_counter;
                        }
                    }
                }
            }

            for (int i = 0; i < undeeds_count; i++)
            {
                if (get_container_room() == undeeds[i].room && undeeds[i].is_alive)
                {
                    Position position = get_absolute_position(undeeds[i].room);
                    Position position_copy = undeeds[i].position;
                    position.x += undeeds[i].position.x;
                    position.y += undeeds[i].position.y;
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
                        if (undeeds[i].is_chasing)
                        {
                            if (!exists_monster(undeeds[i].floor, undeeds[i].room, position_copy))
                            {
                                mvadd_wch(position.y, position.x, &undeeds[i].under);
                                mvin_wch(absolute_position_copy.y, absolute_position_copy.x, &undeeds[i].under);
                                mvprintw(absolute_position_copy.y, absolute_position_copy.x, "U");
                                undeeds[i].position = position_copy;
                            }
                            if (timeline_counter - undeeds[i].chasing_start >= 5)
                                undeeds[i].is_chasing = false;
                        }
                    }
                    else
                    {
                        if (undeeds[i].is_chasing)
                        {
                            undeeds[i].chasing_start = timeline_counter;
                            character.health -= undeeds[i].damage;
                            sprintf(message, "A nearby Undeed damaged you by %d!", undeeds[i].damage);
                            add_message(message);
                        }
                        else
                        {
                            undeeds[i].is_chasing = true;
                            undeeds[i].chasing_start = timeline_counter;
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
                    character.health += health_potion * 3;
                    character.stomach--;
                }
                if (character.stomach > 0 && timeline_counter % 5 == 0)
                    character.stomach--;
            }
        }

        setup_message_box(true);
        setup_sidebar(GUIDES);
        refresh();
        usleep(5000);
    }

    fclose(log_file);
}

bool exists_monster(Floor *floor, Room *room, Position position)
{
    return exists_daemon(floor, room, position) || exists_fire_monster(floor, room, position) || exists_snake(floor, room, position) || exists_giant(floor, room, position) || exists_undeed(floor, room, position);
}