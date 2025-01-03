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
bool exists_username(char *username);

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
                    break;
            }
            else if (current_window == SIGNUP)
            {
                clear();
                mvprintw(2, 2, "SIGNUP");
                getch();
                break;
            }
        }
        else
        {
            clear();
            mvprintw(2, 2, "Have fun playing the game :)");
            getch();
            break;
        }
    }

    endwin();
    return 0;
}

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
    init_pair(4, COLOR_RED, COLOR_BLACK);

    fields[0].label = (char *)calloc(50, sizeof(char));
    fields[0].label = "Username";
    fields[0].position.y = 10;
    fields[0].position.x = 2;
    fields[0].cursor_index = 0;
    fields[0].max_length = CREDENTIALS_MAX_LENGTH;
    fields[0].value = (char *)calloc(fields[0].max_length + 1, sizeof(char));

    fields[1].label = (char *)calloc(50, sizeof(char));
    fields[1].label = "Password";
    fields[1].position.y = 12;
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
    mvprintw(8, 2, "Don't have an account? Press F4 to register.");

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
        else if (ch == KEY_F(4))
        {
            current_window = SIGNUP;
            return 1;
        }
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
            move(15, 2);
            clrtoeol();
            attron(A_STANDOUT | COLOR_PAIR(4));

            if (strlen(fields[0].value) == 0)
                printw("  The 'username' field is not filled!  ");
            else if (strlen(fields[1].value) == 0)
                printw("  The 'password' field is not filled!  ");
            else if (exists_username(fields[0].value))
            {
                FILE *file;
                char file_name[100] = "users/", password[CREDENTIALS_MAX_LENGTH + 2];
                strcat(file_name, fields[0].value);
                strcat(file_name, ".txt");
                file = fopen(file_name, "r");
                fgets(password, CREDENTIALS_MAX_LENGTH + 2, file);
                fclose(file);
                int len = strlen(password);
                if (password[len - 1] == '\n')
                    password[len - 1] = '\0';
                if (strcmp(password, fields[1].value) == 0)
                {
                    attroff(A_STANDOUT | COLOR_PAIR(4));
                    signed_in = 1;
                    current_window = GAME;
                    return 1;
                }
                else
                    printw("  Incorrect password!  ");
            }
            else
                printw("  Username does not exist!  ");

            attroff(A_STANDOUT | COLOR_PAIR(4));
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