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

    attron(COLOR_PAIR(CHAR_TEAL));
    draw_hline(((Position){position.x + 1, position.y + 6}), SIDEBAR_WIDTH - 2);
    attroff(COLOR_PAIR(CHAR_TEAL));

    draw_guides();
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

void draw_guides()
{
    Position position = get_sidebar_position();
    position.x += 3;
    position.y += 8;

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

void draw_hline(Position position, int length)
{
    move(position.y, position.x);
    for (int i = 0; i < length; i++)
        printw("─");
}