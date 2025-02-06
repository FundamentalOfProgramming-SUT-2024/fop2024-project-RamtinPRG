#include "../include/rogue.h"

#define is_obstacle(c) (c == L'╭' || c == L'╮' || c == L'╰' || c == L'╯' || c == L'┌' || c == L'┐' || c == L'└' || c == L'┘' || c == L'│' || c == L'─' || c == L' ' || c == L'D' || c == L'F' || c == L'S' || c == L'G' || c == L'U')
#define is_corridor(c) (c == L'█' || c == L'▓' || c == L'▒' || c == L'░')
#define is_door(c) (c == L'?' || c == L'+' || c == L'┃' || c == L'━')

cchar_t trap_character = {0, {L'•'}, 4};
cchar_t ground_character = {A_DIM, {L'.'}, 0};

Character character;
Room *initial_room;
int current_floor_index;
FILE *log_file;
FILE *map_file;
char game_message[20][500];
int game_message_count = 0;
int timeline_counter = 0;
int speed_potion = 0;
int health_potion = 0;
int damage_potion = 0;
bool visible_cells[FLOORS][MAP_SCREEN_HEIGHT][MAP_SCREEN_WIDTH] = {0};
bool is_map_visible = false;

bool handle_game()
{
    Position position;
    char file_name[50] = "logs/";
    strcat(file_name, player->username);

    if (game_mode == NEW_GAME)
    {
        generate_map();

        position = get_absolute_position(floors[0].rooms[0]);
        position.x += rand() % floors[0].rooms[0]->width + 1;
        position.y += rand() % floors[0].rooms[0]->height + 1;

        save_map(&position);
    }
    else
    {
        load_map(&position);
    }

    timeline_counter = 0;
    speed_potion = 0;
    health_potion = 0;
    damage_potion = 0;
    is_map_visible = false;
    current_floor_index = 0;
    initial_room = floors[0].rooms[0];
    initial_room->visible = true;
    player->continuable = true;
    character.health = 100;
    character.stomach = 100;
    character.gold = 0;
    character.score = 0;
    // strcpy(game_message, "Welcome to the dungeons of DOOM!");
    add_message("Welcome to the dungeons of DOOM!");
    for (int i = 0; i < FLOORS; i++)
        for (int j = 0; j < MAP_SCREEN_HEIGHT; j++)
            for (int k = 0; k < MAP_SCREEN_WIDTH; k++)
                visible_cells[i][j][k] = false;
    setup_floor();
    place_character(position);
    refresh();
    if (game_mode == NEW_GAME)
        log_file = fopen(file_name, "w");
    else
    {
        replay_commands();
        log_file = fopen(file_name, "a");
    }

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            game_exit_routine();
            return false;
        }
        else if (ch == KEY_F(4))
        {
            current_window = MAIN_MENU;
            game_exit_routine();
            return true;
        }
        else if (ch == KEY_UP && current_floor_index < FLOORS - 1)
        {
            current_floor_index++;
            erase_scr();
            setup_floor();
        }
        else if (ch == KEY_DOWN && current_floor_index > 0)
        {
            current_floor_index--;
            setup_floor();
        }
        else if (ch == 'w')
            register_command("move", 1, N);
        else if (ch == 'W')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.y--;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, N);
                next = character.position;
                next.y--;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 'e')
            register_command("move", 1, NE);
        else if (ch == 'E')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.y--;
            next.x++;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, NE);
                next = character.position;
                next.y--;
                next.x++;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 'd')
            register_command("move", 1, E);
        else if (ch == 'D')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.x++;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, E);
                next = character.position;
                next.x++;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 'c')
            register_command("move", 1, SE);
        else if (ch == 'C')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.y++;
            next.x++;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, SE);
                next = character.position;
                next.y++;
                next.x++;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 's')
            register_command("move", 1, S);
        else if (ch == 'S')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.y++;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, S);
                next = character.position;
                next.y++;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 'z')
            register_command("move", 1, SW);
        else if (ch == 'Z')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.y++;
            next.x--;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, SW);
                next = character.position;
                next.y++;
                next.x--;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 'a')
            register_command("move", 1, W);
        else if (ch == 'A')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.x--;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, W);
                next = character.position;
                next.x--;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == 'q')
            register_command("move", 1, NW);
        else if (ch == 'Q')
        {
            cchar_t next_char;
            Room *room = get_current_room();
            Position abs_position = get_absolute_position(room);
            Position next = character.position;
            next.y--;
            next.x--;
            mvin_wch(next.y, next.x, &next_char);
            next.y -= abs_position.y;
            next.x -= abs_position.x;
            while (!exists_item(&floors[current_floor_index], room, next) && !is_obstacle(next_char.chars[0]))
            {
                register_command("move", 1, NW);
                next = character.position;
                next.y--;
                next.x--;
                mvin_wch(next.y, next.x, &next_char);
                next.y -= abs_position.y;
                next.x -= abs_position.x;
            }
        }
        else if (ch == '>')
            register_command("ascend", 0);
        else if (ch == '<')
            register_command("descend", 0);
        else if (ch == ('p' & 0x1F))
            register_command("pick", 0);
        else if (ch == ('e' & 0x1F))
            eat_food();
        else if (ch == ('w' & 0x1F))
            take_weapon();
        else if (ch == ('u' & 0x1F))
            use_potion();
        else if (ch == 't')
        {
            Weapon *weapon = get_current_weapon();
            if (weapon->type == MACE || weapon->type == SWORD)
                register_command("short-attack", 0);
            else
            {
                setup_sidebar(DIRECTIONS);
                while (1)
                {
                    ch = getch();
                    if ('a' <= ch && ch <= 'd')
                    {
                        register_command("long-attack", 1, ch - 'a');
                        break;
                    }
                    else if (ch == 'q')
                        break;
                }
                setup_sidebar(GUIDES);
            }
        }
        else if (ch == 'm')
        {
            is_map_visible = !is_map_visible;
            if (is_map_visible)
            {
                for (int i = 0; i < MAP_SCREEN_HEIGHT; i++)
                    for (int j = 0; j < MAP_SCREEN_WIDTH; j++)
                    {
                        cchar_t current;
                        mvin_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                        current.attr = current.attr & (~A_INVIS);
                        mvadd_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                    }
            }
            else
            {
                for (int i = 0; i < MAP_SCREEN_HEIGHT; i++)
                    for (int j = 0; j < MAP_SCREEN_WIDTH; j++)
                    {
                        if (visible_cells[current_floor_index][i][j])
                        {
                            cchar_t current;
                            mvin_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                            current.attr = current.attr & (~A_INVIS);
                            mvadd_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                        }
                        else
                        {
                            cchar_t current;
                            mvin_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                            current.attr = current.attr | A_INVIS;
                            mvadd_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                        }
                    }
            }
        }

        if (current_window != GAME)
        {
            game_exit_routine();
            return true;
        }
    }
    return false;
}

