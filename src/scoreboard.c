#include "../include/rogue.h"

int scoreboard_page;
int scoreboard_pages_count;
int scoreboard_items_count;

bool handle_scoreboard()
{
    FILE *usernames_file;
    usernames_file = fopen("usernames.txt", "r");
    int usernames_count = file_lines_count(usernames_file);
    Player scoreboard_players[usernames_count];

    for (int i = 0; i < usernames_count; i++)
    {
        char username[100];
        fscanf(usernames_file, "%s", username);
        if (strcmp(player->username, username) == 0)
            scoreboard_players[i] = *player;
        else
        {
            strcpy(scoreboard_players[i].username, username);
            load_user_data(username, &scoreboard_players[i]);
        }
    }
    fclose(usernames_file);

    scoreboard_items_count = usernames_count;
    scoreboard_page = 0;
    scoreboard_pages_count = usernames_count / SCOREBOARD_ITEM_PER_PAGE;
    if (usernames_count % SCOREBOARD_ITEM_PER_PAGE != 0)
        scoreboard_pages_count++;

    qsort(scoreboard_players, usernames_count, sizeof(Player), compare_players);

    erase_scr();
    setup_scoreboard(scoreboard_players);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            return false;
        }
        else if (ch == KEY_F(4))
        {
            current_window = MAIN_MENU;
            return true;
        }
        else if (ch == KEY_RIGHT)
        {
            if (scoreboard_page < scoreboard_pages_count - 1)
                scoreboard_page++;
            setup_scoreboard(scoreboard_players);
        }
        else if (ch == KEY_LEFT)
        {
            if (scoreboard_page > 0)
                scoreboard_page--;
            setup_scoreboard(scoreboard_players);
        }
    }
}

int compare_strings(char *a, char *b)
{
    char *a_cpy = (char *)malloc(100 * sizeof(char)), *b_cpy = (char *)malloc(100 * sizeof(char));
    strcpy(a_cpy, a);
    strcpy(b_cpy, b);
    for (int i = 0; i < strlen(a_cpy); i++)
        if ('A' <= a_cpy[i] && a_cpy[i] <= 'Z')
            a_cpy[i] += 'a' - 'A';
    for (int i = 0; i < strlen(b_cpy); i++)
        if ('A' <= b_cpy[i] && b_cpy[i] <= 'Z')
            b_cpy[i] += 'a' - 'A';
    while (*a_cpy && *b_cpy)
    {
        if (*a_cpy != *b_cpy)
            return *a_cpy - *b_cpy;
        a_cpy++;
        b_cpy++;
    }
    return *a_cpy - *b_cpy;
}

int compare_players(const void *ap, const void *bp)
{
    Player *a = (Player *)ap, *b = (Player *)bp;
    if (a->score != b->score)
        return b->score - a->score;
    else
    {
        if (a->victories_count != b->victories_count)
            return b->victories_count - a->victories_count;
        else
        {
            int string_comparison = compare_strings(a->username, b->username);
            return string_comparison;
        }
    }
}

void print_by_index(Player *player, int index)
{
    if (index == 1)
        printw("%s", player->username);
    else if (index == 2)
        printw("%d", player->score);
    else if (index == 3)
        printw("%d", player->victories_count);
    else if (index == 4)
        printw("%d", player->defeats_count);
    else if (index == 5)
        printw("%d", player->gold);
    else if (index == 6)
        printw("%d", player->best_score);
}

void setup_scoreboard(Player players[])
{
    int scoreboard_columns_count = 7, scoreboard_columns_width = 20;
    char scoreboard_columns[7][100] = {"Rank", "Name", "Score", "Victories", "Defeats", "Golds", "Best Score"};
    int scoreboard_height = 2 * (SCOREBOARD_ITEM_PER_PAGE + 1) + 3;
    int scoreboard_wdith = (scoreboard_columns_width + 1) * scoreboard_columns_count + 1;
    Position position;
    position.x = MIN_SCREEN_WIDTH / 2 - scoreboard_wdith / 2;
    position.y = MIN_SCREEN_HEIGHT / 2 - scoreboard_height / 2;

    attron(COLOR_PAIR(CHAR_VIOLET));

    erase_box(position, scoreboard_wdith, scoreboard_height);

    for (int i = 0; i < scoreboard_wdith; i++)
        if (i % (scoreboard_columns_width + 1) == 0 && i != 0 && i != scoreboard_wdith - 1)
            mvprintw(position.y, position.x + i, "┬");

    for (int i = 0; i < scoreboard_wdith; i++)
        if (i % (scoreboard_columns_width + 1) == 0 && i != 0 && i != scoreboard_wdith - 1)
            mvprintw(position.y + 1, position.x + i, "│");

    move(position.y + 2, position.x);
    for (int i = 0; i < scoreboard_wdith; i++)
        if (i == 0)
            printw("├");
        else if (i == scoreboard_wdith - 1)
            printw("┤");
        else if (i % (scoreboard_columns_width + 1) == 0)
            printw("┼");
        else
            printw("─");

    for (int i = 0; i < scoreboard_wdith; i++)
        for (int j = 0; j < scoreboard_height - 4; j++)
            if (i % (scoreboard_columns_width + 1) == 0 && i != 0 && i != scoreboard_wdith - 1)
                mvprintw(position.y + 3 + j, position.x + i, "│");

    for (int i = 0; i < scoreboard_wdith; i++)
        if (i % (scoreboard_columns_width + 1) == 0 && i != 0 && i != scoreboard_wdith - 1)
            mvprintw(position.y + scoreboard_height - 1, position.x + i, "┴");

    for (int i = 0; i < scoreboard_columns_count; i++)
        mvprintw(position.y + 1, position.x + i * (scoreboard_columns_width + 1) + 2, "%s", scoreboard_columns[i]);

    attroff(COLOR_PAIR(CHAR_VIOLET));

    int start = scoreboard_page * SCOREBOARD_ITEM_PER_PAGE;
    int end = (scoreboard_page + 1) * SCOREBOARD_ITEM_PER_PAGE;
    if (end > scoreboard_items_count)
        end = scoreboard_items_count;
    for (int i = start; i < end; i++)
    {
        for (int j = 0; j < scoreboard_columns_count; j++)
        {
            move(position.y + 4 + (i - start) * 2, position.x + j * (scoreboard_columns_width + 1) + 2);
            if (j == 0)
                printw("%d", i + 1);
            else
                print_by_index(players + i, j);
        }
    }

    move(position.y + scoreboard_height + 2, position.x + scoreboard_wdith / 2 - ((scoreboard_pages_count - 1) * 3 + 1) / 2);
    for (int i = 0; i < scoreboard_pages_count; i++)
    {
        if (i == scoreboard_page)
            attron(COLOR_PAIR(CHAR_PINK) | A_BOLD);
        else
            attron(A_DIM);
        printw("◉  ");
        if (i == scoreboard_page)
            attroff(COLOR_PAIR(CHAR_PINK) | A_BOLD);
        else
            attroff(A_DIM);
    }
}