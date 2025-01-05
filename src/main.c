#include "../include/rogue.h"

int ch;
int screen_width, screen_height;
int current_window = LOGIN;
Player *player;

int main()
{
    // Initializing
    player = (Player *)malloc(sizeof(Player));
    player->signed_in = false;
    screen_setup();

    // Mainloop
    while (1)
    {
        if (!player->signed_in)
        {
            if (current_window == LOGIN)
            {
                if (!handle_login())
                    break;
            }
            else if (current_window == SIGNUP)
            {
                if (!handle_signup())
                    break;
            }
        }
        else if (current_window == MAIN_MENU)
        {
            if (!handle_main_menu())
                break;
        }
    }

    endwin();
    return 0;
}

bool handle_login()
{
    Field fields[2];
    int field_index = 0;

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

    erase();

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
    curs_set(1);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            curs_set(0);
            return 0;
        }
        else if (ch == KEY_F(4))
        {
            current_window = SIGNUP;
            curs_set(0);
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
            char username[CREDENTIALS_MAX_LENGTH + 1];
            char entered_password[CREDENTIALS_MAX_LENGTH + 1];
            strcpy(username, fields[0].value);
            strcpy(entered_password, fields[1].value);

            move(15, 2);
            clrtoeol();
            attron(A_STANDOUT | COLOR_PAIR(4));

            if (strlen(username) == 0)
                printw("  The 'username' field is not filled!  ");
            else if (strlen(entered_password) == 0)
                printw("  The 'password' field is not filled!  ");
            else if (exists_username(username))
            {
                FILE *file;
                char file_name[100] = "users/", password[CREDENTIALS_MAX_LENGTH + 2], email[EMAIL_MAX_LENGTH + 2];
                strcat(file_name, username);
                strcat(file_name, ".txt");
                file = fopen(file_name, "r");
                fgets(password, CREDENTIALS_MAX_LENGTH + 2, file);
                fgets(email, EMAIL_MAX_LENGTH + 2, file);
                fclose(file);
                int len = strlen(password);
                if (password[len - 1] == '\n')
                    password[len - 1] = '\0';
                len = strlen(email);
                if (email[len - 1] == '\n')
                    email[len - 1] = '\0';
                if (strcmp(password, entered_password) == 0)
                {
                    attroff(A_STANDOUT | COLOR_PAIR(4));
                    player->signed_in = true;
                    strcpy(player->username, username);
                    strcpy(player->password, entered_password);
                    strcpy(player->email, email);
                    current_window = MAIN_MENU;
                    curs_set(0);
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

bool handle_signup()
{
    int field_index = 0, total_indices = 4;
    Field fields[total_indices];

    fields[0].label = (char *)calloc(50, sizeof(char));
    fields[0].label = "Username";
    fields[0].position.y = 10;
    fields[0].position.x = 2;
    fields[0].cursor_index = 0;
    fields[0].max_length = CREDENTIALS_MAX_LENGTH;
    fields[0].value = (char *)calloc(fields[0].max_length + 1, sizeof(char));

    fields[1].label = (char *)calloc(50, sizeof(char));
    fields[1].label = "Email";
    fields[1].position.y = 12;
    fields[1].position.x = 2;
    fields[1].cursor_index = 0;
    fields[1].max_length = EMAIL_MAX_LENGTH;
    fields[1].value = (char *)calloc(fields[1].max_length + 1, sizeof(char));

    fields[2].label = (char *)calloc(50, sizeof(char));
    fields[2].label = "Password";
    fields[2].position.y = 14;
    fields[2].position.x = 2;
    fields[2].cursor_index = 0;
    fields[2].max_length = CREDENTIALS_MAX_LENGTH;
    fields[2].value = (char *)calloc(fields[2].max_length + 1, sizeof(char));

    fields[3].label = (char *)calloc(50, sizeof(char));
    fields[3].label = "Confirm Password";
    fields[3].position.y = 16;
    fields[3].position.x = 2;
    fields[3].cursor_index = 0;
    fields[3].max_length = CREDENTIALS_MAX_LENGTH;
    fields[3].value = (char *)calloc(fields[3].max_length + 1, sizeof(char));

    erase();

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(2, 2, "Sign Up");
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
    mvprintw(8, 2, "Already have an account? Press F4 to login.");

    attron(A_BOLD | COLOR_PAIR(2));
    for (int i = 0; i < total_indices; i++)
    {
        mvprintw(fields[i].position.y, fields[i].position.x, "%s: ", fields[i].label);
        chgat(fields[i].max_length, A_UNDERLINE, 0, NULL);
    }
    attroff(A_BOLD | COLOR_PAIR(2));

    move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
    curs_set(1);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
        {
            curs_set(0);
            return 0;
        }
        else if (ch == KEY_F(4))
        {
            current_window = LOGIN;
            curs_set(0);
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
            field_index++;
            if (field_index >= total_indices)
                field_index = 0;
            move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
        }
        else if (ch == '\n')
        {
            char username[CREDENTIALS_MAX_LENGTH + 1];
            char entered_password[CREDENTIALS_MAX_LENGTH + 1];
            char confirm_password[CREDENTIALS_MAX_LENGTH + 1];
            char email[EMAIL_MAX_LENGTH + 1];
            strcpy(username, fields[0].value);
            strcpy(email, fields[1].value);
            strcpy(entered_password, fields[2].value);
            strcpy(confirm_password, fields[3].value);

            move(19, 2);
            clrtoeol();
            attron(A_STANDOUT | COLOR_PAIR(4));

            if (strlen(username) == 0)
                printw("  The 'username' field is not filled!  ");
            else if (strlen(email) == 0)
                printw("  The 'email' field is not filled!  ");
            else if (strlen(entered_password) == 0)
                printw("  The 'password' field is not filled!  ");
            else if (strlen(confirm_password) == 0)
                printw("  The 'confirm password' field is not filled!  ");
            else if (exists_username(username))
                printw("  The username '%s' already exists!  ", username);
            else if (!validate_email(email))
                printw("  The email is not valid!  ");
            else if (strlen(entered_password) < 7)
                printw("  Password is too short (Minimum 7 characters)  ");
            else if (!validate_password(entered_password))
                printw("  Password should contain at least 1 capital, 1 small and 1 digit.  ");
            else if (strcmp(entered_password, confirm_password) != 0)
                printw("  The fields 'password' and 'confirm password' don't match!  ");
            else
            {
                attroff(A_STANDOUT | COLOR_PAIR(4));
                add_user(username, entered_password, email);
                player->signed_in = true;
                strcpy(player->username, username);
                strcpy(player->password, entered_password);
                strcpy(player->email, email);
                current_window = MAIN_MENU;
                curs_set(0);
                return 1;
            }

            attroff(A_STANDOUT | COLOR_PAIR(4));
            move(fields[field_index].position.y, fields[field_index].position.x + strlen(fields[field_index].label) + fields[field_index].cursor_index + 2);
        }
        else if (isalnum(ch) || ispunct(ch))
        {
            if (fields[field_index].cursor_index < fields[field_index].max_length)
            {
                fields[field_index].value[fields[field_index].cursor_index] = ch;
                fields[field_index].value[fields[field_index].cursor_index + 1] = '\0';
                if (field_index < 2)
                    addch(ch | A_UNDERLINE);
                else
                    addch('*' | A_UNDERLINE);
                fields[field_index].cursor_index++;
            }
        }
    }
}

bool handle_main_menu()
{
    char menu_items[][21] = {"New Game", "Continue", "Scoreboard", "Settings", "Exit"};
    int items_count = 5, item_index = 0, menu_width = 20;
    MenuItem items[items_count];

    for (int i = 0; i < items_count; i++)
    {
        items[i].label = (char *)calloc(50, sizeof(char));
        strcpy(items[i].label, menu_items[i]);
        items[i].position.y = 12 + i * 2;
        items[i].position.x = 2;
    }

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
    draw_menu(stdscr, items, items_count, item_index);

    while (1)
    {
        ch = getch();
        if (ch == KEY_F(1))
            return false;
        else if (ch == KEY_UP)
        {
            item_index--;
            if (item_index < 0)
                item_index += items_count;
            draw_menu(stdscr, items, items_count, item_index);
        }
        else if (ch == KEY_DOWN)
        {
            item_index++;
            if (item_index >= items_count)
                item_index -= items_count;
            draw_menu(stdscr, items, items_count, item_index);
        }
        else if (ch == '\n')
        {
            if (item_index == 4)
                return false;
        }
    }

    return false;
}

void draw_menu(WINDOW *win, MenuItem *items, int n, int selected_index)
{
    attron(A_ITALIC);
    for (int i = 0; i < n; i++)
    {
        if (i == selected_index)
        {
            attron(A_BOLD | COLOR_PAIR(3));
            mvwprintw(win, items[i].position.y, items[i].position.x, " • ");
            wprintw(win, "%s", items[i].label);
            attroff(A_BOLD | COLOR_PAIR(3));
            continue;
        }
        attron(A_DIM);
        mvwprintw(win, items[i].position.y, items[i].position.x, "   ");
        wprintw(win, "%s", items[i].label);
        attroff(A_DIM);
    }
    attroff(A_ITALIC);
}