void game_exit_routine()
{
    fclose(log_file);
    for (int i = 0; i < FLOORS; i++)
    {
        for (int j = 0; j < floors[i].rooms_count; j++)
        {
            free(floors[i].rooms[j]);
        }
        free(floors[i].rooms);
        floors[i].up_stair.room = NULL;
        floors[i].down_stair.room = NULL;
    }
    free(traps);
    free(golds);
    free(black_golds);
    free(foods);
    free(weapons);
    free(potions);
    free(daemons);
    free(fire_monsters);
    free(snakes);
    free(giants);
    free(undeeds);
    traps_count = 0;
    golds_count = 0;
    black_golds_count = 0;
    foods_count = 0;
    weapons_count = 0;
    potions_count = 0;
    daemons_count = 0;
    fire_monsters_count = 0;
    snakes_count = 0;
    giants_count = 0;
    undeeds_count = 0;
}

void setup_floor()
{
    erase_scr();
    attron(A_INVIS);
    draw_rooms(&floors[current_floor_index]);
    draw_corridors(&floors[current_floor_index]);
    draw_stairs(&floors[current_floor_index]);
    draw_traps(&floors[current_floor_index]);
    draw_golds(&floors[current_floor_index]);
    draw_black_golds(&floors[current_floor_index]);
    draw_foods(&floors[current_floor_index]);
    draw_weapons(&floors[current_floor_index]);
    draw_potions(&floors[current_floor_index]);
    draw_daemons(&floors[current_floor_index]);
    draw_fire_monsters(&floors[current_floor_index]);
    draw_snakes(&floors[current_floor_index]);
    draw_giants(&floors[current_floor_index]);
    draw_undeeds(&floors[current_floor_index]);
    attroff(A_INVIS);
    for (int i = 0; i < MAP_SCREEN_HEIGHT; i++)
        for (int j = 0; j < MAP_SCREEN_WIDTH; j++)
        {
            if (visible_cells[current_floor_index][i][j])
            {
                cchar_t current;
                mvin_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
                current.attr = current.attr & (~A_INVIS);
                mvadd_wch(SCREEN_OFFSET + i, SCREEN_OFFSET + j, &current);
            }
        }
    for (int i = 0; i < floors[current_floor_index].rooms_count; i++)
    {
        if (floors[current_floor_index].rooms[i]->visible)
            show_room(floors[current_floor_index].rooms[i]);
    }
    setup_sidebar(GUIDES);
    setup_message_box(false);
}

void show_room(Room *room)
{
    Position position = get_absolute_position(room);
    int width = room->width;
    int height = room->height;
    for (int j = 0; j < height + 2; j++)
        for (int k = 0; k < width + 2; k++)
        {
            visible_cells[current_floor_index][position.y + j - SCREEN_OFFSET][position.x + k - SCREEN_OFFSET] = true;
            cchar_t current;
            mvin_wch(position.y + j, position.x + k, &current);
            current.attr = current.attr & (~A_INVIS);
            mvadd_wch(position.y + j, position.x + k, &current);
        }
}

