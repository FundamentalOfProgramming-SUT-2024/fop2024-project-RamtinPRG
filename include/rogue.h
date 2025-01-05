#ifndef ROGUE_H_
#define ROGUE_H_

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

#define CUSTOM_PURPLE 8
#define CUSTOM_GREEN 9

// ______________ TYPES & VARIABLES___________

enum Window
{
    LOGIN,
    SIGNUP,
    MAIN_MENU,
    SCOREBOARD,
    SETTINGS,
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

typedef struct
{
    Position position;
    char *label;
    int width;
} MenuItem;

extern int ch;
extern int screen_width, screen_height;
extern int current_window;
extern Player *player;

// ______________ FUNCTION PROTOTYPES ________

bool screen_setup();
WINDOW *init_win(int x, int y, int width, int height);
bool handle_login();
bool handle_signup();
bool handle_main_menu();
bool exists_username(char *username);
bool validate_email(char *email);
bool validate_password(char *password);
bool add_user(char *username, char *password, char *email);
void draw_menu(WINDOW *win, MenuItem *items, int n, int selected_index);

#endif