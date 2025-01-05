#ifndef ROGUE_H
#define ROGUE_H

#include <ncurses.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

// _______________ DEFINES ___________________

#define CREDENTIALS_MAX_LENGTH 16
#define EMAIL_MAX_LENGTH 50
#define PASSWORD_MIN_LENGTH 7
#define MAX_USERS 256

// ______________ TYPES & VARIABLES___________

enum Window
{
    LOGIN,
    SIGNUP,
    GAME
};

typedef struct
{
    char username[CREDENTIALS_MAX_LENGTH + 5];
    char password[CREDENTIALS_MAX_LENGTH + 5];
    char email[EMAIL_MAX_LENGTH + 1];
    bool signed_in;
} Player;

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    Position position;
    char *label;
    int max_length;
    int cursor_index;
    char *value;
} Field;

extern int ch;
extern int screen_width, screen_height;
extern int current_window;
extern Player *player;

// ______________ FUNCTION PROTOTYPES ________

bool screen_setup();
WINDOW *init_win(int x, int y, int width, int height);
bool handle_login();
bool handle_signup();
bool exists_username(char *username);
bool validate_email(char *email);
bool validate_password(char *password);
bool add_user(char *username, char *password, char *email);

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

#endif