void draw_stairs(Floor *floor)
{
    if (floor->has_down_stair)
    {
        Position position = get_absolute_position(floor->down_stair.room);
        position.x += floor->down_stair.position.x;
        position.y += floor->down_stair.position.y;
        mvadd_wch(position.y, position.x, &((cchar_t){0, {L'▼', 0}, CHAR_LIME}));
    }
    if (floor->has_up_stair)
    {
        Position position = get_absolute_position(floor->up_stair.room);
        position.x += floor->up_stair.position.x;
        position.y += floor->up_stair.position.y;
        mvadd_wch(position.y, position.x, &((cchar_t){0, {L'▲', 0}, CHAR_TEAL}));
    }
}

void draw_traps(Floor *floor)
{
    for (int i = 0; i < traps_count; i++)
    {
        if (traps[i].floor == floor && traps[i].is_discovered)
        {
            Position position = get_absolute_position(traps[i].room);
            position.x += traps[i].position.x;
            position.y += traps[i].position.y;
            attron(COLOR_PAIR(4));
            mvprintw(position.y, position.x, "•");
            attroff(COLOR_PAIR(4));
        }
    }
}

void draw_golds(Floor *floor)
{
    for (int i = 0; i < golds_count; i++)
    {
        if (golds[i].floor == floor && !golds[i].is_discovered)
        {
            Position position = get_absolute_position(golds[i].room);
            position.x += golds[i].position.x;
            position.y += golds[i].position.y;
            attron(COLOR_PAIR(CHAR_YELLOW));
            mvprintw(position.y, position.x, "$");
            attroff(COLOR_PAIR(CHAR_YELLOW));
        }
    }
}

void draw_black_golds(Floor *floor)
{
    for (int i = 0; i < black_golds_count; i++)
    {
        if (black_golds[i].floor == floor && !black_golds[i].is_discovered)
        {
            Position position = get_absolute_position(black_golds[i].room);
            position.x += black_golds[i].position.x;
            position.y += black_golds[i].position.y;
            attron(COLOR_PAIR(CHAR_SLATE));
            mvprintw(position.y, position.x, "♦");
            attroff(COLOR_PAIR(CHAR_SLATE));
        }
    }
}

void draw_foods(Floor *floor)
{
    for (int i = 0; i < foods_count; i++)
    {
        if (foods[i].floor == floor && !foods[i].is_picked)
        {
            Position position = get_absolute_position(foods[i].room);
            position.x += foods[i].position.x;
            position.y += foods[i].position.y;
            if (foods[i].type == REGULAR_FOOD || foods[i].type == ROTTEN_FOOD)
                attron(COLOR_PAIR(CHAR_BRONZE));
            else if (foods[i].type == SUPER_FOOD)
                attron(COLOR_PAIR(CHAR_YELLOW));
            else if (foods[i].type == MAGICAL_FOOD)
                attron(COLOR_PAIR(CHAR_VIOLET));

            mvprintw(position.y, position.x, "♥");

            if (foods[i].type == REGULAR_FOOD || foods[i].type == ROTTEN_FOOD)
                attroff(COLOR_PAIR(CHAR_BRONZE));
            else if (foods[i].type == SUPER_FOOD)
                attroff(COLOR_PAIR(CHAR_YELLOW));
            else if (foods[i].type == MAGICAL_FOOD)
                attroff(COLOR_PAIR(CHAR_VIOLET));
        }
    }
}

void draw_weapons(Floor *floor)
{
    for (int i = 0; i < weapons_count; i++)
    {
        if (weapons[i].floor == floor && !weapons[i].is_picked)
        {
            Position position = get_absolute_position(weapons[i].room);
            position.x += weapons[i].position.x;
            position.y += weapons[i].position.y;
            attron(COLOR_PAIR(CHAR_PINK));
            if (weapons[i].type == MACE)
                mvprintw(position.y, position.x, "₸");
            if (weapons[i].type == DAGGER)
                mvprintw(position.y, position.x, "𐃉");
            if (weapons[i].type == WAND)
                mvprintw(position.y, position.x, "/");
            if (weapons[i].type == ARROW)
                mvprintw(position.y, position.x, "↑");
            if (weapons[i].type == SWORD)
                mvprintw(position.y, position.x, "⁋");
            attroff(COLOR_PAIR(CHAR_PINK));
        }
    }
}

void draw_potions(Floor *floor)
{
    for (int i = 0; i < potions_count; i++)
    {
        if (potions[i].floor == floor && !potions[i].is_picked)
        {
            Position position = get_absolute_position(potions[i].room);
            position.x += potions[i].position.x;
            position.y += potions[i].position.y;
            if (potions[i].type == HEALTH_POTION)
                attron(COLOR_PAIR(CHAR_LIME));
            else if (potions[i].type == DAMAGE_POTION)
                attron(COLOR_PAIR(CHAR_BRONZE));
            else if (potions[i].type == SPEED_POTION)
                attron(COLOR_PAIR(CHAR_PINK));

            mvprintw(position.y, position.x, "⧗");

            if (potions[i].type == HEALTH_POTION)
                attroff(COLOR_PAIR(CHAR_LIME));
            else if (potions[i].type == DAMAGE_POTION)
                attroff(COLOR_PAIR(CHAR_BRONZE));
            else if (potions[i].type == SPEED_POTION)
                attroff(COLOR_PAIR(CHAR_PINK));
        }
    }
}

