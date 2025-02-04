#include "../include/rogue.h"

void setup_message_box()
{
    int width = MAP_SCREEN_WIDTH + H_GAP + SIDEBAR_WIDTH;
    Position position;
    position.x = SCREEN_OFFSET;
    position.y = SCREEN_OFFSET + MAP_SCREEN_HEIGHT + V_GAP;

    attron(COLOR_PAIR(CHAR_YELLOW));
    erase_box(position, width, MESSAGES_HEIGHT + 1);
    attron(A_BOLD);
    mvprintw(position.y, position.x + 5, " Message ");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(CHAR_YELLOW));

    if (game_message_count > 0)
    {
        attron(A_ITALIC);
        mvprintw(position.y + 2, position.x + 5, "%s", game_message[0]);
        attroff(A_ITALIC);
    }
    if (game_message_count > 1)
    {
        for (int i = 1; i < game_message_count; i++)
        {
            printw("     Press SPACE...");
            while ((ch = getch()) != ' ')
                ;
            attron(COLOR_PAIR(CHAR_YELLOW));
            erase_box(position, width, MESSAGES_HEIGHT + 1);
            attron(A_BOLD);
            mvprintw(position.y, position.x + 5, " Message ");
            attroff(A_BOLD);
            attroff(COLOR_PAIR(CHAR_YELLOW));

            attron(A_ITALIC);
            mvprintw(position.y + 2, position.x + 5, "%s", game_message[i]);
            attroff(A_ITALIC);
        }
    }
    game_message_count = 0;
}

void add_message(char *message)
{
    strcpy(game_message[game_message_count], message);
    game_message_count++;
}