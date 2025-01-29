#include "../include/rogue.h"

void setup_sidebar()
{
    Position position;
    position.x = SCREEN_OFFSET + MAP_SCREEN_WIDTH + GAP;
    position.y = SCREEN_OFFSET;
    attron(COLOR_PAIR(CHAR_TEAL));
    draw_box(position, SIDEBAR_WIDTH, MAP_SCREEN_HEIGHT);
    attroff(COLOR_PAIR(CHAR_TEAL));
    update_health();
    update_gold();
}

Position get_sidebar_position()
{
    Position position;
    position.x = SCREEN_OFFSET + MAP_SCREEN_WIDTH + GAP;
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

void update_gold()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 4;
    attron(COLOR_PAIR(CHAR_YELLOW) | A_BOLD);
    mvprintw(position.y, position.x, "Gold:   ");
    attroff(A_BOLD);

    attron(A_ITALIC);
    printw("%d $", character.gold);
    attroff(A_ITALIC);

    attroff(COLOR_PAIR(CHAR_YELLOW));
}