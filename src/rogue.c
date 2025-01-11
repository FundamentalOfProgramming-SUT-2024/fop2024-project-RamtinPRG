#include "../include/rogue.h"

int ch;
int screen_width, screen_height;
int current_window = LOGIN;
int previous_window;
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

    init_pair(1, CUSTOM_GREEN, COLOR_BLACK);
    init_pair(2, CUSTOM_PURPLE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    init_pair(CHAR_YELLOW, CHAR_YELLOW, COLOR_BLACK);
    init_pair(CHAR_LIME, CHAR_LIME, COLOR_BLACK);
    init_pair(CHAR_TEAL, CHAR_TEAL, COLOR_BLACK);
    init_pair(CHAR_VIOLET, CHAR_VIOLET, COLOR_BLACK);
    init_pair(CHAR_PINK, CHAR_PINK, COLOR_BLACK);

    refresh();

// ____________ MUSIC INITIALIZATION _______
#if ENABLE_MUSIC
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, AUDIO_F32SYS, 2, 1024);
    load_music();
#endif

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
#endif