#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"
#include "posts.h"
#include "utils.h"
#include "utils2.h"

/**
 * @brief Functie ce creeaza o postare noua
 * @param posts_array - vectorul de postari
 * @param post_id - id-ul noii postari
 * @param nr_posts - numarul de postari existente
 * @param user_name - numele user-ului
 * @param title - titlul postarii
 * @param capacity - capacitatea curenta a vectorului de postari
 */
void create_post(post_t ***posts_array, int *post_id, int *nr_posts,
				 char *user_name, char *title, int *capacity)
{
	post_t *post = calloc(1, sizeof(post_t));
	DIE(!post, "calloc failed");

	// aloc spatiu pentru elementele de pe o pozitie a vectorului
	// vectorul are pe fiecare pozitie o structura de tip post_t
	post->id = *post_id;
	post->user_id = get_user_id(user_name);
	post->title = strdup(title);
	post->likes = 0;
	post->users_likes = calloc(MAX_PEOPLE, sizeof(int));
	DIE(!post->users_likes, "calloc failed");

	post->events = calloc(1, sizeof(g_tree_t));
	DIE(!post->events, "calloc failed");
	post->events->root = calloc(1, sizeof(g_node_t));
	DIE(!post->events->root, "calloc failed");
	post->events->root->value = post;

	// realoc vectorul de postari daca este plin
	if (*nr_posts >= *capacity) {
		int new_capacity = *capacity * 2; // dublez capacitatea
		post_t **temp = realloc(*posts_array, new_capacity * sizeof(post_t *));
		DIE(!temp, "realloc failed");
		for (int i = *capacity; i < new_capacity; i++)
			temp[i] = NULL;
		*posts_array = temp;
		*capacity = new_capacity;
	}

	(*posts_array)[*nr_posts] = calloc(1, sizeof(post_t));
	DIE(!(*posts_array)[*nr_posts], "calloc failed");
	memcpy((*posts_array)[*nr_posts], post, sizeof(*post));

	printf("Created %s for %s\n", title, user_name);
}

/**
 * @brief Functie care creeaza un repost
 * @param posts_array - vectorul de postari
 * @param user_name - numele user-ului
 * @param original - id-ul postarii la care se da repost
 * @param repost_id - id-ul repost-ului la care se da repost
 * @param nr_posts - numarul de postari din vector
 * @param post_id - id-ul postarii
 */
void repost(post_t ***posts_array, char *user_name, int original,
			int repost_id, int *nr_posts, int *post_id)
{
	post_t *repost = calloc(1, sizeof(post_t));
	DIE(!repost, "calloc failed");
	repost->id = *post_id;
	repost->user_id = get_user_id(user_name);
	repost->title = NULL;
	repost->events = NULL;
	repost->likes = 0;
	repost->users_likes = calloc(MAX_PEOPLE, sizeof(int));
	DIE(!repost->users_likes, "calloc failed");

	if (!repost_id) {
		// adaug copil la root
		for (int i = 0; i < *nr_posts; i++) {
			if ((*posts_array)[i]->id == original) {
				add_child((*posts_array)[i]->events->root, repost);
				break;
			}
		}
	} else {
		// adaug copil la nodul cu id-ul repost_id
		for (int i = 0; i < *nr_posts; i++) {
			if ((*posts_array)[i]->id == original) {
				g_node_t *node =
					find_node_by_id((*posts_array)[i]->events->root,
									repost_id);
				if (node) {
					add_child(node, repost);
					break;
				}
			}
		}
	}

	printf("Created repost #%d for %s\n", *post_id, user_name);
}

/**
 * @brief Functie ce contorizeaza numarul de like-uri
 * pentru fiecare postare si utilizatorii care au dat like
 * @param posts_array - vectorul de postari
 * @param user_name - numele user-ului
 * @param original - id-ul postarii la care se da repost
 * @param repost_id - id-ul repost-ului la care se da repost
 * @param nr_posts - numarul de postari din vector
 */