void draw_daemons(Floor *floor)
{
    for (int i = 0; i < daemons_count; i++)
    {
        if (daemons[i].floor == floor && daemons[i].is_alive)
        {
            Position position = get_absolute_position(daemons[i].room);
            position.x += daemons[i].position.x;
            position.y += daemons[i].position.y;
            mvin_wch(position.y, position.x, &daemons[i].under);
            if (daemons[i].under.attr & A_INVIS)
                daemons[i].under.attr &= (~A_INVIS);
            mvprintw(position.y, position.x, "D");
        }
    }
}

void draw_fire_monsters(Floor *floor)
{
    for (int i = 0; i < fire_monsters_count; i++)
    {
        if (fire_monsters[i].floor == floor && fire_monsters[i].is_alive)
        {
            Position position = get_absolute_position(fire_monsters[i].room);
            position.x += fire_monsters[i].position.x;
            position.y += fire_monsters[i].position.y;
            mvin_wch(position.y, position.x, &fire_monsters[i].under);
            if (fire_monsters[i].under.attr & A_INVIS)
                fire_monsters[i].under.attr &= (~A_INVIS);
            mvprintw(position.y, position.x, "F");
        }
    }
}

void draw_snakes(Floor *floor)
{
    for (int i = 0; i < snakes_count; i++)
    {
        if (snakes[i].floor == floor && snakes[i].is_alive)
        {
            Position position = get_absolute_position(snakes[i].room);
            position.x += snakes[i].position.x;
            position.y += snakes[i].position.y;
            mvin_wch(position.y, position.x, &snakes[i].under);
            if (snakes[i].under.attr & A_INVIS)
                snakes[i].under.attr &= (~A_INVIS);
            mvprintw(position.y, position.x, "S");
        }
    }
}

void draw_giants(Floor *floor)
{
    for (int i = 0; i < giants_count; i++)
    {
        if (giants[i].floor == floor && giants[i].is_alive)
        {
            Position position = get_absolute_position(giants[i].room);
            position.x += giants[i].position.x;
            position.y += giants[i].position.y;
            mvin_wch(position.y, position.x, &giants[i].under);
            if (giants[i].under.attr & A_INVIS)
                giants[i].under.attr &= (~A_INVIS);
            mvprintw(position.y, position.x, "G");
        }
    }
}

void draw_undeeds(Floor *floor)
{
    for (int i = 0; i < undeeds_count; i++)
    {
        if (undeeds[i].floor == floor && undeeds[i].is_alive)
        {
            Position position = get_absolute_position(undeeds[i].room);
            position.x += undeeds[i].position.x;
            position.y += undeeds[i].position.y;
            mvin_wch(position.y, position.x, &undeeds[i].under);
            if (undeeds[i].under.attr & A_INVIS)
                undeeds[i].under.attr &= (~A_INVIS);
            mvprintw(position.y, position.x, "U");
        }
    }
}

void remove_character()
{
    mvadd_wch(character.position.y, character.position.x, &character.under);
}

void place_character(Position position)
{
    character.position = position;
    mvin_wch(character.position.y, character.position.x, &character.under);
    mvaddch(character.position.y, character.position.x, '@' | COLOR_PAIR(color_settings->color_number[color_settings->current_color_index]) | A_BOLD | A_ITALIC);
}

void teleport_character(Position position)
{
    remove_character();
    place_character(position);
}

