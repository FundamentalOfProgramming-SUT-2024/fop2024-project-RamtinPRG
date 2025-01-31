#include "../include/rogue.h"

FILE *user_data_file;

void load_user_data()
{
    int continuable;

    char user_data_file_name[50] = "users_data/";
    strcat(user_data_file_name, player->username);
    strcat(user_data_file_name, ".txt");

    user_data_file = fopen(user_data_file_name, "r");
    fscanf(user_data_file, "%d", &continuable);
    fscanf(user_data_file, "%d", &player->score);
    fscanf(user_data_file, "%d", &player->best_score);
    fscanf(user_data_file, "%d", &player->games_count);
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
    fprintf(user_data_file, "%d\n", player->score);
    fprintf(user_data_file, "%d\n", player->best_score);
    fprintf(user_data_file, "%d\n", player->games_count);

    fclose(user_data_file);
}