void like_function(post_t ***posts_array, char *user_name,
				   int original, int repost_id, int *nr_posts)
{
	int user_id = get_user_id(user_name);
	post_t *post = find_post_by_id(original, *posts_array, *nr_posts);

	// daca user-ul apreciaza o postare
	if (!repost_id) {
		// user-ul a apreciat postarea
		if (post->users_likes[user_id] == 0) {
			post->likes++;
			post->users_likes[user_id] = 1;
			printf("User %s liked post %s\n", user_name, post->title);
		} else { // user-ul a scos aprecierea
			post->users_likes[user_id] = 0;
			post->likes--;
			printf("User %s unliked post %s\n", user_name, post->title);
		}
	} else { // daca user-ul apreciaza un repost
		g_node_t *node = find_node_by_id(post->events->root, repost_id);
		if (((post_t *)node->value)->users_likes[user_id] == 0) {
			((post_t *)(node->value))->likes++;
			((post_t *)(node->value))->users_likes[user_id] = 1;
			printf("User %s liked repost %s\n", user_name, post->title);
		} else { // daca user-ul a scos aprecierea de la un repost
			((post_t *)(node->value))->likes--;
			((post_t *)(node->value))->users_likes[user_id] = 0;
			printf("User %s unliked repost %s\n", user_name, post->title);
		}
	}
}

/**
 * @brief Afiseaza numarul de like-uri pentru o postare
 * @param posts_array - vectorul de postari
 * @param original - id-ul postarii la care se da repost
 * @param repost_id - id-ul repost-ului la care se da repost
 * @param nr_posts - numarul de postari din vector
 */
void get_likes_function(post_t ***posts_array, int original,
						int repost_id, int *nr_posts)
{
	post_t *post = find_post_by_id(original, *posts_array, *nr_posts);
	if (!repost_id) {
		printf("Post %s has %d likes\n", post->title, post->likes);
	} else {
		g_node_t *node = find_node_by_id(post->events->root, repost_id);
		printf("Repost #%d has %d likes\n", ((post_t *)(node->value))->id,
			   ((post_t *)(node->value))->likes);
	}
}

/**
 * @brief Functie care gaseste cea mai apreciata repostare
 * @param posts_array - vectorul de postari
 * @param original - id-ul postarii la care se da repost
 * @param nr_posts - numarul de postari din vector
 */
void ratio_function(post_t ***posts_array, int original, int *nr_posts)
{
	post_t *post = find_post_by_id(original, *posts_array, *nr_posts);

	// functie care gaseste nodul din arbore cu cel mai mare nr de aprecieri
	g_node_t *largest_node = get_most_liked_repost(post->events->root);

	if (original == ((post_t *)largest_node->value)->id ||
		post->likes >= ((post_t *)largest_node->value)->likes)
		printf("The original post is the highest rated\n");
	else
		printf("Post %d got ratio'd by repost %d\n", original,
			   ((post_t *)(largest_node->value))->id);
}

/**
 * @brief Afiseaza toata ierarhia de reposturi pentru un post/repost
 * @param post_id - id-ul postarii
 * @param posts_array - vectorul de postari
 * @param nr_posts - numarul de postari din vector
 * @param repost_id - id-ul repost-ului pe care dorim sa il afisam.
 * Daca e 0, atunci nu a fost trimis de la tastatura
 */
void get_reposts_nr(int post_id, post_t **posts_array,
					int nr_posts, int repost_id)
{
	post_t *post = find_post_by_id(post_id, posts_array, nr_posts);
	int ok = 1;
	if (!post) {
		printf("Post #%d not found.\n", post_id);
		return;
	}

	int start_from_root = (repost_id == 0); //If repost_id is 0, start from root

	if (repost_id == 0) {
		printf("%s - Post by %s\n", post->title, get_user_name(post->user_id));
		ok = 0;
		print_reposts(post->events->root, 1, repost_id, -1, ok);
	} else {
		print_reposts(post->events->root, 1, repost_id, start_from_root, ok);
	}
}

/**
 * @brief Sterge un anumit repost si toate nodurile copil
 * @param node - nodul de la care incepe cautarea
 * @param repost_id - id-ul repost-ului pe care dorim sa il stergem
 */
void delete_repost(g_node_t **node, int repost_id)
{
	if (!node || !*node)
		return;

	g_node_t *current = *node;
	for (int i = 0; i < current->n_children; i++) {
		post_t *post = (post_t *)(current->children[i]->value);
		if (post->id == repost_id) {
			delete_node(&current->children[i]);
			for (int j = i; j < current->n_children - 1; j++)
				current->children[j] = current->children[j + 1];

			current->n_children--;
			return;
		}
		delete_repost(&current->children[i], repost_id);
	}
}

/**
 * @brief Sterge o anumita postare sau un repost si toate nodurile copil
 * @param posts_array - vectorul de postari
 * @param nr_posts - numarul de postari din vector
 * @param repost_id - id-ul repost-ului pe care dorim sa il stergem.
 * Daca e 0, atunci nu a fost trimis de la tastatura
 */