void move_character(int direction, char *message)
{
    Position position = character.position;
    switch (direction)
    {
    case N:
        position.y--;
        break;
    case NE:
        position.y--;
        position.x++;
        break;
    case E:
        position.x++;
        break;
    case SE:
        position.y++;
        position.x++;
        break;
    case S:
        position.y++;
        break;
    case SW:
        position.y++;
        position.x--;
        break;
    case W:
        position.x--;
        break;
    case NW:
        position.y--;
        position.x--;
        break;
    default:
        break;
    }
    cchar_t dest;
    mvin_wch(position.y, position.x, &dest);
    if (!is_obstacle(dest.chars[0]))
    {
        teleport_character(position);
        if (character.under.chars[0] == L'━' || character.under.chars[0] == L'┃')
        {
            character.under.chars[0] = '?';
            character.under.ext_color = COLOR_DOOR;
            Room *room = get_current_room();
            for (int i = 0; i < 4; i++)
            {
                if (room->doors[i].exists && room->doors[i].secret)
                {
                    Position position = get_absolute_position(room);
                    position.x += room->doors[i].position.x;
                    position.y += room->doors[i].position.y;
                    if (position.x == character.position.x && position.y == character.position.y)
                        room->doors[i].is_discovered = true;
                }
            }
        }
        if (is_corridor(character.under.chars[0]) || is_door(character.under.chars[0]))
        {
            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    if (i != 0 || j != 0)
                    {
                        cchar_t adjacent;
                        mvin_wch(character.position.y + i, character.position.x + j, &adjacent);
                        if (is_corridor(adjacent.chars[0]) || is_door(adjacent.chars[0]))
                        {
                            adjacent.attr = adjacent.attr & (~A_INVIS);
                            mvadd_wch(character.position.y + i, character.position.x + j, &adjacent);
                            visible_cells[current_floor_index][character.position.y + i - SCREEN_OFFSET][character.position.x + j - SCREEN_OFFSET] = true;
                        }
                    }
        }
        if (is_door(character.under.chars[0]))
        {
            Room *room = get_current_room();
            if (!room->visible)
            {
                room->visible = true;
                show_room(room);
            }
        }
        strcpy(message, "");
    }
}

bool ascend_character(char *message)
{
    if (character.under.chars[0] == L'▲')
    {
        current_floor_index++;
        if (current_floor_index < FLOORS && !floors[current_floor_index].rooms[0]->visible)
        {
            floors[current_floor_index].rooms[0]->visible = true;
            show_room(floors[current_floor_index].rooms[0]);
        }
        strcpy(message, "To the next floor!");
        return true;
    }
    strcpy(message, "I see no way up!");
    return false;
}

bool descend_character(char *message)
{
    if (character.under.chars[0] == L'▼')
    {
        current_floor_index--;
        strcpy(message, "To the previous floor!");
        return true;
    }
    strcpy(message, "I see no way down!");
    return false;
}

int inventory_foods_count()
{
    int count = 0;
    for (int i = 0; i < foods_count; i++)
        if (foods[i].is_picked && !foods[i].is_eaten)
            count++;
    return count;
}

int inventory_weapons_count()
{
    int existence[5] = {0, 0, 0, 0, 0};
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].is_picked)
            existence[weapons[i].type] = 1;
    return existence[0] + existence[1] + existence[2] + existence[3] + existence[4];
}

Food *food_inventory_by_index(int index)
{
    int count = 0;
    for (int i = 0; i < foods_count; i++)
    {
        if (foods[i].is_picked && !foods[i].is_eaten)
            count++;
        if (count - 1 == index)
        {
            return &foods[i];
        }
    }
    return NULL;
}

void pick_character(char *message)
{
    bool found = false;
    for (int i = 0; i < foods_count; i++)
    {
        if (!foods[i].is_picked && foods[i].floor == &floors[current_floor_index])
        {
            Position position = get_absolute_position(foods[i].room);
            position.x += foods[i].position.x;
            position.y += foods[i].position.y;
            if (character.position.x == position.x && character.position.y == position.y)
            {
                found = true;
                if (inventory_foods_count() < 5)
                {
                    sprintf(message, "You found some food; It's added to your food inventory!");
                    foods[i].is_picked = true;
                    character.under = ground_character;
                }
                else
                    sprintf(message, "You found some food but your inventory is already full!");
            }
        }
    }

    for (int i = 0; i < weapons_count; i++)
    {
        if (!weapons[i].is_picked && weapons[i].floor == &floors[current_floor_index])
        {
            Position position = get_absolute_position(weapons[i].room);
            position.x += weapons[i].position.x;
            position.y += weapons[i].position.y;
            if (character.position.x == position.x && character.position.y == position.y)
            {
                found = true;
                char found_weapon[20];
                if (weapons[i].type == MACE)
                    strcpy(found_weapon, "MACE");
                if (weapons[i].type == DAGGER)
                    strcpy(found_weapon, "DAGGER");
                if (weapons[i].type == WAND)
                    strcpy(found_weapon, "WAND");
                if (weapons[i].type == ARROW)
                    strcpy(found_weapon, "ARROW");
                if (weapons[i].type == SWORD)
                    strcpy(found_weapon, "SWORD");
                sprintf(message, "You found a %s; It's added to your weapon inventory!", found_weapon);
                weapons[i].is_picked = true;
                character.under = ground_character;
            }
        }
    }

    for (int i = 0; i < potions_count; i++)
    {
        if (!potions[i].is_picked && potions[i].floor == &floors[current_floor_index])
        {
            Position position = get_absolute_position(potions[i].room);
            position.x += potions[i].position.x;
            position.y += potions[i].position.y;
            if (character.position.x == position.x && character.position.y == position.y)
            {
                found = true;
                char found_potion[20];
                if (potions[i].type == HEALTH_POTION)
                    strcpy(found_potion, "HEALTH");
                if (potions[i].type == SPEED_POTION)
                    strcpy(found_potion, "SPEED");
                if (potions[i].type == WAND)
                    strcpy(found_potion, "DAMAGE");
                sprintf(message, "You found a %s; It's added to your potion inventory!", found_potion);
                potions[i].is_picked = true;
                character.under = ground_character;
            }
        }
    }
    if (!found)
        sprintf(message, "There's nothing to pick!");
}

