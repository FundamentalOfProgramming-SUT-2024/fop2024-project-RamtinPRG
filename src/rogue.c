#include "../include/rogue.h"

// ______________ FUNCTIONS __________________

bool screen_setup()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, screen_height, screen_width);

    if (!has_colors())
    {
        char string[200] = "Your terminal doesn't support colors. Press a key to exit!";
        move(screen_height / 2, (screen_width - strlen(string)) / 2);
        printw("%s", string);
        getch();
        endwin();
        exit_curses(1);
        return 0;
    }

    start_color();

    // Redefining black
    init_color(COLOR_BLACK, 90, 78, 129);
    // Purple
    init_color(CUSTOM_PURPLE, 623, 588, 902);
    // Green
    init_color(CUSTOM_GREEN, 459, 796, 533);

    init_pair(1, CUSTOM_GREEN, COLOR_BLACK);
    init_pair(2, CUSTOM_PURPLE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    refresh();
    return 1;
}

WINDOW *init_win(int x, int y, int width, int height)
{
    WINDOW *win = newwin(height, width, y, x);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

bool exists_username(char *username)
{
    FILE *file;
    file = fopen("usernames.txt", "r");
    if (file == NULL)
    {
        file = fopen("usernames.txt", "a");
        fclose(file);
        return false;
    }
    char lines[MAX_USERS][CREDENTIALS_MAX_LENGTH + 2];
    int line_index = 0;
    while (fgets(lines[line_index], CREDENTIALS_MAX_LENGTH + 2, file) != NULL)
    {
        int len = strlen(lines[line_index]);
        if (lines[line_index][len - 1] == '\n')
            lines[line_index][len - 1] = '\0';
        if (strcmp(lines[line_index], username) == 0)
        {
            fclose(file);
            return true;
        }
        line_index++;
    }
    fclose(file);
    return false;
}

bool validate_email(char *email)
{
    char *ptr = email;
    ptr++;
    while (*ptr && *ptr != '@')
        ptr++;
    ptr++;
    if (*ptr)
    {
        while (*ptr && *ptr != '.')
            ptr++;
        if (*ptr && *(ptr + 1))
            return true;
        return false;
    }
    return false;
}

bool validate_password(char *password)
{
    int upper_count = 0, lower_count = 0, digit_count = 0;
    int len = strlen(password);
    for (int i = 0; i < len; i++)
    {
        if (isupper(password[i]))
            upper_count++;
        if (islower(password[i]))
            lower_count++;
        if (isdigit(password[i]))
            digit_count++;
    }
    return (upper_count && lower_count && digit_count);
}

bool add_user(char *username, char *password, char *email)
{
    FILE *file = fopen("usernames.txt", "a");
    fputs(username, file);
    fputc('\n', file);
    fclose(file);
    char file_name[100] = "users/";
    strcat(file_name, username);
    strcat(file_name, ".txt");
    file = fopen(file_name, "w");
    fputs(password, file);
    fputc('\n', file);
    fputs(email, file);
    fputc('\n', file);
    fclose(file);
    return 1;
}

void draw_menu(WINDOW *win, MenuItem *items, int n, int selected_index)
{
    attron(A_ITALIC);
    for (int i = 0; i < n; i++)
    {
        if (i == selected_index)
        {
            attron(A_BOLD | COLOR_PAIR(3));
            mvwprintw(win, items[i].position.y, items[i].position.x, " • ");
            wprintw(win, "%s", items[i].label);
            attroff(A_BOLD | COLOR_PAIR(3));
            continue;
        }
        attron(A_DIM);
        mvwprintw(win, items[i].position.y, items[i].position.x, "   ");
        wprintw(win, "%s", items[i].label);
        attroff(A_DIM);
    }
    attroff(A_ITALIC);
}