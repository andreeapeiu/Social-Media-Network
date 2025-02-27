#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "friends.h"
#include "posts.h"
#include "utils.h"
#include "utils2.h"

/**
 * @brief Afiseaza cele mai recente <feed-size> postari ale unui
 * utilizator si ale prietenilor lui.
 * @param posts_array - vectorul de postari
 * @param friends - graful de useri
 * @param nr_posts - numarul de postari din vector
 * @param name - numele user-ului
 * @param feed_size - numarul de activitati afisate
 */
void get_feed(post_t ***posts_array, list_graph_t **friends,
			  int *nr_posts, char *name, int feed_size)
{
	// incep de la sfarsit, pentru a accesa cea mai recenta postare
	for (int i = *nr_posts - 1; i >= 0; i--) {
		// verific daca user-ul este un prieten sau daca postarea
		// apartine user-ului cautat
		if (lg_has_edge(*friends, (*posts_array)[i]->user_id,
						get_user_id(name)) == 1 || (*posts_array)[i]->user_id ==
						get_user_id(name)) {
			feed_size--;
			printf("%s: %s\n", get_user_name((*posts_array)[i]->user_id),
				   (*posts_array)[i]->title);
		}
		if (feed_size == 0)
			break;
	}
}

/**
 * @brief Afiseaza toti prietenii unui utilizator care au dat
 * repost la postarea cu <post-id>-ul primit.
 * @param posts_array - vectorul de postari
 * @param friends - graful de useri
 * @param nr_posts - numarul de postari din vector
 * @param name - numele user-ului
 * @param post_id - id-ul postarii
 */
void friends_repost(post_t ***posts_array, list_graph_t **friends,
					int *nr_posts, char *name, int post_id)
{
	int user_id = get_user_id(name);
	post_t *post = find_post_by_id(post_id, *posts_array, *nr_posts);
	traverse_graph_and_print(post->events->root, user_id, *friends);
}

/**
 * @brief Afiseaza toate postarile si repostarile unui utilizator.
 * @param posts_array - vectorul de postari
 * @param nr_posts - numarul de postari din vector
 * @param name - numele user-ului
 */
void view_profile(post_t ***posts_array, int *nr_posts, char *name)
{
	int user_id = get_user_id(name);
	for (int i = 0; i < *nr_posts; i++)
		if ((*posts_array)[i]->user_id == user_id)
			printf("Posted: %s\n", (*posts_array)[i]->title);

	for (int i = 0; i < *nr_posts; i++) {
		post_t *post = (*posts_array)[i];
		char *title = post->title;
		find_user_posts(post->events->root, user_id, 1, title);
	}
}

void bron_kerbosch(list_graph_t **friends, int *R, int R_size, int *P,
				   int P_size, int *X, int X_size, int user_id, int *max_clique,
				   int *max_clique_size)
{
	// fara pivotare

	// daca P si X sunt goale, R este max_clique
	if (P_size == 0 && X_size == 0) {
		int find_user = 0;
		// verific daca R contine user-ul
		for (int i = 0; i < R_size; i++) {
			if (R[i] == user_id) {
				find_user = 1;
				break;
			}
		}
		// daca am gasit max_clique mai mare, il schimb
		if (find_user == 1 && R_size > *max_clique_size) {
			*max_clique_size = R_size;
			for (int j = 0; j < R_size; j++)
				max_clique[j] = R[j];
		}
		return;
	}

	// parcurg fiecare nod din P
	for (int i = 0; i < P_size; i++) {
		int v = P[i];
		// reuniunea
		// adaug v in R
		R[R_size] = v;

		int P_new[MAX_PEOPLE], X_new[MAX_PEOPLE];

		int P_new_size = 0, X_new_size = 0;

		// intersectia
		// pun in P_new doar vecinii lui V
		for (int j = 0; j < P_size; j++) {
			if (lg_has_edge(*friends, v, P[j])) {
				P_new[P_new_size] = P[j];
				P_new_size++;
			}
		}

		// pun in X_new doar vecinii lui V
		for (int j = 0; j < X_size; j++) {
			if (lg_has_edge(*friends, v, X[j])) {
				X_new[X_new_size] = X[j];
				X_new_size++;
			}
		}

		bron_kerbosch(friends, R, R_size + 1, P_new, P_new_size, X_new,
					  X_new_size, user_id, max_clique, max_clique_size);

		// scot v din P
		for (int j = 0; j < P_size; j++) {
			if (P[j] == v) {
				P_size--;
				P[j] = P[P_size];
				break;
			}
		}
		// adaug v la X
		X[X_size++] = v;
	}
}

/**
 * @brief Functie care gaseste cea mai mare grup de prieteni ce
 * include un user
 * @param friends - graful de useri
 * @param name - numele user-ului
 */
void common_groups(list_graph_t **friends, char *name)
{
	int user_id = get_user_id(name);
	int max_clique_size = 0;

	// intializez vectorii de care am nevoie pentru a aplica algoritmul
	int max_clique[MAX_PEOPLE], R[MAX_PEOPLE], P[MAX_PEOPLE], X[MAX_PEOPLE];

	int R_size = 0, X_size = 0;
	for (int i = 0; i < (*friends)->nodes; i++)
		P[i] = i;

	int P_size = (*friends)->nodes;

	// algoritmul Bron Kerbosch
	bron_kerbosch(friends, R, R_size, P, P_size, X, X_size, user_id,
				  max_clique, &max_clique_size);

	printf("The closest friend group of %s is:\n", name);

	// sortare crescatoare dupa user_id
	for (int i = 0; i < max_clique_size - 1; i++) {
		for (int j = i + 1; j < max_clique_size; j++) {
			if (max_clique[i] > max_clique[j]) {
				int aux = max_clique[i];
				max_clique[i] = max_clique[j];
				max_clique[j] = aux;
			}
		}
	}

	for (int i = 0; i < max_clique_size; i++)
		printf("%s\n", get_user_name(max_clique[i]));
}

void handle_input_feed(char *input, list_graph_t **friends,
					   post_t ***posts_array, int *nr_posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name = strtok(NULL, "\n ");
		int feed_size = atoi(strtok(NULL, "\n "));
		get_feed(posts_array, friends, nr_posts, name, feed_size);
	} else if (!strcmp(cmd, "view-profile")) {
		char *name = strtok(NULL, "\n ");
		view_profile(posts_array, nr_posts, name);
	} else if (!strcmp(cmd, "friends-repost")) {
		char *name = strtok(NULL, "\n ");
		int post_id = atoi(strtok(NULL, "\n "));
		friends_repost(posts_array, friends, nr_posts, name, post_id);
	} else if (!strcmp(cmd, "common-group")) {
		char *name = strtok(NULL, "\n ");
		common_groups(friends, name);
	}

	free(commands);
}