void eat_character(int index, char *message)
{
    foods[index].is_picked = true;
    foods[index].is_eaten = true;
    character.stomach += foods[index].value;
    if (character.stomach > 100)
        character.stomach = 100;
    sprintf(message, "You consumed %d units of food!", foods[index].value);
}

void eat_food()
{
    setup_sidebar(FOODS);
    while (1)
    {
        ch = getch();
        if ('a' <= ch && ch <= inventory_foods_count() + 'a' - 1)
        {
            int index = ch - 'a';
            int count = 0;
            for (int i = 0; i < foods_count; i++)
            {
                if (foods[i].is_picked && !foods[i].is_eaten)
                    count++;
                if (count - 1 == index)
                {
                    register_command("eat", 1, i);
                    break;
                }
            }
            break;
        }
        else if (ch == 'q')
            break;
    }
    setup_sidebar(GUIDES);
}

void take_weapon()
{
    setup_sidebar(WEAPONS);
    while (1)
    {
        ch = getch();
        if ('a' <= ch && ch <= 'e')
        {
            int index = ch - 'a';
            bool found = false;
            for (int i = 0; i < weapons_count; i++)
                if (weapons[i].is_picked && weapons[i].type == index)
                {
                    register_command("weapon", 1, index);
                    found = true;
                    break;
                }
            if (found)
                break;
        }
        else if (ch == 'q')
            break;
    }
    setup_sidebar(GUIDES);
}

void use_potion()
{
    setup_sidebar(POTIONS);
    while (1)
    {
        ch = getch();
        if ('a' <= ch && ch <= 'c')
        {
            int index = ch - 'a';
            bool found = false;
            for (int i = 0; i < potions_count; i++)
                if (potions[i].is_picked && potions[i].type == index && !potions[i].is_consumed && !potions[i].is_being_consumed)
                {
                    register_command("potion", 1, index);
                    found = true;
                    break;
                }
            if (found)
                break;
        }
        else if (ch == 'q')
            break;
    }
    setup_sidebar(GUIDES);
}

void short_attack_character()
{
    char message[500];
    Position adjacent_cells[8];
    int index = 0;
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            if (i != 0 || j != 0)
            {
                adjacent_cells[index] = character.position;
                adjacent_cells[index].x += i;
                adjacent_cells[index].y += j;
                index++;
            }

    int damage = get_current_weapon()->damage * (damage_potion ? 2 : 1);

    for (int i = 0; i < daemons_count; i++)
    {
        if (daemons[i].is_alive && daemons[i].room == get_container_room())
        {
            Position position = get_absolute_position(daemons[i].room);
            position.x += daemons[i].position.x;
            position.y += daemons[i].position.y;
            for (int j = 0; j < 8; j++)
                if (adjacent_cells[j].x == position.x && adjacent_cells[j].y == position.y)
                {
                    daemons[i].health -= damage;
                    sprintf(message, "You damaged a nearby Daemon by %d!", damage);
                    add_message(message);
                    if (daemons[i].health <= 0)
                    {
                        character.score += daemons[i].damage * score_multiplier;
                        daemons[i].is_alive = false;
                        mvadd_wch(position.y, position.x, &daemons[i].under);
                        add_message("You killed a Daemon!");
                    }
                }
        }
    }

    for (int i = 0; i < fire_monsters_count; i++)
    {
        if (fire_monsters[i].is_alive && fire_monsters[i].room == get_container_room())
        {
            Position position = get_absolute_position(fire_monsters[i].room);
            position.x += fire_monsters[i].position.x;
            position.y += fire_monsters[i].position.y;
            for (int j = 0; j < 8; j++)
                if (adjacent_cells[j].x == position.x && adjacent_cells[j].y == position.y)
                {
                    fire_monsters[i].health -= damage;
                    sprintf(message, "You damaged a nearby Fire Breathing Monster by %d!", damage);
                    add_message(message);
                    if (fire_monsters[i].health <= 0)
                    {
                        character.score += fire_monsters[i].damage * score_multiplier;
                        fire_monsters[i].is_alive = false;
                        mvadd_wch(position.y, position.x, &fire_monsters[i].under);
                        add_message("You killed a Fire Breathing Monster!");
                    }
                }
        }
    }

    for (int i = 0; i < snakes_count; i++)
    {
        if (snakes[i].is_alive && snakes[i].room == get_container_room())
        {
            Position position = get_absolute_position(snakes[i].room);
            position.x += snakes[i].position.x;
            position.y += snakes[i].position.y;
            for (int j = 0; j < 8; j++)
                if (adjacent_cells[j].x == position.x && adjacent_cells[j].y == position.y)
                {
                    snakes[i].health -= damage;
                    sprintf(message, "You damaged a nearby Snake by %d!", damage);
                    add_message(message);
                    if (snakes[i].health <= 0)
                    {
                        character.score += snakes[i].damage * score_multiplier;
                        snakes[i].is_alive = false;
                        mvadd_wch(position.y, position.x, &snakes[i].under);
                        add_message("You killed a Snake!");
                    }
                }
        }
    }

    for (int i = 0; i < giants_count; i++)
    {
        if (giants[i].is_alive && giants[i].room == get_container_room())
        {
            Position position = get_absolute_position(giants[i].room);
            position.x += giants[i].position.x;
            position.y += giants[i].position.y;
            for (int j = 0; j < 8; j++)
                if (adjacent_cells[j].x == position.x && adjacent_cells[j].y == position.y)
                {
                    giants[i].health -= damage;
                    sprintf(message, "You damaged a nearby Giant by %d!", damage);
                    add_message(message);
                    if (giants[i].health <= 0)
                    {
                        character.score += giants[i].damage * score_multiplier;
                        giants[i].is_alive = false;
                        mvadd_wch(position.y, position.x, &giants[i].under);
                        add_message("You killed a Giant!");
                    }
                }
        }
    }

    for (int i = 0; i < undeeds_count; i++)
    {
        if (undeeds[i].is_alive && undeeds[i].room == get_container_room())
        {
            Position position = get_absolute_position(undeeds[i].room);
            position.x += undeeds[i].position.x;
            position.y += undeeds[i].position.y;
            for (int j = 0; j < 8; j++)
                if (adjacent_cells[j].x == position.x && adjacent_cells[j].y == position.y)
                {
                    undeeds[i].health -= damage;
                    sprintf(message, "You damaged a nearby Undeed by %d!", damage);
                    add_message(message);
                    if (undeeds[i].health <= 0)
                    {
                        character.score += undeeds[i].damage * score_multiplier;
                        undeeds[i].is_alive = false;
                        mvadd_wch(position.y, position.x, &undeeds[i].under);
                        add_message("You killed an Undeed!");
                    }
                }
        }
    }
}

