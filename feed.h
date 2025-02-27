#ifndef FEED_H
#define FEED_H

#include "utils.h"
#include "utils2.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/

void get_feed(post_t ***posts_array, list_graph_t **friends,
			  int *nr_posts, char *name, int feed_size);

void friends_repost(post_t ***posts_array, list_graph_t **friends,
					int *nr_posts, char *name, int post_id);

void view_profile(post_t ***posts_array, int *nr_posts, char *name);

void bron_kerbosch(list_graph_t **friends, int *R, int R_size, int *P,
				   int P_size, int *X, int X_size, int user_id, int *max_clique,
				   int *max_clique_size);

void common_groups(list_graph_t **friends, char *name);

void handle_input_feed(char *input, list_graph_t **friends,
					   post_t ***posts_array, int *nr_posts);

#endif // FEED_H
