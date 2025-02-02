#include "../include/rogue.h"

void setup_sidebar(int list)
{
    Position position;
    position.x = SCREEN_OFFSET + MAP_SCREEN_WIDTH + H_GAP;
    position.y = SCREEN_OFFSET;

    attron(COLOR_PAIR(CHAR_TEAL));
    erase_box(position, SIDEBAR_WIDTH, MAP_SCREEN_HEIGHT + 1);
    attroff(COLOR_PAIR(CHAR_TEAL));

    update_health();
    update_stomach();
    update_gold();
    update_score();
    update_weapon();

    attron(COLOR_PAIR(CHAR_TEAL));
    draw_hline(((Position){position.x + 1, position.y + 12}), SIDEBAR_WIDTH - 2);
    attroff(COLOR_PAIR(CHAR_TEAL));

    if (list == GUIDES)
        draw_guides();
    if (list == FOODS)
        draw_food_inventory();
    if (list == WEAPONS)
        draw_weapon_inventory();
}

Position get_sidebar_position()
{
    Position position;
    position.x = SCREEN_OFFSET + MAP_SCREEN_WIDTH + H_GAP;
    position.y = SCREEN_OFFSET;
    return position;
}

void update_health()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 2;
    attron(COLOR_PAIR(CHAR_LIME) | A_BOLD);
    mvprintw(position.y, position.x, "Health: ");
    attroff(A_BOLD);

    int health_points = character.health / 5;
    attron(A_UNDERLINE);
    for (int i = 0; i < health_points; i++)
        printw("▊");
    for (int i = 0; i < 20 - health_points; i++)
        printw(" ");
    attroff(A_UNDERLINE);

    attron(A_ITALIC);
    printw("  %d%%", character.health);
    attroff(A_ITALIC);

    attroff(COLOR_PAIR(CHAR_LIME));
}

void update_stomach()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 4;
    attron(COLOR_PAIR(CHAR_BRONZE) | A_BOLD);
    mvprintw(position.y, position.x, "Stomach:");
    attroff(A_BOLD);

    int stomach_points = character.stomach / 5;
    attron(A_UNDERLINE);
    for (int i = 0; i < stomach_points; i++)
        printw("▊");
    for (int i = 0; i < 20 - stomach_points; i++)
        printw(" ");
    attroff(A_UNDERLINE);

    attron(A_ITALIC);
    printw("  %d%%", character.stomach);
    attroff(A_ITALIC);

    attroff(COLOR_PAIR(CHAR_BRONZE));
}

void update_gold()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 6;
    attron(COLOR_PAIR(CHAR_YELLOW) | A_BOLD);
    mvprintw(position.y, position.x, "Gold:   ");
    attroff(A_BOLD);

    attron(A_ITALIC);
    printw("%d $", character.gold);
    attroff(A_ITALIC);

    attroff(COLOR_PAIR(CHAR_YELLOW));
}

void update_score()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 8;
    attron(COLOR_PAIR(CHAR_VIOLET) | A_BOLD);
    mvprintw(position.y, position.x, "Score:  ");
    attroff(A_BOLD);

    attron(A_ITALIC);
    printw("%d", character.score);
    attroff(A_ITALIC);

    attroff(COLOR_PAIR(CHAR_VIOLET));
}

void update_weapon()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 10;
    attron(A_BOLD);
    mvprintw(position.y, position.x, "Weapon: ");
    attroff(A_BOLD);

    for (int i = 0; i < weapons_count; i++)
    {
        if (weapons[i].in_hand)
        {
            char current_weapon[20];
            if (weapons[i].type == MACE)
                strcpy(current_weapon, "MACE");
            if (weapons[i].type == DAGGER)
                strcpy(current_weapon, "DAGGER");
            if (weapons[i].type == WAND)
                strcpy(current_weapon, "WAND");
            if (weapons[i].type == ARROW)
                strcpy(current_weapon, "ARROW");
            if (weapons[i].type == SWORD)
                strcpy(current_weapon, "SWORD");
            attron(A_ITALIC);
            printw("%s", current_weapon);
            attroff(A_ITALIC);
            break;
        }
    }
}

