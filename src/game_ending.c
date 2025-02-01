#include "../include/rogue.h"

bool handle_defeat()
{
    wchar_t gameover[GAMEOVER_HEIGHT][GAMEOVER_WIDTH + 1] =
        {L"  ▄████  ▄▄▄       ███▄ ▄███▓▓█████     ▒█████   ██▒   █▓▓█████  ██▀███  ",
         L" ██▒ ▀█▒▒████▄    ▓██▒▀█▀ ██▒▓█   ▀    ▒██▒  ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒",
         L"▒██░▄▄▄░▒██  ▀█▄  ▓██    ▓██░▒███      ▒██░  ██▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒",
         L"░▓█  ██▓░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄    ▒██   ██░  ▒██ █░░▒▓█  ▄ ▒██▀▀█▄  ",
         L"░▒▓███▀▒ ▓█   ▓██▒▒██▒   ░██▒░▒████▒   ░ ████▓▒░   ▒▀█░  ░▒████▒░██▓ ▒██▒",
         L" ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░   ░ ▒░▒░▒░    ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░",
         L"  ░   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░     ░ ▒ ▒░    ░ ░░   ░ ░  ░  ░▒ ░ ▒░",
         L"░ ░   ░   ░   ▒   ░      ░      ░      ░ ░ ░ ▒       ░░     ░     ░░   ░ ",
         L"      ░       ░  ░       ░      ░  ░       ░ ░        ░     ░  ░   ░     ",
         L"                                                     ░                   "};

    Position position;
    position.x = MIN_SCREEN_WIDTH / 2 - GAMEOVER_WIDTH / 2;
    position.y = MIN_SCREEN_HEIGHT / 2 - GAMEOVER_HEIGHT / 2 - 5;

    for (int i = 1; i < MIN_SCREEN_HEIGHT - 1; i++)
    {
        usleep(50000);
        move(i, 1);
        for (int j = 0; j < MIN_SCREEN_WIDTH - 2; j++)
            printw(" ");
        refresh();
    }
    usleep(1000000);
    for (int i = 0; i < GAMEOVER_HEIGHT; i++)
    {
        usleep(50000);
        for (int j = 0; j < GAMEOVER_WIDTH; j++)
            mvadd_wch(position.y + i, position.x + j, &((cchar_t){A_BLINK, {gameover[i][j]}, 4}));
        refresh();
    }
    usleep(1000000);
    position.y += GAMEOVER_HEIGHT + 2;

    char stats[500];
    sprintf(stats, "Golds: %d $        Total Score: %d", character.gold, character.score);
    int length = strlen(stats);
    for (int i = 0; i < length; i++)
    {
        usleep(50000);
        mvaddch(position.y, position.x + i, stats[i] | COLOR_PAIR(CHAR_YELLOW) | A_ITALIC);
        refresh();
    }
    usleep(1000000);
    position.y += 3;

    char exit_message[] = "Press any key to continue...";
    length = strlen(exit_message);
    for (int i = 0; i < length; i++)
    {
        usleep(50000);
        mvaddch(position.y, position.x + i, exit_message[i] | A_ITALIC);
        refresh();
    }

    player->continuable = false;
    player->games_count++;
    player->defeats_count++;
    player->gold += character.gold;

    flushinp();

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            return false;
        }
        else
        {
            current_window = MAIN_MENU;
            return true;
        }
    }
}

bool handle_victory()
{
    wchar_t victory[VICTORY_WIDTH][VICTORY_WIDTH + 1] =
        {L" ██▒   █▓ ██▓ ▄████▄  ▄▄▄█████▓ ▒█████   ██▀███ ▓██   ██▓",
         L"▓██░   █▒▓██▒▒██▀ ▀█  ▓  ██▒ ▓▒▒██▒  ██▒▓██ ▒ ██▒▒██  ██▒",
         L" ▓██  █▒░▒██▒▒▓█    ▄ ▒ ▓██░ ▒░▒██░  ██▒▓██ ░▄█ ▒ ▒██ ██░",
         L"  ▒██ █░░░██░▒▓▓▄ ▄██▒░ ▓██▓ ░ ▒██   ██░▒██▀▀█▄   ░ ▐██▓░",
         L"   ▒▀█░  ░██░▒ ▓███▀ ░  ▒██▒ ░ ░ ████▓▒░░██▓ ▒██▒ ░ ██▒▓░",
         L"   ░ ▐░  ░▓  ░ ░▒ ▒  ░  ▒ ░░   ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░  ██▒▒▒ ",
         L"   ░ ░░   ▒ ░  ░  ▒       ░      ░ ▒ ▒░   ░▒ ░ ▒░▓██ ░▒░ ",
         L"     ░░   ▒ ░░          ░      ░ ░ ░ ▒    ░░   ░ ▒ ▒ ░░  ",
         L"      ░   ░  ░ ░                   ░ ░     ░     ░ ░     ",
         L"     ░       ░                                   ░ ░     "};

    Position position;
    position.x = MIN_SCREEN_WIDTH / 2 - VICTORY_WIDTH / 2;
    position.y = MIN_SCREEN_HEIGHT / 2 - VICTORY_HEIGHT / 2 - 5;

    for (int i = 1; i < MIN_SCREEN_HEIGHT - 1; i++)
    {
        usleep(50000);
        move(i, 1);
        for (int j = 0; j < MIN_SCREEN_WIDTH - 2; j++)
            printw(" ");
        refresh();
    }
    usleep(1000000);
    for (int i = 0; i < VICTORY_HEIGHT; i++)
    {
        usleep(50000);
        for (int j = 0; j < VICTORY_WIDTH; j++)
            mvadd_wch(position.y + i, position.x + j, &((cchar_t){A_BLINK, {victory[i][j]}, CHAR_TEAL}));
        refresh();
    }
    usleep(1000000);
    position.y += VICTORY_HEIGHT + 2;

    char stats[500];
    sprintf(stats, "Golds: %d $        Total Score: %d", character.gold, character.score);
    int length = strlen(stats);
    for (int i = 0; i < length; i++)
    {
        usleep(50000);
        mvaddch(position.y, position.x + i, stats[i] | COLOR_PAIR(CHAR_LIME) | A_ITALIC);
        refresh();
    }
    usleep(1000000);
    position.y += 3;

    char exit_message[] = "Press any key to continue...";
    length = strlen(exit_message);
    for (int i = 0; i < length; i++)
    {
        usleep(50000);
        mvaddch(position.y, position.x + i, exit_message[i] | A_ITALIC);
        refresh();
    }

    player->continuable = false;
    player->score += character.score;
    player->gold += character.gold;
    if (character.score > player->best_score)
        player->best_score = character.score;
    player->games_count++;
    player->victories_count++;

    flushinp();

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            return false;
        }
        else
        {
            current_window = MAIN_MENU;
            return true;
        }
    }
}