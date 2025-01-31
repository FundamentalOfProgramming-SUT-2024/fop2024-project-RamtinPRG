#include "../include/rogue.h"

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

    erase_scr();

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(2, 2, "Login");
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
                fscanf(file, "%s", password);
                fscanf(file, "%s", email);
                fclose(file);
                if (strcmp(password, entered_password) == 0)
                {
                    attroff(A_STANDOUT | COLOR_PAIR(4));
                    player->signed_in = true;
                    strcpy(player->username, username);
                    strcpy(player->password, entered_password);
                    strcpy(player->email, email);

                    load_user_data();

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