void draw_guides()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 14;

    attron(A_ITALIC | A_UNDERLINE | A_BOLD | COLOR_PAIR(1));
    mvprintw(position.y, position.x, "Guides:");
    attroff(A_ITALIC | A_UNDERLINE | A_BOLD | COLOR_PAIR(1));

    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(position.y + 2, position.x, "Movement:");
    attroff(A_BOLD | COLOR_PAIR(2));
    mvprintw(position.y + 2, position.x + 12, "Q   W   E");
    mvprintw(position.y + 3, position.x + 12, "  ↖ ↑ ↗  ");
    mvprintw(position.y + 4, position.x + 12, "A ← @ → D");
    mvprintw(position.y + 5, position.x + 12, "  ↙ ↓ ↘  ");
    mvprintw(position.y + 6, position.x + 12, "Z   S   C");

    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(position.y + 8, position.x, "Ascend:");
    attroff(A_BOLD | COLOR_PAIR(2));
    mvprintw(position.y + 8, position.x + 12, "Ctrl + >");

    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(position.y + 10, position.x, "Descend:");
    attroff(A_BOLD | COLOR_PAIR(2));
    mvprintw(position.y + 10, position.x + 12, "Ctrl + <");
}

void draw_food_inventory()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 14;

    attron(A_ITALIC | A_UNDERLINE | A_BOLD | COLOR_PAIR(1));
    mvprintw(position.y, position.x, "Foods:");
    attroff(A_ITALIC | A_UNDERLINE | A_BOLD | COLOR_PAIR(1));

    int count = inventory_foods_count();

    if (count == 0)
    {
        attron(A_ITALIC | COLOR_PAIR(2) | A_BOLD);
        mvprintw(position.y += 2, position.x, "No food in inventory!");
        attroff(A_ITALIC | COLOR_PAIR(2) | A_BOLD);
    }
    else
        for (int i = 0; i < count; i++)
        {
            position.y += 2;
            move(position.y, position.x);
            attron(A_BOLD | COLOR_PAIR(2));
            printw("%c)   ", 'a' + i);
            attroff(A_BOLD | COLOR_PAIR(2));
            printw("Regular ( %d )", food_inventory_by_index(i)->value);
        }

    attron(A_ITALIC);
    mvprintw(position.y + 4, position.x, "Press Q to exit...");
    attroff(A_ITALIC);
}

void draw_hline(Position position, int length)
{
    move(position.y, position.x);
    for (int i = 0; i < length; i++)
        printw("─");
}

void draw_weapon_inventory()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 14;

    attron(A_ITALIC | A_UNDERLINE | A_BOLD | COLOR_PAIR(1));
    mvprintw(position.y, position.x, "Weapons:");
    attroff(A_ITALIC | A_UNDERLINE | A_BOLD | COLOR_PAIR(1));

    int count = 0;
    position.y += 2;
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].is_picked && weapons[i].type == MACE)
            count++;
    move(position.y, position.x);
    if (count == 0)
        attron(A_DIM);
    attron(A_BOLD | COLOR_PAIR(2));
    printw("%c)   ", 'a');
    attroff(A_BOLD | COLOR_PAIR(2));
    printw("MACE   ( %d )", count);
    if (count == 0)
        attroff(A_DIM);

    count = 0;
    position.y += 2;
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].is_picked && weapons[i].type == DAGGER)
            count++;
    move(position.y, position.x);
    if (count == 0)
        attron(A_DIM);
    attron(A_BOLD | COLOR_PAIR(2));
    printw("%c)   ", 'b');
    attroff(A_BOLD | COLOR_PAIR(2));
    printw("DAGGER ( %d )", count);
    if (count == 0)
        attroff(A_DIM);

    count = 0;
    position.y += 2;
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].is_picked && weapons[i].type == WAND)
            count++;
    move(position.y, position.x);
    if (count == 0)
        attron(A_DIM);
    attron(A_BOLD | COLOR_PAIR(2));
    printw("%c)   ", 'c');
    attroff(A_BOLD | COLOR_PAIR(2));
    printw("WAND   ( %d )", count);
    if (count == 0)
        attroff(A_DIM);

    count = 0;
    position.y += 2;
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].is_picked && weapons[i].type == ARROW)
            count++;
    move(position.y, position.x);
    if (count == 0)
        attron(A_DIM);
    attron(A_BOLD | COLOR_PAIR(2));
    printw("%c)   ", 'd');
    attroff(A_BOLD | COLOR_PAIR(2));
    printw("ARROW  ( %d )", count);
    if (count == 0)
        attroff(A_DIM);

    count = 0;
    position.y += 2;
    for (int i = 0; i < weapons_count; i++)
        if (weapons[i].is_picked && weapons[i].type == SWORD)
            count++;
    move(position.y, position.x);
    if (count == 0)
        attron(A_DIM);
    attron(A_BOLD | COLOR_PAIR(2));
    printw("%c)   ", 'e');
    attroff(A_BOLD | COLOR_PAIR(2));
    printw("SWORD  ( %d )", count);
    if (count == 0)
        attroff(A_DIM);

    attron(A_ITALIC);
    mvprintw(position.y + 4, position.x, "Press Q to exit...");
    attroff(A_ITALIC);
}