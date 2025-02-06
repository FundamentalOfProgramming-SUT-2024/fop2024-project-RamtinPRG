#include "../include/rogue.h"

int ch;
int screen_width, screen_height;
int current_window = LOGIN;
int previous_window;
int game_mode;
Player *player;
MusicSettings *music_settings;
ColorSettings *color_settings;
Settings *settings;
Mix_Music *music;

// ______________ FUNCTIONS __________________

bool setup()
{
    setlocale(LC_ALL, "");

    // ________ SETTINGS _________
    player = (Player *)malloc(sizeof(Player));
    player->signed_in = false;

    music_settings = (MusicSettings *)malloc(sizeof(MusicSettings));
    music_settings->current_music_index = 0;
    music_settings->musics_count = 3;
    music_settings->musics = (char **)malloc(MAX_MUSICS * sizeof(char *));
    for (int i = 0; i < music_settings->musics_count; i++)
        music_settings->musics[i] = (char *)malloc(20 * sizeof(char));
    music_settings->musics[0] = "Noise.mp3";
    music_settings->musics[1] = "Yay.mp3";
    music_settings->musics[2] = "PISTA.mp3";

    color_settings = (ColorSettings *)malloc(sizeof(ColorSettings));
    color_settings->current_color_index = 0;
    color_settings->color_counts = 5;
    color_settings->color_number = (int *)malloc(color_settings->color_counts * sizeof(int));
    color_settings->color_number[0] = CHAR_YELLOW;
    color_settings->color_number[1] = CHAR_LIME;
    color_settings->color_number[2] = CHAR_TEAL;
    color_settings->color_number[3] = CHAR_VIOLET;
    color_settings->color_number[4] = CHAR_PINK;
    color_settings->colors = (char **)malloc(color_settings->color_counts * sizeof(char *));
    for (int i = 0; i < color_settings->color_counts; i++)
        color_settings->colors[i] = (char *)malloc(10 * sizeof(char));
    color_settings->colors[0] = "Yellow";
    color_settings->colors[1] = "Lime";
    color_settings->colors[2] = "Teal";
    color_settings->colors[3] = "Violet";
    color_settings->colors[4] = "Pink";

    settings = (Settings *)malloc(sizeof(Settings));
    settings->music_settings = music_settings;
    settings->color_settings = color_settings;
    settings->difficulty = EASY;

    // ________ NCURSES __________
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, screen_height, screen_width);

    if (screen_height < MIN_SCREEN_HEIGHT || screen_width < MIN_SCREEN_WIDTH)
    {
        char string[200] = "You're screen is too small. Press any key to exit!";
        move(screen_height / 2, (screen_width - strlen(string)) / 2);
        printw("%s", string);
        getch();
        endwin();
        exit_curses(1);
        return 0;
    }

    if (!has_colors())
    {
        char string[200] = "Your terminal doesn't support colors. Press any key to exit!";
        move(screen_height / 2, (screen_width - strlen(string)) / 2);
        printw("%s", string);
        getch();
        endwin();
        exit_curses(1);
        return 0;
    }

    start_color();

    // Overwriting black
    init_color(COLOR_BLACK, 90, 78, 129);
    // Overwriting red
    init_color(COLOR_RED, 862, 149, 149);
    // Purple
    init_color(CUSTOM_PURPLE, 623, 588, 902);
    // Green
    init_color(CUSTOM_GREEN, 459, 796, 533);

    // Character colors
    init_color(CHAR_YELLOW, 917, 701, 31);
    init_color(CHAR_LIME, 517, 800, 86);
    init_color(CHAR_TEAL, 78, 721, 650);
    init_color(CHAR_VIOLET, 545, 360, 964);
    init_color(CHAR_PINK, 925, 282, 600);
    init_color(CHAR_SLATE, 384, 454, 556);
    init_color(CHAR_BRONZE, 1000, 341, 200);
    init_color(CHAR_SILVER, 752, 752, 752);

    // Game items
    init_color(COLOR_WALL, 203, 443, 921);
    init_pair(COLOR_WALL, COLOR_WALL, COLOR_BLACK);
    init_color(COLOR_DOOR, 611, 313, 949);
    init_pair(COLOR_DOOR, COLOR_DOOR, COLOR_BLACK);

    init_pair(1, CUSTOM_GREEN, COLOR_BLACK);
    init_pair(2, CUSTOM_PURPLE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    init_pair(CHAR_YELLOW, CHAR_YELLOW, COLOR_BLACK);
    init_pair(CHAR_LIME, CHAR_LIME, COLOR_BLACK);
    init_pair(CHAR_TEAL, CHAR_TEAL, COLOR_BLACK);
    init_pair(CHAR_VIOLET, CHAR_VIOLET, COLOR_BLACK);
    init_pair(CHAR_PINK, CHAR_PINK, COLOR_BLACK);
    init_pair(CHAR_SLATE, CHAR_SLATE, COLOR_BLACK);
    init_pair(CHAR_BRONZE, CHAR_BRONZE, COLOR_BLACK);
    init_pair(CHAR_SILVER, CHAR_SILVER, COLOR_BLACK);

    refresh();

    // ____________ MUSIC INITIALIZATION _______

#if ENABLE_MUSIC
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, AUDIO_F32SYS, 2, 1024);
#endif

    // ____________ RANDOM SEED ________________
    srand(time(NULL));

    return 1;
}

