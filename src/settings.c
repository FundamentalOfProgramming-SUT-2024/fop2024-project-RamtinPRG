#include "../include/rogue.h"

bool handle_settings()
{
    bool applied = true;
    int items_count = 3, item_index = 0, labels_length = 30, options_length = 15;
    char settings_labels[][21] = {"Difficulty", "Character's color", "Music"};
    SettingItem items[items_count];

    for (int i = 0; i < items_count; i++)
    {
        items[i].label = (char *)calloc(50, sizeof(char));
        items[i].label_length = labels_length;
        items[i].option_length = options_length;
        items[i].option_index = 0;
        items[i].label_color_pair_index = 2;
        items[i].option_color_pair_index = 2;
        strcpy(items[i].label, settings_labels[i]);
        items[i].position.y = 18 + i * 2;
        items[i].position.x = 2;
    }

    items[0].options_count = 3;
    items[0].options = (char **)malloc(items[0].options_count * sizeof(char *));
    for (int i = 0; i < items[0].options_count; i++)
        items[0].options[i] = (char *)malloc((options_length + 1) * sizeof(char));
    strcpy(items[0].options[0], "Easy");
    strcpy(items[0].options[1], "Medium");
    strcpy(items[0].options[2], "Hard");
    items[0].option_index = settings->difficulty;

    items[1].options_count = color_settings->color_counts;
    items[1].options = (char **)malloc(items[1].options_count * sizeof(char *));
    for (int i = 0; i < items[1].options_count; i++)
    {
        items[1].options[i] = (char *)malloc((options_length + 1) * sizeof(char));
        strcpy(items[1].options[i], color_settings->colors[i]);
    }
    items[1].label_color_pair_index = color_settings->current_color_index + CHAR_YELLOW;
    items[1].option_color_pair_index = color_settings->current_color_index + CHAR_YELLOW;
    items[1].option_index = color_settings->current_color_index;

    items[2].options_count = music_settings->musics_count;
    items[2].options = (char **)malloc(items[2].options_count * sizeof(char *));
    for (int i = 0; i < items[2].options_count; i++)
    {
        items[2].options[i] = (char *)malloc((options_length + 1) * sizeof(char));
        strcpy(items[2].options[i], music_settings->musics[i]);
    }
    items[2].option_index = music_settings->current_music_index;

    erase();

    attron(COLOR_PAIR(4));
    mvprintw(1, 2, " ██▀███   ▒█████    ▄████  █    ██ ▓█████ \n");
    mvprintw(2, 2, "▓██ ▒ ██▒▒██▒  ██▒ ██▒ ▀█▒ ██  ▓██▒▓█   ▀ \n");
    mvprintw(3, 2, "▓██ ░▄█ ▒▒██░  ██▒▒██░▄▄▄░▓██  ▒██░▒███   \n");
    mvprintw(4, 2, "▒██▀▀█▄  ▒██   ██░░▓█  ██▓▓▓█  ░██░▒▓█  ▄ \n");
    mvprintw(5, 2, "░██▓ ▒██▒░ ████▓▒░░▒▓███▀▒▒▒█████▓ ░▒████▒\n");
    mvprintw(6, 2, "░ ▒▓ ░▒▓░░ ▒░▒░▒░  ░▒   ▒ ░▒▓▒ ▒ ▒ ░░ ▒░ ░\n");
    mvprintw(7, 2, "  ░▒ ░ ▒░  ░ ▒ ▒░   ░   ░ ░░▒░ ░ ░  ░ ░  ░\n");
    mvprintw(8, 2, "  ░░   ░ ░ ░ ░ ▒  ░ ░   ░  ░░░ ░ ░    ░   \n");
    mvprintw(9, 2, "   ░         ░ ░        ░    ░        ░  ░\n");
    attroff(COLOR_PAIR(4));

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(12, 2, "Settings");
    attroff(A_BOLD | COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvprintw(14, 2, "* ");
    attron(A_ITALIC | A_UNDERLINE);
    printw("Press F4 to exit the settings, press ENTER to apply the settings.");
    attroff(A_ITALIC | A_UNDERLINE);
    mvprintw(15, 2, "* ");
    attron(A_ITALIC | A_UNDERLINE);
    printw("Use arrow keys to change the settings.");
    attroff(A_ITALIC | A_UNDERLINE | COLOR_PAIR(3));

    draw_settings(stdscr, items, items_count, item_index);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
            return false;
        else if (ch == KEY_F(4))
        {
            current_window = previous_window;
            return 1;
        }
        else if (ch == '\n')
        {
            settings->difficulty = items[0].option_index;
            color_settings->current_color_index = items[1].option_index;
            if (music_settings->current_music_index != items[2].option_index)
            {
                music_settings->current_music_index = items[2].option_index;
#if ENABLE_MUSIC
                load_music();
#endif
            }
            if (!applied)
            {
                mvprintw(25, 2, "Changes applied!");
                applied = true;
            }
        }
        else if (ch == KEY_UP)
        {
            item_index--;
            if (item_index < 0)
                item_index += items_count;
            draw_settings(stdscr, items, items_count, item_index);
        }
        else if (ch == KEY_DOWN)
        {
            item_index++;
            if (item_index >= items_count)
                item_index -= items_count;
            draw_settings(stdscr, items, items_count, item_index);
        }
        else if (ch == KEY_RIGHT)
        {
            if (applied)
            {
                move(25, 0);
                clrtoeol();
                applied = false;
            }
            items[item_index].option_index++;
            if (items[item_index].option_index >= items[item_index].options_count)
                items[item_index].option_index -= items[item_index].options_count;
            if (item_index == 1)
            {
                items[item_index].option_color_pair_index = items[item_index].option_index + CHAR_YELLOW;
                items[item_index].label_color_pair_index = items[item_index].option_index + CHAR_YELLOW;
            }
            draw_settings(stdscr, items, items_count, item_index);
        }
        else if (ch == KEY_LEFT)
        {
            if (applied)
            {
                move(25, 0);
                clrtoeol();
                applied = false;
            }
            items[item_index].option_index--;
            if (items[item_index].option_index < 0)
                items[item_index].option_index += items[item_index].options_count;
            if (item_index == 1)
            {
                items[item_index].option_color_pair_index = items[item_index].option_index + CHAR_YELLOW;
                items[item_index].label_color_pair_index = items[item_index].option_index + CHAR_YELLOW;
            }
            draw_settings(stdscr, items, items_count, item_index);
        }
    }

    return false;
}