void delete(int post_id, post_t **posts_array, int *nr_posts, int repost_id)
{
	for (int i = 0; i < *nr_posts; i++) {
		if (posts_array[i]->id == post_id) {
			if (repost_id == 0) {
				// Sterg o postare
				printf("Deleted post %s\n", posts_array[i]->title);
				destroy_tree(posts_array[i]->events);
				for (int j = i; j < *nr_posts - 1; j++)
					posts_array[j] = posts_array[j + 1];

				(*nr_posts)--;
			} else {
				// Sterg un repost
				printf("Deleted repost #%d of post %s\n", repost_id,
					   posts_array[i]->title);
				delete_repost(&posts_array[i]->events->root,
							  repost_id);
			}
			return;
		}
	}
}

g_node_t *common_repost(g_node_t *root, int node1_id, int node2_id) {
	if (!root)
		return NULL;

	post_t *post = (post_t *)root->value;

	// Daca e unul dintre ele atunci il returneaza
	if (post->id == node1_id || post->id == node2_id)
		return root;

	// Recursive search in children
	g_node_t *left = NULL;
	g_node_t *right = NULL;
	for (int i = 0; i < root->n_children; i++) {
		g_node_t *temp = common_repost(root->children[i], node1_id, node2_id);
		if (temp) {
			if (!left)
				left = temp;
			else
				right = temp;
		}
	}

	// Daca s-au gasit si left si right atunci nodul curent este cel mai
	// apropiat stramos
	if (left && right)
		return root;

	if (left)
		return left;
	else
		return right;
}

void handle_input_posts(char *input, post_t ***posts_array, int *post_id,
						int *nr_posts, int *capacity)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *user_name = strtok(NULL, " ");
		char *title = strtok(NULL, "\n");
		create_post(posts_array, post_id, nr_posts, user_name, title, capacity);
		(*nr_posts)++;
		(*post_id)++;
	} else if (!strcmp(cmd, "repost")) {
		char *user_name = strtok(NULL, " ");
		int original = atoi(strtok(NULL, " "));
		int repost_id = 0;
		char *aux = strtok(NULL, "\n");
		if (aux)
			repost_id = atoi(aux);
		repost(posts_array, user_name, original, repost_id, nr_posts, post_id);
		(*post_id)++;
	} else if (!strcmp(cmd, "common-repost")) {
		int original = atoi(strtok(NULL, " "));
		int repost_id1 = atoi(strtok(NULL, " "));
		int repost_id2 = atoi(strtok(NULL, "\n"));
		post_t *common = find_post_by_id(original, *posts_array, *nr_posts);
		g_node_t *root = common->events->root;
		g_node_t *common_r = common_repost(root, repost_id1, repost_id2);

		if (!common_r) {
			printf("No common repost found\n");
		} else {
			post_t *post = (post_t *)common_r->value;
			printf("The first common repost of %d and %d is %d\n",
				   repost_id1, repost_id2, post->id);
		}
	} else if (!strcmp(cmd, "like")) {
		char *user_name = strtok(NULL, " ");
		int original = atoi(strtok(NULL, " "));
		int repost_id = 0;
		char *aux = strtok(NULL, "\n");
		if (aux)
			repost_id = atoi(aux);
		like_function(posts_array, user_name, original, repost_id, nr_posts);
	} else if (!strcmp(cmd, "ratio")) {
		int original_id = atoi(strtok(NULL, " "));
		ratio_function(posts_array, original_id, nr_posts);
	} else if (!strcmp(cmd, "delete")) {
		int post = atoi(strtok(NULL, " "));
		char *aux = strtok(NULL, "\n");
		int repost_id = 0;
		if (aux)
			repost_id = atoi(aux);
		delete(post, *posts_array, nr_posts, repost_id);
	} else if (!strcmp(cmd, "get-likes")) {
		int original = atoi(strtok(NULL, " "));
		int repost_id = 0;
		char *aux = strtok(NULL, "\n");
		if (aux)
			repost_id = atoi(aux);
		get_likes_function(posts_array, original, repost_id, nr_posts);
	} else if (!strcmp(cmd, "get-reposts")) {
		int post = atoi(strtok(NULL, " "));
		int repost_id = 0;
		char *aux = strtok(NULL, "\n");
		if (aux)
			repost_id = atoi(aux);
		get_reposts_nr(post, *posts_array, *nr_posts, repost_id);
	}
	free(commands);
}