void erase_box(Position start, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
                mvprintw(i + start.y, j + start.x, "╭");
            else if (i == 0 && j == width - 1)
                mvprintw(i + start.y, j + start.x, "╮");
            else if (i == height - 1 && j == width - 1)
                mvprintw(i + start.y, j + start.x, "╯");
            else if (i == height - 1 && j == 0)
                mvprintw(i + start.y, j + start.x, "╰");
            else if (i == 0 || i == height - 1)
                mvprintw(i + start.y, j + start.x, "─");
            else if (j == 0 || j == width - 1)
                mvprintw(i + start.y, j + start.x, "│");
            else
                mvprintw(i + start.y, j + start.x, " ");
        }
    }
}

void draw_box(Position start, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
                mvprintw(i + start.y, j + start.x, "╭");
            else if (i == 0 && j == width - 1)
                mvprintw(i + start.y, j + start.x, "╮");
            else if (i == height - 1 && j == width - 1)
                mvprintw(i + start.y, j + start.x, "╯");
            else if (i == height - 1 && j == 0)
                mvprintw(i + start.y, j + start.x, "╰");
            else if (i == 0 || i == height - 1)
                mvprintw(i + start.y, j + start.x, "─");
            else if (j == 0 || j == width - 1)
                mvprintw(i + start.y, j + start.x, "│");
        }
    }
}

