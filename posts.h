#include "utils2.h"
#include "utils.h"

#ifndef POSTS_H
#define POSTS_H

void create_post(post_t ***posts_array, int *post_id, int *nr_posts,
				 char *user_name, char *title, int *capacity);

void repost(post_t ***posts_array, char *user_name, int original,
			int repost_id, int *nr_posts, int *post_id);

void like_function(post_t ***posts_array, char *user_name,
				   int original, int repost_id, int *nr_posts);

void get_likes_function(post_t ***posts_array, int original,
						int repost_id, int *nr_posts);

void ratio_function(post_t ***posts_array, int original, int *nr_posts);

void get_reposts_nr(int post_id, post_t **posts_array,
					int nr_posts, int repost_id);

void delete_repost(g_node_t **node, int repost_id);

void delete(int post_id, post_t **posts_array, int *nr_posts, int repost_id);

g_node_t *common_repost(g_node_t *root, int node1_id, int node2_id);

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, post_t ***posts_array, int *post_id,
						int *nr_posts, int *capacity);

#endif // POSTS_H