void draw_settings(WINDOW *win, SettingItem *items, int n, int selected_index)
{
    for (int i = 0; i < n; i++)
    {
        if (i == selected_index)
        {
            attron(A_BOLD | COLOR_PAIR(items[i].label_color_pair_index));
            mvwprintw(win, items[i].position.y, items[i].position.x, " • ");
            wprintw(win, "%s", items[i].label);
            attroff(COLOR_PAIR(items[i].label_color_pair_index));
            int whitespace = items[i].label_length - strlen(items[i].label);
            int x, y;
            getyx(win, y, x);
            attron(COLOR_PAIR(items[i].option_color_pair_index));
            mvwprintw(win, y, x + whitespace, " 🞀 ");
            wprintw(win, "%s", items[i].options[items[i].option_index]);
            whitespace = items[i].option_length - strlen(items[i].options[items[i].option_index]);
            for (int j = 0; j < whitespace; j++)
                wprintw(win, " ");
            wprintw(win, " 🞂 ");
            attroff(A_BOLD | COLOR_PAIR(items[i].option_color_pair_index));
            continue;
        }
        attron(A_DIM);
        mvwprintw(win, items[i].position.y, items[i].position.x, "   ");
        wprintw(win, "%s", items[i].label);
        int whitespace = items[i].label_length - strlen(items[i].label);
        int x, y;
        getyx(win, y, x);
        mvwprintw(win, y, x + whitespace, " 🞀 ");
        wprintw(win, "%s", items[i].options[items[i].option_index]);
        whitespace = items[i].option_length - strlen(items[i].options[items[i].option_index]);
        for (int j = 0; j < whitespace; j++)
            wprintw(win, " ");
        wprintw(win, " 🞂 ");
        attroff(A_DIM);
    }
}