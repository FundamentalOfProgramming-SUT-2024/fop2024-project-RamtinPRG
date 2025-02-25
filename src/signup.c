#include "../include/rogue.h"

char get_random_char(int type)
{
    if (type == 0)
        return 'A' + rand() % 26;
    if (type == 1)
        return 'a' + rand() % 26;
    if (type == 2)
        return '0' + rand() % 10;
    return "!@#$%^&*"[rand() % 8];
}

void generate_password(char *password, int length)
{
    if (length < 7)
        length = 7;

    srand(time(NULL));

    password[0] = get_random_char(0);
    password[1] = get_random_char(1);
    password[2] = get_random_char(2);

    for (int i = 3; i < length; i++)
    {
        password[i] = get_random_char(rand() % 4);
    }

    password[length] = '\0';
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

    erase_scr();

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(2, 2, "Sign Up");
    attroff(A_BOLD | COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvprintw(5, 2, "* ");
    attron(A_ITALIC | A_UNDERLINE);
    printw("Use TAB to to switch between fields.");
    attroff(A_ITALIC | A_UNDERLINE);
    mvprintw(6, 2, "* ");
    attron(A_ITALIC | A_UNDERLINE);
    printw("Use ENTER to submit the form.");
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
        else if (ch == KEY_F(6))
        {
            char generated_password[CREDENTIALS_MAX_LENGTH + 1];
            generate_password(generated_password, 12);
            move(19, 2);
            clrtoeol();
            printw("Your password: %s", generated_password);
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
                player->continuable = false;
                player->score = 0;
                player->best_score = 0;
                player->games_count = 0;
                settings->difficulty = EASY;
                settings->color_settings->current_color_index = 0;
                settings->music_settings->current_music_index = 0;
                settings->music_settings->volume = 100;

                save_user_data();
                save_settings();
#if ENABLE_MUSIC
                set_volume();
                load_music();
#endif

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
