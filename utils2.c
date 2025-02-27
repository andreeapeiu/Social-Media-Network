#include "utils.h"
#include "users.h"
#include "friends.h"
#include "posts.h"
#include <string.h>
#include "utils2.h"

/**
 * @brief Functie ce creeaza un nod nou
 * @param value - valoarea pe care o pune in nod
 * @return g_node_t* - nodul nou
 */
g_node_t *create_node(void *value)
{
	g_node_t *node = (g_node_t *)malloc(sizeof(g_node_t));
	DIE(!node, "malloc failed");
	if (node) {
		node->value = value;
		node->n_children = 0;
		node->children = NULL;
	}

	return node;
}

/**
 * @brief Functie ce gaseste un nod dupa id, intr-un arbore
 * @param root - radacina arborelui
 * @param id - id-ul nodului
 * @return g_node_t* - nodul gasit
 */
g_node_t *find_node_by_id(g_node_t *root, int id)
{
	if (!root)
		return NULL; // Tree is empty

	post_t *post = (post_t *)root->value;
	if (post->id == id)
		return root; // Found node with matching id

	// Recursively search in the children
	for (int i = 0; i < root->n_children; i++) {
		g_node_t *found = find_node_by_id(root->children[i], id);
		if (found)
			return found; // Found in one of the children
	}

	return NULL; // Not found
}

/**
 * @brief - functie ce adauga un nod la arbore
 * @param parent - parintele nodului
 * @param value - valoarea din nodul adaugat
 */
void add_child(g_node_t *parent, void *value)
{
	if (!parent) {
		fprintf(stderr, "Parent node is NULL\n");
		return;
	}

	// cresc numarul de copii
	parent->n_children++;

	// realoc memorie pentru vectorul de copii
	if (parent->n_children == 1) {
		parent->children = (g_node_t **)calloc(parent->n_children,
						   sizeof(g_node_t *));
		DIE(!parent->children, "calloc failed");
	} else {
		parent->children = (g_node_t **)realloc(parent->children,
						   parent->n_children * sizeof(g_node_t *));
		DIE(!parent->children, "realloc failed");
	}

	g_node_t *child = create_node(value);

	// adaug nodul
	parent->children[parent->n_children - 1] = child;
}

/**
 * @brief Gaseste o postare din vectorul de postari, dupa ID
 * @param post_id - id-ul postarii
 * @param posts_array - vectorul de postari
 * @param nr_posts - numarul de postari din vector
 * @return post_t* - postarea cautata
 */
post_t *find_post_by_id(int post_id, post_t **posts_array, int nr_posts)
{
	if (!posts_array)
		return NULL;

	// parcurg vectorul de structuri
	for (int i = 0; i < nr_posts; i++)
		if (posts_array[i] && posts_array[i]->id == post_id)
			return posts_array[i];

	return NULL;
}

/**
 * @brief Compara id-urile pentru doua postari
 * @param a - id-ul primei postari
 * @param b - id-ul celei de-a doua postari
 * @return int - rezultatul
 */
int compare_post_ids(const void *a, const void *b)
{
	const g_node_t *node_a = *(const g_node_t **)a;
	const g_node_t *node_b = *(const g_node_t **)b;
	const post_t *post_a = (const post_t *)node_a->value;
	const post_t *post_b = (const post_t *)node_b->value;
	return post_a->id - post_b->id;
}

/**
 * @brief Functie recursiva ce afiseaza repost-urile din arbore
 * @param node - radacina arborelui
 * @param depth - adancimea
 * @param repost_id - !
 * @param found - !
 * @param ok - !
 */
void print_reposts(g_node_t *node, int depth, int repost_id, int found, int ok)
{
	if (!node)
		return;

	post_t *post = (post_t *)node->value;

	if (post && (post->id == repost_id || found) && ok) {
		printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
		found = 1;
	}

	ok = 1;

	for (int i = 0; i < node->n_children; i++)
		print_reposts(node->children[i], depth + 1, repost_id, found, ok);
}

