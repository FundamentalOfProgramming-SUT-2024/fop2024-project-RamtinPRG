#include "../include/rogue.h"

FILE *user_data_file;

void load_user_data(char *username, Player *player)
{
    int continuable;

    char user_data_file_name[50] = "users_data/";
    strcat(user_data_file_name, username);
    strcat(user_data_file_name, ".txt");

    user_data_file = fopen(user_data_file_name, "r");
    fscanf(user_data_file, "%d", &continuable);
    fscanf(user_data_file, "%d", &player->gold);
    fscanf(user_data_file, "%d", &player->score);
    fscanf(user_data_file, "%d", &player->best_score);
    fscanf(user_data_file, "%d", &player->games_count);
    fscanf(user_data_file, "%d", &player->victories_count);
    fscanf(user_data_file, "%d", &player->defeats_count);
    player->continuable = (bool)continuable;

    fclose(user_data_file);
}

void save_user_data()
{
    char user_data_file_name[50] = "users_data/";
    strcat(user_data_file_name, player->username);
    strcat(user_data_file_name, ".txt");

    user_data_file = fopen(user_data_file_name, "w");
    fprintf(user_data_file, "%d\n", player->continuable);
    fprintf(user_data_file, "%d\n", player->gold);
    fprintf(user_data_file, "%d\n", player->score);
    fprintf(user_data_file, "%d\n", player->best_score);
    fprintf(user_data_file, "%d\n", player->games_count);
    fprintf(user_data_file, "%d\n", player->victories_count);
    fprintf(user_data_file, "%d\n", player->defeats_count);

    fclose(user_data_file);
}

int file_lines_count(FILE *file)
{
    long x = ftell(file);

    if (file == NULL)
        return -1;

    int lines = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF)
        if (ch == '\n')
            lines++;

    if (lines > 0)
    {
        fseek(file, -1, SEEK_END);
        if (fgetc(file) != '\n')
            lines++;
    }

    fseek(file, 0, x);

    return lines;
}

bool get_file_line(FILE *file, int line_number)
{
    if (file == NULL || line_number < 1)
        return -1;

    rewind(file);
    int current_line = 1;
    int ch;

    while (current_line < line_number && (ch = fgetc(file)) != EOF)
        if (ch == '\n')
            current_line++;

    return (current_line == line_number) ? true : false;
}
