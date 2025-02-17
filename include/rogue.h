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
#define CHAR_SLATE 35
#define CHAR_BRONZE 36
#define CHAR_SILVER 37

#define COLOR_WALL 100
#define COLOR_DOOR 101

#define LOGO_WIDTH 42
#define LOGO_HEIGHT 9
#define GAMEOVER_WIDTH 73
#define GAMEOVER_HEIGHT 10
#define VICTORY_WIDTH 57
#define VICTORY_HEIGHT 10

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
#define MESSAGES_HEIGHT 4

#define SCREEN_OFFSET 2
#define H_GAP 2
#define V_GAP 1
#define MIN_SCREEN_WIDTH (SCREEN_OFFSET * 2 + MAP_SCREEN_WIDTH + H_GAP + SIDEBAR_WIDTH)
#define MIN_SCREEN_HEIGHT (SCREEN_OFFSET * 2 + MAP_SCREEN_HEIGHT + V_GAP + MESSAGES_HEIGHT)

#define MIN_TRAP_DAMAGE 5
#define MAX_TRAP_DAMAGE 10

#define GOLDS_TO_ROOMS_RATIO 4
#define ROOMS_TO_GOLDS_RATIO 3
#define MIN_GOLD_VALUE 20
#define MAX_GOLD_VALUE 50

#define BLACK_GOLDS_TO_ROOMS_RATIO 1
#define ROOMS_TO_BLACK_GOLDS_RATIO 10
#define MIN_BLACK_GOLD_VALUE 100
#define MAX_BLACK_GOLD_VALUE 300

#define MIN_FOOD_VALUE 20
#define MAX_FOOD_VALUE 40

#define SCOREBOARD_ITEM_PER_PAGE 10

// ______________ TYPES & VARIABLES___________

enum Window
{
    LOGIN,
    SIGNUP,
    MAIN_MENU,
    SCOREBOARD,
    SETTINGS,
    PROFILE,
    GAME,
    DEFEAT,
    VICTORY
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

enum RoomType
{
    REGULAR_ROOM,
    ENCHANT_ROOM,
    TREASURE_ROOM,
};

enum WeaponTypes
{
    MACE,
    DAGGER,
    WAND,
    ARROW,
    SWORD
};

enum PotionTypes
{
    HEALTH_POTION,
    SPEED_POTION,
    DAMAGE_POTION
};

enum FoodTypes
{
    REGULAR_FOOD,
    SUPER_FOOD,
    MAGICAL_FOOD,
    ROTTEN_FOOD
};

enum SidebarList
{
    GUIDES,
    FOODS,
    WEAPONS,
    DIRECTIONS,
    POTIONS,
};

typedef struct
{
    char username[CREDENTIALS_MAX_LENGTH + 5];
    char password[CREDENTIALS_MAX_LENGTH + 5];
    char email[EMAIL_MAX_LENGTH + 1];
    bool signed_in;
    bool continuable;
    int gold;
    int score;
    int best_score;
    int games_count;
    int victories_count;
    int defeats_count;
} Player;

typedef struct
{
    char **musics;
    int current_music_index;
    int musics_count;
    int volume;
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
    bool secret;
    bool is_discovered;
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
    int type;
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

typedef struct Trap
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    bool is_discovered;
} Trap;

typedef struct Gold
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int value;
    bool is_discovered;
} Gold;

typedef struct BlackGold
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int value;
    bool is_discovered;
} BlackGold;

typedef struct Food
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int value;
    bool is_picked;
    bool is_eaten;
    int type;
} Food;

typedef struct Weapon
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    int type;
    bool is_picked;
    bool in_hand;
    int count;
} Weapon;

typedef struct Potion
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int type;
    bool is_picked;
    bool is_consumed;
    bool is_being_consumed;
    int start_time;
} Potion;

typedef struct Daemon
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    int health;
    bool is_alive;
    cchar_t under;
} Daemon;

typedef struct FireMonster
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    int health;
    bool is_alive;
    cchar_t under;
} FireMonster;

typedef struct Snake
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    int health;
    bool is_alive;
    cchar_t under;
} Snake;

typedef struct Giant
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    int health;
    bool is_alive;
    cchar_t under;
    bool is_chasing;
    int chasing_start;
} Giant;

typedef struct Undeed
{
    Floor *floor;
    Room *room;
    int floor_index;
    int room_index;
    Position position;
    int damage;
    int health;
    bool is_alive;
    cchar_t under;
    bool is_chasing;
    int chasing_start;
} Undeed;

