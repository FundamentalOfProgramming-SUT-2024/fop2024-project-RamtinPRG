#include "../include/rogue.h"

#define is_obstacle(c) (c == L'┌' || c == L'┐' || c == L'└' || c == L'┘' || c == L'│' || c == L'─' || c == L' ')
#define is_corridor(c) (c == L'█' || c == L'▓' || c == L'▒' || c == L'░')

Character character;
Floor *current_floor;
Room *initial_room;
int current_floor_index;
FILE *log_file;

bool handle_game()
{
    if (game_mode == NEW_GAME)
    {
        char file_name[50] = "logs/";
        strcat(file_name, player->username);
        log_file = fopen(file_name, "w");
        generate_map();
        current_floor_index = 0;
        current_floor = &floors[current_floor_index];
        setup_floor();
        Position position = get_absolute_position(initial_room);
        position.x += rand() % initial_room->width + 1;
        position.y += rand() % initial_room->height + 1;
        place_character(position);
    }
    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            fclose(log_file);
            return false;
        }
        else if (ch == KEY_F(4))
        {
            fclose(log_file);
            current_window = MAIN_MENU;
            return true;
        }
        // else if (ch == 'r')
        // {
        //     generate_floor();
        //     erase();
        //     setup_new_map();
        // }
        else if (ch == KEY_UP && current_floor_index < FLOORS - 1)
        {
            current_floor_index++;
            current_floor = &floors[current_floor_index];
            erase();
            setup_floor();
        }
        else if (ch == KEY_DOWN && current_floor_index > 0)
        {
            current_floor_index--;
            current_floor = &floors[current_floor_index];
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
        else if (ch == '>' & 0x1F)
            register_command("ascend", 0);
        else if (ch == '<' & 0x1F)
            register_command("descend", 0);
    }
    return false;
}

void setup_floor()
{
    erase();
    initial_room = current_floor->rooms[0];
    initial_room->visible = true;
    draw_rooms(current_floor);
    // attron(A_INVIS);
    draw_corridors(current_floor);
    draw_stairs(current_floor);
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

void remove_character()
{
    mvadd_wch(character.position.y, character.position.x, &character.under);
}

void place_character(Position position)
{
    character.position = position;
    mvin_wch(character.position.y, character.position.x, &character.under);
    mvprintw(0, 0, "%lc%d", character.under.chars[0], character.under.chars[1]);
    mvaddch(character.position.y, character.position.x, '@' | COLOR_PAIR(color_settings->color_number[color_settings->current_color_index]) | A_BOLD | A_ITALIC);
}

void teleport_character(Position position)
{
    remove_character();
    place_character(position);
}

void move_character(int direction)
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
    }
}

bool ascend_character()
{
    if (character.under.chars[0] == L'▲')
    {
        current_floor_index++;
        current_floor = &floors[current_floor_index];
        return true;
    }
    return false;
}

bool descend_character()
{
    if (character.under.chars[0] == L'▼')
    {
        current_floor_index--;
        current_floor = &floors[current_floor_index];
        return true;
    }
    return false;
}