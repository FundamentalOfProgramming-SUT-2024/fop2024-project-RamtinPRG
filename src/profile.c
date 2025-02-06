#include "../include/rogue.h"

bool handle_profile()
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

    erase_scr();

    for (int i = 0; i < LOGO_HEIGHT; i++)
        for (int j = 0; j < LOGO_WIDTH; j++)
            mvadd_wch(i + 2, j + 4, &((cchar_t){0, {logo[i][j]}, 4}));

    // char username[100], email[100], password[100], best_score[100], total_score[100], total_golds[100], vicotories[100], defeats[100];
    // sprintf(username, "Username")
    char fields[8][100] = {"Username", "Email", "Password", "Best Score", "Total Score", "Total Golds", "Victories", "Defeats"};
    attron(COLOR_PAIR(2) | A_BOLD);
    for (int i = 0; i < 8; i++)
        mvprintw(13 + i * 2, 4, "%s:", fields[i]);
    attroff(COLOR_PAIR(2) | A_BOLD);

    mvprintw(13, 20, "%s", player->username);
    mvprintw(15, 20, "%s", player->email);
    mvprintw(17, 20, "%s", player->password);
    mvprintw(19, 20, "%d", player->best_score);
    mvprintw(21, 20, "%d", player->score);
    mvprintw(23, 20, "%d", player->gold);
    mvprintw(25, 20, "%d", player->victories_count);
    mvprintw(27, 20, "%d", player->defeats_count);

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
    }

    return false;
}
