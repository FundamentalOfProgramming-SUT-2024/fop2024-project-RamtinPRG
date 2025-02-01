#include "../include/rogue.h"

bool handle_main_menu()
{
    wchar_t logo[LOGO_HEIGHT][LOGO_WIDTH + 1] =
        {L" ██▀███   ▒█████    ▄████  █    ██ ▓█████ ",
         L"▓██ ▒ ██▒▒██▒  ██▒ ██▒ ▀█▒ ██  ▓██▒▓█   ▀ ",
         L"▓██ ░▄█ ▒▒██░  ██▒▒██░▄▄▄░▓██  ▒██░▒███   ",
         L"▒██▀▀█▄  ▒██   ██░░▓█  ██▓▓▓█  ░██░▒▓█  ▄ ",
         L"░██▓ ▒██▒░ ████▓▒░░▒▓███▀▒▒▒█████▓ ░▒████▒",
         L"░ ▒▓ ░▒▓░░ ▒░▒░▒░  ░▒   ▒ ░▒▓▒ ▒ ▒ ░░ ▒░ ░",
         L"  ░▒ ░ ▒░  ░ ▒ ▒░   ░   ░ ░░▒░ ░ ░  ░ ░  ░",
         L"  ░░   ░ ░ ░ ░ ▒  ░ ░   ░  ░░░ ░ ░    ░   ",
         L"   ░         ░ ░        ░    ░        ░  ░"};
    char menu_items[][21] = {"New Game", "Continue", "Scoreboard", "Settings", "Exit"};
    int items_count = 5, item_index = 0, menu_width = 20;
    MenuItem items[items_count];

    for (int i = 0; i < items_count; i++)
    {
        items[i].label = (char *)calloc(50, sizeof(char));
        strcpy(items[i].label, menu_items[i]);
        items[i].position.y = 12 + i * 2;
        items[i].position.x = 2;
    }

    erase_scr();

    for (int i = 0; i < LOGO_HEIGHT; i++)
        for (int j = 0; j < LOGO_WIDTH; j++)
            mvadd_wch(i + 1, j + 2, &((cchar_t){0, {logo[i][j]}, 4}));
    draw_menu(stdscr, items, items_count, item_index);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
            return false;
        else if (ch == KEY_UP)
        {
            item_index--;
            if (item_index < 0)
                item_index += items_count;
            draw_menu(stdscr, items, items_count, item_index);
        }
        else if (ch == KEY_DOWN)
        {
            item_index++;
            if (item_index >= items_count)
                item_index -= items_count;
            draw_menu(stdscr, items, items_count, item_index);
        }
        else if (ch == '\n')
        {
            if (item_index == 0)
            {
                current_window = GAME;
                previous_window = MAIN_MENU;
                game_mode = NEW_GAME;
                return true;
            }
            else if (item_index == 1 && player->continuable)
            {
                current_window = GAME;
                previous_window = MAIN_MENU;
                game_mode = CONTINUE;
                return true;
            }
            else if (item_index == 2)
            {
                current_window = SCOREBOARD;
                previous_window = MAIN_MENU;
                return true;
            }
            else if (item_index == 3)
            {
                current_window = SETTINGS;
                previous_window = MAIN_MENU;
                return true;
            }
            else if (item_index == 4)
                return false;
        }
    }

    return false;
}
