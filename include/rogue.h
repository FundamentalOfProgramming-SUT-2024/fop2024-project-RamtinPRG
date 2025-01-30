#ifndef ROGUE_H_
#define ROGUE_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <ncurses.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "config.h"

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

#define COLOR_WALL 100
#define COLOR_DOOR 101

#define LOGO_WIDTH 42
#define LOGO_HEIGHT 9

#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 4
#define MAX_ROOM_WIDTH 28
#define MAX_ROOM_HEIGHT 8
#define MIN_CORRIDOR_LENGTH 4
#define MAP_WIDTH 4
#define MAP_HEIGHT 3
#define MIN_ROOMS 6
#define MAX_ROOMS (MAP_WIDTH * MAP_HEIGHT)
#define FLOORS 5

#define MAP_SCREEN_WIDTH (MAP_WIDTH * (MAX_ROOM_WIDTH + 2) + (MAP_WIDTH - 1) * MIN_CORRIDOR_LENGTH)
#define MAP_SCREEN_HEIGHT (MAP_HEIGHT * (MAX_ROOM_HEIGHT + 2) + (MAP_HEIGHT - 1) * MIN_CORRIDOR_LENGTH)
#define SIDEBAR_WIDTH 50

#define SCREEN_OFFSET 2
#define GAP 2
#define MIN_SCREEN_WIDTH (SCREEN_OFFSET * 2 + MAP_SCREEN_WIDTH + GAP + SIDEBAR_WIDTH)
#define MIN_SCREEN_HEIGHT (SCREEN_OFFSET * 2 + MAP_SCREEN_HEIGHT)

// ______________ TYPES & VARIABLES___________

enum Window
{
    LOGIN,
    SIGNUP,
    MAIN_MENU,
    SCOREBOARD,
    SETTINGS,
    GAME,
};

enum Difficulty
{
    EASY,
    MEDUIM,
    HARD,
};

enum GameMode
{
    CONTINUE,
    NEW_GAME,
};

enum Direction
{
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
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

typedef struct
{
    Position position;
    bool exists;
} Door;

typedef struct
{
    Position position;
    Position block;
    int width;
    int height;
    bool visible;
    // [0]: right door, [1]: top door, [2]: left door, [3]: bottom door
    Door doors[4];
} Room;

typedef struct
{

} Corridor;

typedef struct
{
    Position position;
    Room *room;
} Stair;

typedef struct
{
    Room **rooms;
    int rooms_count;
    Stair down_stair;
    Stair up_stair;
    bool has_down_stair;
    bool has_up_stair;
} Floor;

typedef struct Character
{
    Position position;
    Position prev_position;
    cchar_t under;
    int health;
    int gold;
} Character;

extern int ch;
extern int screen_width, screen_height;
extern int current_window;
extern int previous_window;
extern int game_mode;
extern Player *player;
extern MusicSettings *music_settings;
extern ColorSettings *color_settings;
extern Settings *settings;
extern Mix_Music *music;
extern Character character;
extern Room *initial_room;
extern Floor floors[FLOORS];
extern int current_floor_index;
extern FILE *log_file;
extern FILE *map_file;

// ______________ FUNCTION PROTOTYPES ________

bool setup();
void draw_box(Position start, int width, int height);
void erase_scr();
WINDOW *init_win(int x, int y, int width, int height);
bool handle_login();
bool handle_signup();
bool handle_main_menu();
bool handle_settings();
bool handle_game();
bool exists_username(char *username);
bool validate_email(char *email);
bool validate_password(char *password);
bool add_user(char *username, char *password, char *email);
void draw_menu(WINDOW *win, MenuItem *items, int n, int selected_index);
void draw_settings(WINDOW *win, SettingItem *items, int n, int selected_index);
int nrandom(int min, int max);
void generate_map();
void generate_floor(Floor *floor, Floor *prev_floor);
bool exists_room(Floor *floor, int y, int x);
Room *get_room(Floor *floor, int y, int x);
int empty_adjacent_blocks(Floor *floor, Room *room, Position blocks[4]);
int adjacent_room_direction(Room *a, Room *b);
void draw_room(Room *room);
void draw_rooms(Floor *floor);
void draw_corridors(Floor *floor);
void draw_stairs(Floor *floor);
Position get_absolute_position(Room *room);
void remove_character();
void place_character(Position position);
void teleport_character(Position position);
void move_character(int direction);
bool ascend_character();
bool descend_character();
void setup_floor();
bool register_command(char *command, int num, ...);
void replay_commands();
void save_map(Position *position);
void load_map(Position *position);
void setup_sidebar();
Position get_sidebar_position();
void update_health();
void update_gold();
void draw_guides();
void draw_hline(Position position, int length);

#if ENABLE_MUSIC
void load_music();
#endif

#endif