void erase_scr()
{
    erase();
    attron(COLOR_PAIR(COLOR_WALL));
    draw_box(((Position){0, 0}), MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
    attroff(COLOR_PAIR(COLOR_WALL));
}

WINDOW *init_win(int x, int y, int width, int height)
{
    WINDOW *win = newwin(height, width, y, x);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void to_lowercase(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        if (isalnum(str[i]) && isupper(str[i]))
            str[i] += 'a' - 'A';
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

    char username_copy[100];
    strcpy(username_copy, username);
    to_lowercase(username_copy);

    char lines[MAX_USERS][CREDENTIALS_MAX_LENGTH + 2];
    int line_index = 0;
    while (fgets(lines[line_index], CREDENTIALS_MAX_LENGTH + 2, file) != NULL)
    {
        int len = strlen(lines[line_index]);
        if (lines[line_index][len - 1] == '\n')
            lines[line_index][len - 1] = '\0';
        to_lowercase(lines[line_index]);
        if (strcmp(lines[line_index], username_copy) == 0)
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
    fprintf(file, "%s\n", username);
    fclose(file);
    char file_name[100] = "users/";
    strcat(file_name, username);
    strcat(file_name, ".txt");
    file = fopen(file_name, "w");
    fprintf(file, "%s\n", password);
    fprintf(file, "%s\n", email);
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
            attron(A_BOLD | COLOR_PAIR(2));
            mvwprintw(win, items[i].position.y, items[i].position.x, " • ");
            wprintw(win, "%s", items[i].label);
            attroff(A_BOLD | COLOR_PAIR(2));
            continue;
        }
        attron(A_DIM);
        mvwprintw(win, items[i].position.y, items[i].position.x, "   ");
        wprintw(win, "%s", items[i].label);
        attroff(A_DIM);
    }
    attroff(A_ITALIC);
}

int nrandom(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void draw_room(Room *room, int walls_color_pair)
{
    int height = room->height + 2, width = room->width + 2;
    Position position = get_absolute_position(room);
    int x = position.x;
    int y = position.y;

    attron(COLOR_PAIR(walls_color_pair) | A_BOLD);
    draw_box(position, width, height);
    attroff(COLOR_PAIR(walls_color_pair) | A_BOLD);

    attron(A_DIM);
    for (int i = 0; i < room->height; i++)
        for (int j = 0; j < room->width; j++)
            mvprintw(i + position.y + 1, j + position.x + 1, ".");
    attroff(A_DIM);

    attron(COLOR_PAIR(COLOR_DOOR));
    for (int i = 0; i < 4; i++)
        if (room->doors[i].exists)
            mvprintw(y + room->doors[i].position.y, x + room->doors[i].position.x, "+");
    attroff(COLOR_PAIR(COLOR_DOOR));
}

void draw_rooms(Floor *floor)
{
    for (int i = 0; i < floor->rooms_count; i++)
    {
        if (floor->rooms[i]->type == REGULAR_ROOM)
            draw_room(floor->rooms[i], COLOR_WALL);
        else if (floor->rooms[i]->type == TREASURE_ROOM)
            draw_room(floor->rooms[i], CHAR_YELLOW);
    }
}

Position get_absolute_position(Room *room)
{
    Position position;
    position.x = SCREEN_OFFSET + room->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + room->position.x;
    position.y = SCREEN_OFFSET + room->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + room->position.y;
    return position;
}

void draw_corridors(Floor *floor)
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH - 1; j++)
        {
            Room *a = get_room(floor, i, j);
            if (a != NULL && a->doors[0].exists)
            {
                Room *b = get_room(floor, i, j + 1);
                Position door_a, door_b;
                door_a = get_absolute_position(a);
                door_a.x += a->doors[0].position.x + 1;
                door_a.y += a->doors[0].position.y;
                door_b = get_absolute_position(b);
                door_b.x += b->doors[2].position.x - 1;
                door_b.y += b->doors[2].position.y;
                int first_half = (door_b.x - door_a.x + 1) / 2;
                int second_half = door_b.x - door_a.x + 1 - first_half;
                move(door_a.y, door_a.x);
                for (int k = 0; k < first_half; k++)
                {
                    printw("░");
                }
                if (door_b.y > door_a.y)
                    for (int k = 0; k < door_b.y - door_a.y; k++)
                    {
                        mvprintw(door_a.y + k + 1, door_a.x + first_half - 1, "░");
                    }
                else if (door_b.y < door_a.y)
                    for (int k = 0; k < door_a.y - door_b.y; k++)
                    {
                        mvprintw(door_a.y - k - 1, door_a.x + first_half - 1, "░");
                    }
                move(door_b.y, door_a.x + first_half);
                for (int k = 0; k < second_half; k++)
                {
                    printw("░");
                }
            }
        }
    }

    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT - 1; j++)
        {
            Room *a = get_room(floor, j, i);
            if (a != NULL && a->doors[3].exists)
            {
                Room *b = get_room(floor, j + 1, i);
                Position door_a, door_b;
                door_a = get_absolute_position(a);
                door_a.x += a->doors[3].position.x;
                door_a.y += a->doors[3].position.y + 1;
                door_b = get_absolute_position(b);
                door_b.x += b->doors[1].position.x;
                door_b.y += b->doors[1].position.y - 1;
                int first_half = (door_b.y - door_a.y + 1) / 2;
                int second_half = door_b.y - door_a.y + 1 - first_half;
                for (int k = 0; k < first_half; k++)
                {
                    mvprintw(door_a.y + k, door_a.x, "░");
                }
                if (door_b.x > door_a.x)
                    for (int k = 0; k < door_b.x - door_a.x; k++)
                    {
                        mvprintw(door_a.y + first_half - 1, door_a.x + k + 1, "░");
                    }
                else if (door_b.x < door_a.x)
                    for (int k = 0; k < door_a.x - door_b.x; k++)
                    {
                        mvprintw(door_a.y + first_half - 1, door_a.x - k - 1, "░");
                    }
                for (int k = 0; k < second_half; k++)
                {
                    mvprintw(door_a.y + first_half + k, door_b.x, "░");
                }
            }
        }
    }
}

#if ENABLE_MUSIC
void load_music()
{
    char music_file_name[100] = "musics/";
    strcat(music_file_name, music_settings->musics[music_settings->current_music_index]);
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = Mix_LoadMUS(music_file_name);
    Mix_PlayMusic(music, -1);
}

void set_volume()
{
    Mix_VolumeMusic(music_settings->volume);
}
#endif