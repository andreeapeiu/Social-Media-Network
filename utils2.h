#ifndef UTILS2_H
#define UTILS2_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef struct g_node_t g_node_t;
struct g_node_t
{
	void *value;
	g_node_t **children;
	int n_children;
};

typedef struct g_tree_t g_tree_t;
struct g_tree_t
{
	g_node_t *root;
};

typedef struct post_t post_t;
struct post_t {
	int id;
	char *title;
	int user_id;
	g_tree_t *events;
	int likes;
	int *users_likes;       // oamenii care au dat like
};

g_node_t *create_node(void *value);

g_node_t *find_node_by_id(g_node_t *root, int id);

void add_child(g_node_t *parent, void *value);

post_t *find_post_by_id(int post_id, post_t **posts_array, int nr_posts);

int compare_post_ids(const void *a, const void *b);

void print_reposts(g_node_t *node, int depth, int repost_id, int found, int ok);

void get_reposts_nr(int post_id, post_t **posts_array, int nr_posts,
					int repost_id);

g_node_t *get_most_liked_repost(g_node_t *node);

void traverse_graph_and_print(g_node_t *node, int user_id,
							  list_graph_t *friends);

void find_user_posts(g_node_t *node, int user_id, int is_root, char *title);

void free_tree_node(g_node_t *node);

void destroy_tree(g_tree_t *tree);

void free_array_of_posts(post_t **posts_array, int nr_posts);

void delete_node(g_node_t **node);

#endif /* UTILS2_H */

