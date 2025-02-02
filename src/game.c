#include "../include/rogue.h"

#define is_obstacle(c) (c == L'╭' || c == L'╮' || c == L'╰' || c == L'╯' || c == L'┌' || c == L'┐' || c == L'└' || c == L'┘' || c == L'│' || c == L'─' || c == L' ')
#define is_corridor(c) (c == L'█' || c == L'▓' || c == L'▒' || c == L'░')

cchar_t trap_character = {0, {L'•'}, 4};
cchar_t ground_character = {A_DIM, {L'.'}, 0};

Character character;
Room *initial_room;
int current_floor_index;
FILE *log_file;
FILE *map_file;
char game_message[500];
int timeline_counter = 0;

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
    current_floor_index = 0;
    initial_room = floors[0].rooms[0];
    initial_room->visible = true;
    player->continuable = true;
    character.health = 100;
    character.stomach = 100;
    character.gold = 0;
    character.score = 0;
    strcpy(game_message, "Welcome to the dungeons of DOOM!");
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
        else if (ch == 'w' || ch == 'W')
            register_command("move", 1, N);
        else if (ch == 'e' || ch == 'E')
            register_command("move", 1, NE);
        else if (ch == 'd' || ch == 'D')
            register_command("move", 1, E);
        else if (ch == 'c' || ch == 'C')
            register_command("move", 1, SE);
        else if (ch == 's' || ch == 'S')
            register_command("move", 1, S);
        else if (ch == 'z' || ch == 'Z')
            register_command("move", 1, SW);
        else if (ch == 'a' || ch == 'A')
            register_command("move", 1, W);
        else if (ch == 'q' || ch == 'Q')
            register_command("move", 1, NW);
        else if (ch == '>')
            register_command("ascend", 0);
        else if (ch == '<')
            register_command("descend", 0);
        else if (ch == ('p' & 0x1F))
            register_command("pick", 0);
        else if (ch == ('e' & 0x1F))
            eat_food();

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
    traps_count = 0;
    golds_count = 0;
    black_golds_count = 0;
    foods_count = 0;
}

void setup_floor()
{
    erase_scr();
    draw_rooms(&floors[current_floor_index]);
    // attron(A_INVIS);
    draw_corridors(&floors[current_floor_index]);
    draw_stairs(&floors[current_floor_index]);
    draw_traps(&floors[current_floor_index]);
    draw_golds(&floors[current_floor_index]);
    draw_black_golds(&floors[current_floor_index]);
    draw_foods(&floors[current_floor_index]);
    setup_sidebar(GUIDES);
    setup_message_box();
    // attroff(A_INVIS);
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
            attron(COLOR_PAIR(CHAR_BRONZE));
            mvprintw(position.y, position.x, "♥");
            attroff(COLOR_PAIR(CHAR_BRONZE));
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
        strcpy(message, "");
    }
}

bool ascend_character(char *message)
{
    if (character.under.chars[0] == L'▲')
    {
        current_floor_index++;
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