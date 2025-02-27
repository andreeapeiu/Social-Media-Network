#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 10000
#define MAX_PEOPLE 550

#include "utils.h"

void add_friend(list_graph_t **friends, char *name_1, char *name_2);

void remove_friend(list_graph_t **friends, char *name_1, char *name_2);

void find_distance(list_graph_t *friends, char *name_1, char *name_2);

void get_nr_friends(list_graph_t *friends, char *name);

void find_common_friends(list_graph_t *friends, char *name_1, char *name_2);

void get_popular(list_graph_t *friends, char *name);

void get_suggestions(list_graph_t *friends, char *name);

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t **friends);

#endif // FRIENDS_H