typedef struct Character
{
    Position position;
    Position prev_position;
    cchar_t under;
    int health;
    int stomach;
    int gold;
    int score;
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
extern int score_multiplier;
extern Room *initial_room;
extern Floor floors[FLOORS];
extern Trap *traps;
extern int traps_count;
extern Gold *golds;
extern int golds_count;
extern BlackGold *black_golds;
extern int black_golds_count;
extern Food *foods;
extern int foods_count;
extern Weapon *weapons;
extern int weapons_count;
extern Potion *potions;
extern int potions_count;
extern Daemon *daemons;
extern int daemons_count;
extern FireMonster *fire_monsters;
extern int fire_monsters_count;
extern Snake *snakes;
extern int snakes_count;
extern Giant *giants;
extern int giants_count;
extern Undeed *undeeds;
extern int undeeds_count;
extern int timeline_counter;
extern int speed_potion;
extern int health_potion;
extern int damage_potion;
extern int current_floor_index;
extern FILE *log_file;
extern FILE *map_file;
extern char game_message[20][500];
extern int game_message_count;

extern cchar_t trap_character;
extern cchar_t ground_character;

// ______________ FUNCTION PROTOTYPES ________

bool setup();
void erase_box(Position start, int width, int height);
void draw_box(Position start, int width, int height);
void erase_scr();
WINDOW *init_win(int x, int y, int width, int height);
bool handle_login();
bool handle_signup();
bool handle_main_menu();
bool handle_settings();
bool handle_profile();
bool handle_game();
bool handle_defeat();
bool handle_victory();
bool handle_scoreboard();
bool exists_username(char *username);
bool validate_email(char *email);
bool validate_password(char *password);
bool add_user(char *username, char *password, char *email);
void load_user_data(char *username, Player *player);
void save_user_data();
void load_settings();
void save_settings();
bool get_file_line(FILE *file, int line_number);
int file_lines_count(FILE *file);
void draw_menu(WINDOW *win, MenuItem *items, int n, int selected_index);
void draw_settings(WINDOW *win, SettingItem *items, int n, int selected_index);
int nrandom(int min, int max);
void generate_map();
void generate_floor(Floor *floor, Floor *prev_floor);
void generate_traps();
void generate_golds();
void generate_black_golds();
void generate_foods();
void generate_weapons();
void generate_potions();
void generate_daemons();
void generate_fire_monsters();
void generate_snakes();
void generate_giants();
void generate_undeeds();
bool exists_gold(Floor *floor, Room *room, Position position);
bool exists_black_gold(Floor *floor, Room *room, Position position);
bool exists_trap(Floor *floor, Room *room, Position position);
bool exists_stair(Floor *floor, Room *room, Position position);
bool exists_food(Floor *floor, Room *room, Position position);
bool exists_weapon(Floor *floor, Room *room, Position position);
bool exists_potion(Floor *floor, Room *room, Position position);
bool exists_daemon(Floor *floor, Room *room, Position position);
bool exists_fire_monster(Floor *floor, Room *room, Position position);
bool exists_snake(Floor *floor, Room *room, Position position);
bool exists_giant(Floor *floor, Room *room, Position position);
bool exists_undeed(Floor *floor, Room *room, Position position);
bool exists_monster(Floor *floor, Room *room, Position position);
bool exists_item(Floor *floor, Room *room, Position position);
bool exists_room(Floor *floor, int y, int x);
Room *get_room(Floor *floor, int y, int x);
int empty_adjacent_blocks(Floor *floor, Room *room, Position blocks[4]);
int adjacent_room_direction(Room *a, Room *b);
void game_exit_routine();
void show_room(Room *room);
void draw_room(Room *room, int walls_color_pair);
void draw_rooms(Floor *floor);
void draw_corridors(Floor *floor);
void draw_stairs(Floor *floor);
void draw_traps(Floor *floor);
void draw_golds(Floor *floor);
void draw_black_golds(Floor *floor);
void draw_foods(Floor *floor);
void draw_weapons(Floor *floor);
void draw_potions(Floor *floor);
void draw_daemons(Floor *floor);
void draw_fire_monsters(Floor *floor);
void draw_snakes(Floor *floor);
void draw_giants(Floor *floor);
void draw_undeeds(Floor *floor);
Room *get_current_room();
int get_current_room_index();
Room *get_container_room();
Position get_absolute_position(Room *room);
void remove_character();
void place_character(Position position);
void teleport_character(Position position);
void move_character(int direction, char *message);
bool ascend_character(char *message);
bool descend_character(char *message);
void pick_character(char *message);
void eat_character(int index, char *message);
void short_attack_character();
void long_attack_character(int direction);
void eat_food();
void take_weapon();
void use_potion();
int inventory_foods_count();
Food *food_inventory_by_index(int index);
Weapon *get_current_weapon();
void setup_floor();
bool register_command(char *command, int num, ...);
void replay_commands();
void save_map(Position *position);
void load_map(Position *position);
void setup_sidebar(int list);
Position get_sidebar_position();
void update_health();
void update_stomach();
void update_gold();
void update_score();
void update_weapon();
void draw_guides();
void draw_food_inventory();
void draw_weapon_inventory();
void draw_directions();
void draw_potion_inventory();
void draw_hline(Position position, int length);
void setup_message_box(bool replay_mode);
void add_message(char *message);
void setup_scoreboard(Player players[]);
int compare_players(const void *ap, const void *bp);
int compare_strings(char *a, char *b);

#if ENABLE_MUSIC
void load_music();
void set_volume();
#endif

#endif