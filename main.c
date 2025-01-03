#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

// _______________ DEFINES ___________________

#define CREDENTIALS_MAX_LENGTH 16
#define PASSWORD_MIN_LENGTH 7

// ______________ TYPES & VARIABLES___________

enum Window
{
    LOGIN,
    SIGNUP,
    GAME
};

typedef struct
{
    char username[CREDENTIALS_MAX_LENGTH + 1];
    char password[CREDENTIALS_MAX_LENGTH + 1];
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

int ch;
int signed_in = false;
int screen_width, screen_height;
int current_window = LOGIN;

// ______________ FUNCTION PROTOTYPES ________

bool screen_setup();
WINDOW *init_win(int x, int y, int width, int height);
bool handle_login();

int main()
{
    // Initializing
    screen_setup();

    // WINDOW *login_win = init_win(screen_width / 4, screen_height / 4, screen_width / 2, screen_height / 2);
    // WINDOW *signup_win = init_win(screen_width / 4, screen_height / 4, screen_width / 2, screen_height / 2);
    // PANEL *panels[2] = {new_panel(sign_up_win), new_panel(log_in_win)};

    // update_panels();
    // doupdate();

    while (1)
    {
        if (!signed_in)
        {
            if (current_window == LOGIN)
            {
                if (!handle_login())
                {
                    break;
                }
            }
        }
        else
        {
            clear();
            mvprintw(0, 0, "Have fun playing the game :)");
        }
    }

    endwin();
    return 0;
}

bool screen_setup()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    setlocale(LC_ALL, "");
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

bool handle_login()
{
    int height, width;
    getmaxyx(stdscr, height, width);
    Field fields[2];
    int field_index = 0;

    init_color(8, 623, 588, 902);
    init_color(9, 459, 796, 533);

    init_pair(1, 9, COLOR_BLACK);
    init_pair(2, 8, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    fields[0].label = (char *)calloc(50, sizeof(char));
    fields[0].label = "Username";
    fields[0].position.y = 9;
    fields[0].position.x = 2;
    fields[0].cursor_index = 0;
    fields[0].max_length = CREDENTIALS_MAX_LENGTH;
    fields[0].value = (char *)calloc(fields[0].max_length + 1, sizeof(char));

    fields[1].label = (char *)calloc(50, sizeof(char));
    fields[1].label = "Password";
    fields[1].position.y = 11;
    fields[1].position.x = 2;
    fields[1].cursor_index = 0;
    fields[1].max_length = CREDENTIALS_MAX_LENGTH;
    fields[1].value = (char *)calloc(fields[1].max_length + 1, sizeof(char));

    curs_set(1);

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(2, 2, "Login");
    attroff(A_BOLD | COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvprintw(5, 2, "* ");
    attron(A_ITALIC | A_UNDERLINE);
    printw("Use Tab to to switch between fields.");
    attroff(A_ITALIC | A_UNDERLINE);
    mvprintw(6, 2, "* ");
    attron(A_ITALIC | A_UNDERLINE);
    printw("Use Enter to submit the form.");
    attroff(A_ITALIC | A_UNDERLINE | COLOR_PAIR(3));

    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(fields[0].position.y, fields[0].position.x, "%s: ", fields[0].label);
    chgat(fields[0].max_length, A_UNDERLINE, 0, NULL);
    mvprintw(fields[1].position.y, fields[1].position.x, "%s: ", fields[1].label);
    chgat(fields[1].max_length, A_UNDERLINE, 0, NULL);
    attroff(A_BOLD | COLOR_PAIR(2));

    move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
            return 0;
        else if (ch == KEY_BACKSPACE)
        {
            if (fields[field_index].cursor_index > 0)
            {
                fields[field_index].cursor_index--;
                fields[field_index].value[fields[field_index].cursor_index] = '\0';
                move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
                addch(' ' | A_UNDERLINE);
                move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
                refresh();
            }
        }
        else if (ch == '\t')
        {
            field_index = !field_index;
            move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
        }
        else if (ch == '\n')
        {
            move(15, 0);
            clrtoeol();
            printw("%s", fields[0].value);
            move(17, 0);
            clrtoeol();
            printw("%s", fields[1].value);
            move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
        }
        else if (isalnum(ch) || ispunct(ch))
        {
            if (fields[field_index].cursor_index < fields[field_index].max_length)
            {
                fields[field_index].value[fields[field_index].cursor_index] = ch;
                fields[field_index].value[fields[field_index].cursor_index + 1] = '\0';
                if (field_index == 0)
                    addch(ch | A_UNDERLINE);
                else
                    addch('*' | A_UNDERLINE);
                fields[field_index].cursor_index++;
            }
        }
    }
}