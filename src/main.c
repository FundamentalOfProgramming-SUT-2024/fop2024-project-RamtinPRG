#include "../include/rogue.h"

int main()
{
    // Initializing
    setup();

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
        else if (current_window == SETTINGS)
        {
            if (!handle_settings())
                break;
        }
        else if (current_window == GAME)
        {
            generate_map();
            if (!handle_game())
                break;
        }
    }

    endwin();
    Mix_FreeMusic(music);
    Mix_Quit();
    SDL_Quit();
    return 0;
}