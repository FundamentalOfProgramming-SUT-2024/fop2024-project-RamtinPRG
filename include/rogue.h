#ifndef ROGUE_H_
#define ROGUE_H_

#include <ncurses.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// _______________ DEFINES ___________________

#define CREDENTIALS_MAX_LENGTH 16
#define EMAIL_MAX_LENGTH 50
#define PASSWORD_MIN_LENGTH 7
#define MAX_USERS 256
#define MAX_MUSICS 8

#define CUSTOM_PURPLE 8
#define CUSTOM_GREEN 9

#define CHAR_YELLOW 30
#define CHAR_LIME 31
#define CHAR_TEAL 32
#define CHAR_VIOLET 33
#define CHAR_PINK 34

// ______________ TYPES & VARIABLES___________

enum Window
{
    LOGIN,
    SIGNUP,
    MAIN_MENU,
    SCOREBOARD,
    SETTINGS,
};

enum Difficulty
{
    EASY,
    MEDUIM,
    HARD,
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
    char **musics;
    int current_music_index;
    int musics_count;
} MusicSettings;

typedef struct
{
    char **colors;
    int *color_number;
    int current_color_index;
    int color_counts;
} ColorSettings;

typedef struct
{
    MusicSettings *music_settings;
    ColorSettings *color_settings;
    int difficulty;
} Settings;

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

typedef struct
{
    Position position;

    char *label;
    int label_length;

    char **options;
    int option_length;
    int option_index;
    int options_count;

    int label_color_pair_index;
    int option_color_pair_index;
} SettingItem;

extern int ch;
extern int screen_width, screen_height;
extern int current_window;
extern int previous_window;
extern Player *player;
extern MusicSettings *music_settings;
extern ColorSettings *color_settings;
extern Settings *settings;
extern Mix_Music *music;

// ______________ FUNCTION PROTOTYPES ________

bool setup();
WINDOW *init_win(int x, int y, int width, int height);
bool handle_login();
bool handle_signup();
bool handle_main_menu();
bool handle_settings();
bool exists_username(char *username);
bool validate_email(char *email);
bool validate_password(char *password);
bool add_user(char *username, char *password, char *email);
void draw_menu(WINDOW *win, MenuItem *items, int n, int selected_index);
void draw_settings(WINDOW *win, SettingItem *items, int n, int selected_index);
void load_music();

#endif