/**
 * @brief Functie recursiva, care gaseste postarea cu cele mai multe like-uri
 * @param node - radacina arborelui in care caut
 * @return g_node_t* - nodul cu cele mai multe like-uri
 */
g_node_t *get_most_liked_repost(g_node_t *node)
{
	g_node_t *largest_node = node;
	int largest_value = ((post_t *)(node->value))->likes;
	for (int i = 0; i < node->n_children; i++) {
		g_node_t *child_largest_node = get_most_liked_repost(node->children[i]);
		if (child_largest_node) {
			int child_largest_value =
				((post_t *)(child_largest_node->value))->likes;
			// verific daca am gasit postarea cu nr maxim de aprecieri
			if (child_largest_value > largest_value) {
				largest_value = child_largest_value;
				largest_node = child_largest_node;
			}
		}
	}
	return largest_node;
}

/**
 * @brief Functie care parcurge graful si afiseaza repostarile
 * prietenilor user-ului
 * @param node - radacina arborelui
 * @param user_id - id-ul user-ului
 * @param friends - graful de useri
 */
void traverse_graph_and_print(g_node_t *node, int user_id,
							  list_graph_t *friends)
{
	if (!node)
		return;

	post_t *post = (post_t *)(node->value);
	if (lg_has_edge(friends, post->user_id, user_id) == 1)
		printf("%s\n", get_user_name(post->user_id));

	for (int i = 0; i < node->n_children; i++)
		traverse_graph_and_print(node->children[i], user_id, friends);
}

/**
 * @brief Functie care parcurge un arbore si verifica daca
 * o postare apartine unui user
 * @param node - radacina arborelui
 * @param user_id - id-ul user-ului
 * @param is_root - daca nodul e radacina
 * @param title - titlul postarii originale
 */
void find_user_posts(g_node_t *node, int user_id, int is_root, char *title)
{
	if (!node)
		return;

	// verific sa nu fie radacina
	if (!is_root) {
		post_t *post = (post_t *)(node->value);
		if (post->user_id == user_id)
			printf("Reposted: %s\n", title);
	}

	for (int i = 0; i < node->n_children; i++)
		find_user_posts(node->children[i], user_id, 0, title);
}

/**
 * @brief Functie care elibereaza memoria ocupata de un nod din arbore
 * @param node - nodul de sters
 */
void free_tree_node(g_node_t *node)
{
	if (node) {
		for (int i = 0; i < node->n_children; ++i)
			free_tree_node(node->children[i]);

		if (node->children)
			free(node->children);

		if (node->value) {
			post_t *post = (post_t *)(node->value);
			if (post) {
				if (post->users_likes) {
					free(post->users_likes);
					post->users_likes = NULL;
				}
				free(node->value);
			}
		}
		free(node);
	}
}

/**
 * @brief Functie care elibereaza memoria ocupata de un arbore
 * @param node - arborele de sters
 */
void destroy_tree(g_tree_t *tree)
{
	if (tree && tree->root) {
		free_tree_node(tree->root);
		tree->root = NULL;
	}
}

/**
 * @brief Functie care elibereaza memoria ocupata de un nod din arbore
 * @param posts_array - vectorul de postari care trebuie eliberat
 * @param nr_posts - numarul de postari din vector
 */
void free_array_of_posts(post_t **posts_array, int nr_posts)
{
	for (int i = 0; i < nr_posts; i++) {
		if (posts_array[i]) {
			if (posts_array[i]->title)
				free(posts_array[i]->title);

			if (posts_array[i]->events) {
				destroy_tree(posts_array[i]->events);
				free(posts_array[i]->events);
			}
			free(posts_array[i]);
		}
	}
	free(posts_array);
}

/**
 * @brief Functie care sterge un nod din arbore
 * @param node - nodul de sters
 */
void delete_node(g_node_t **node)
{
	if (!node || !*node)
		return;

	free_tree_node(*node);
	*node = NULL;
}

