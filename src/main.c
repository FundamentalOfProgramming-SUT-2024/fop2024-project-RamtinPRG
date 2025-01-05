#include "../include/rogue.h"

int ch;
int screen_width, screen_height;
int current_window = LOGIN;
Player *player;

int main()
{
    // Initializing
    player = (Player *)malloc(sizeof(Player));
    player->signed_in = false;
    screen_setup();

    // Mainloop
    while (1)
    {
        if (!player->signed_in)
        {
            if (current_window == LOGIN)
            {
                if (!handle_login())
                    break;
            }
            else if (current_window == SIGNUP)
            {
                if (!handle_signup())
                    break;
            }
        }
        else if (current_window == MAIN_MENU)
        {
            if (!handle_main_menu())
                break;
        }
    }

    endwin();
    return 0;
}