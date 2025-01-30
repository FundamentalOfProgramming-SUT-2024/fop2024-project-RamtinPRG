#include "../include/rogue.h"

void setup_message_box()
{
    int width = MAP_SCREEN_WIDTH + H_GAP + SIDEBAR_WIDTH;
    Position position;
    position.x = SCREEN_OFFSET;
    position.y = SCREEN_OFFSET + MAP_SCREEN_HEIGHT + V_GAP;
    attron(COLOR_PAIR(CHAR_YELLOW));
    draw_box(position, width, MESSAGES_HEIGHT + 1);
    attron(A_BOLD);
    mvprintw(position.y, position.x + 5, " Message ");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(CHAR_YELLOW));
}