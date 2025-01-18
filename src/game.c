#include "../include/rogue.h"

#define is_obstacle(c) (c == L'┌' || c == L'┐' || c == L'└' || c == L'┘' || c == L'│' || c == L'─' || c == L' ')
#define is_corridor(c) (c == L'█' || c == L'▓' || c == L'▒' || c == L'░')

Character character;
Room *initial_room;

bool handle_game()
{
    generate_map();
    erase();
    setup_new_map();
    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
            return false;
        else if (ch == KEY_F(4))
        {
            current_window = MAIN_MENU;
            return true;
        }
        else if (ch == 'r')
        {
            generate_map();
            erase();
            setup_new_map();
        }
        else if (ch == 'w' || ch == 'W')
            move_character(N);
        else if (ch == 'e' || ch == 'E')
            move_character(NE);
        else if (ch == 'd' || ch == 'D')
            move_character(E);
        else if (ch == 'c' || ch == 'C')
            move_character(SE);
        else if (ch == 's' || ch == 'S')
            move_character(S);
        else if (ch == 'z' || ch == 'Z')
            move_character(SW);
        else if (ch == 'a' || ch == 'A')
            move_character(W);
        else if (ch == 'q' || ch == 'Q')
            move_character(NW);
    }
    return false;
}

void setup_new_map()
{
    initial_room = rooms[rand() % rooms_count];
    initial_room->visible = true;
    draw_rooms();
    // attron(A_INVIS);
    draw_corridors();
    // attroff(A_INVIS);
    Position position;
    position = get_absolute_position(initial_room);
    position.x += rand() % initial_room->width + 1;
    position.y += rand() % initial_room->height + 1;
    character.position = position;
    mvin_wch(position.y, position.x, &character.under);
    mvaddch(position.y, position.x, '@' | COLOR_PAIR(color_settings->color_number[color_settings->current_color_index]) | A_BOLD | A_ITALIC);
}

void move_character(int direction)
{
    character.prev_position = character.position;
    switch (direction)
    {
    case N:
        character.position.y--;
        break;
    case NE:
        character.position.y--;
        character.position.x++;
        break;
    case E:
        character.position.x++;
        break;
    case SE:
        character.position.y++;
        character.position.x++;
        break;
    case S:
        character.position.y++;
        break;
    case SW:
        character.position.y++;
        character.position.x--;
        break;
    case W:
        character.position.x--;
        break;
    case NW:
        character.position.y--;
        character.position.x--;
        break;
    default:
        break;
    }
    cchar_t dest;
    mvin_wch(character.position.y, character.position.x, &dest);
    if (!is_obstacle(dest.chars[0]))
    {
        mvadd_wch(character.prev_position.y, character.prev_position.x, &character.under);
        mvin_wch(character.position.y, character.position.x, &character.under);
        mvaddch(character.position.y, character.position.x, '@' | COLOR_PAIR(color_settings->color_number[color_settings->current_color_index]) | A_BOLD | A_ITALIC);
    }
    else
        character.position = character.prev_position;
}