Position next_projectile_position(Position position, int direction)
{
    if (direction == 0)
        position.x++;
    if (direction == 1)
        position.y--;
    if (direction == 2)
        position.x--;
    if (direction == 3)
        position.y++;
    return position;
}

Position previous_projectile_position(Position position, int direction)
{
    if (direction == 0)
        position.x--;
    if (direction == 1)
        position.y++;
    if (direction == 2)
        position.x++;
    if (direction == 3)
        position.y--;
    return position;
}

void long_attack_character(int direction)
{
    char message[500];
    Weapon *weapon = get_current_weapon();
    weapon->count--;
    cchar_t projectile_char;
    int trajectory = 0, max_trajectory;
    projectile_char.chars[1] = 0;
    projectile_char.ext_color = CHAR_PINK;
    projectile_char.attr = 0;
    if (weapon->type == DAGGER)
    {
        projectile_char.chars[0] = L'𐃉';
        max_trajectory = 5;
    }
    else if (weapon->type == WAND)
    {
        projectile_char.chars[0] = L'/';
        max_trajectory = 10;
    }
    else if (weapon->type == ARROW)
    {
        projectile_char.chars[0] = L'↑';
        max_trajectory = 5;
    }
    struct Projectile
    {
        Position position;
        cchar_t under;
    } projectile;
    projectile.position = next_projectile_position(character.position, direction);
    mvin_wch(projectile.position.y, projectile.position.x, &projectile.under);
    while (!is_obstacle(projectile.under.chars[0]) && trajectory < max_trajectory)
    {
        mvadd_wch(projectile.position.y, projectile.position.x, &projectile_char);
        refresh();
        usleep(50000);
        mvadd_wch(projectile.position.y, projectile.position.x, &projectile.under);
        projectile.position = next_projectile_position(projectile.position, direction);
        mvin_wch(projectile.position.y, projectile.position.x, &projectile.under);
        trajectory++;
    }

    int damage = weapon->damage * (damage_potion ? 2 : 1);

    for (int i = 0; i < daemons_count; i++)
    {
        if (daemons[i].is_alive && daemons[i].room == get_container_room())
        {
            Position position = get_absolute_position(daemons[i].room);
            position.x += daemons[i].position.x;
            position.y += daemons[i].position.y;
            if (projectile.position.x == position.x && projectile.position.y == position.y)
            {
                daemons[i].health -= damage;
                sprintf(message, "You damaged a Daemon by %d!", damage);
                add_message(message);
                if (daemons[i].health <= 0)
                {
                    character.score += daemons[i].damage * score_multiplier;
                    daemons[i].is_alive = false;
                    mvadd_wch(position.y, position.x, &daemons[i].under);
                    add_message("You killed a Daemon!");
                }
            }
        }
    }

    for (int i = 0; i < fire_monsters_count; i++)
    {
        if (fire_monsters[i].is_alive && fire_monsters[i].room == get_container_room())
        {
            Position position = get_absolute_position(fire_monsters[i].room);
            position.x += fire_monsters[i].position.x;
            position.y += fire_monsters[i].position.y;
            if (projectile.position.x == position.x && projectile.position.y == position.y)
            {
                fire_monsters[i].health -= damage;
                sprintf(message, "You damaged a Fire Breathing Monster by %d!", damage);
                add_message(message);
                if (fire_monsters[i].health <= 0)
                {
                    character.score += fire_monsters[i].damage * score_multiplier;
                    fire_monsters[i].is_alive = false;
                    mvadd_wch(position.y, position.x, &fire_monsters[i].under);
                    add_message("You killed a Fire Breathing Monster!");
                }
            }
        }
    }

    for (int i = 0; i < snakes_count; i++)
    {
        if (snakes[i].is_alive && snakes[i].room == get_container_room())
        {
            Position position = get_absolute_position(snakes[i].room);
            position.x += snakes[i].position.x;
            position.y += snakes[i].position.y;
            if (projectile.position.x == position.x && projectile.position.y == position.y)
            {
                snakes[i].health -= damage;
                sprintf(message, "You damaged a Snake by %d!", damage);
                add_message(message);
                if (snakes[i].health <= 0)
                {
                    character.score += snakes[i].damage * score_multiplier;
                    snakes[i].is_alive = false;
                    mvadd_wch(position.y, position.x, &snakes[i].under);
                    add_message("You killed a Snake!");
                }
            }
        }
    }

    for (int i = 0; i < giants_count; i++)
    {
        if (giants[i].is_alive && giants[i].room == get_container_room())
        {
            Position position = get_absolute_position(giants[i].room);
            position.x += giants[i].position.x;
            position.y += giants[i].position.y;
            if (projectile.position.x == position.x && projectile.position.y == position.y)
            {
                giants[i].health -= damage;
                sprintf(message, "You damaged a Giant by %d!", damage);
                add_message(message);
                if (giants[i].health <= 0)
                {
                    character.score += giants[i].damage * score_multiplier;
                    giants[i].is_alive = false;
                    mvadd_wch(position.y, position.x, &giants[i].under);
                    add_message("You killed a Giant!");
                }
            }
        }
    }

    for (int i = 0; i < undeeds_count; i++)
    {
        if (undeeds[i].is_alive && undeeds[i].room == get_container_room())
        {
            Position position = get_absolute_position(undeeds[i].room);
            position.x += undeeds[i].position.x;
            position.y += undeeds[i].position.y;
            if (projectile.position.x == position.x && projectile.position.y == position.y)
            {
                undeeds[i].health -= damage;
                sprintf(message, "You damaged a Undeed by %d!", damage);
                add_message(message);
                if (undeeds[i].health <= 0)
                {
                    character.score += undeeds[i].damage * score_multiplier;
                    undeeds[i].is_alive = false;
                    mvadd_wch(position.y, position.x, &undeeds[i].under);
                    add_message("You killed a Undeed!");
                }
            }
        }
    }

    Weapon *new = &weapons[weapons_count];
    new->count = 1;
    new->type = weapon->type;
    new->damage = weapon->damage;
    new->floor = &floors[current_floor_index];
    new->floor_index = current_floor_index;
    new->in_hand = false;
    new->is_picked = false;
    new->room = get_current_room();
    new->room_index = get_current_room_index();
    new->position = previous_projectile_position(projectile.position, direction);
    if (new->position.x == character.position.x &&new->position.y == character.position.y)
        character.under = projectile_char;
    else
        mvadd_wch(new->position.y, new->position.x, &projectile_char);
    Position abs_position = get_absolute_position(new->room);
    new->position.x -= abs_position.x;
    new->position.y -= abs_position.y;
    weapons_count++;

    refresh();
}

Room *get_current_room()
{
    Floor *floor = &floors[current_floor_index];
    for (int i = 0; i < floor->rooms_count; i++)
    {
        Room *room = floor->rooms[i];
        Position position = get_absolute_position(room);
        if (position.x <= character.position.x && character.position.x <= position.x + room->width + 1 && position.y <= character.position.y && character.position.y <= position.y + room->height + 1)
            return room;
    }
    return NULL;
}

int get_current_room_index()
{
    Room *room = get_current_room();
    for (int i = 0; i < floors[current_floor_index].rooms_count; i++)
        if (room == floors[current_floor_index].rooms[i])
            return i;
    return -1;
}

Room *get_container_room()
{
    Floor *floor = &floors[current_floor_index];
    for (int i = 0; i < floor->rooms_count; i++)
    {
        Room *room = floor->rooms[i];
        Position position = get_absolute_position(room);
        if (position.x + 1 <= character.position.x && character.position.x <= position.x + room->width && position.y + 1 <= character.position.y && character.position.y <= position.y + room->height)
            return room;
    }
    return NULL;
}

Weapon *get_current_weapon()
{
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].in_hand)
            return &weapons[i];
}