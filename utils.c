
#include "utils.h"
#include "users.h"
#include "friends.h"
#include <string.h>

linked_list_t *ll_create(unsigned int data_size)
{
	linked_list_t *ll;
	ll = malloc(sizeof(*ll));
	DIE(!ll, "malloc failed");
	ll->head = NULL;
	ll->data_size = data_size;
	ll->size = 0;

	return ll;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se returneaza
 * ultimul nod.
 */
ll_node_t *get_nth_node(linked_list_t *list, unsigned int n)
{
	if (!list)
		return NULL;

	unsigned int len = list->size - 1;
	unsigned int i;
	ll_node_t *node = list->head;

	n = MIN(n, len);

	for (i = 0; i < n; ++i)
		node = node->next;

	return node;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Dac
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data)
{
	ll_node_t *prev, *curr;
	ll_node_t *new_node;

	if (!list)
		return;

	/* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
	if (n > list->size)
		n = list->size;

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "malloc failed");
	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "malloc failed");
	memcpy(new_node->data, new_data, list->data_size);

	new_node->next = curr;
	if (!prev)
		list->head = new_node;
	else
		prev->next = new_node;

	list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre aces
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t *
ll_remove_nth_node(linked_list_t *list, unsigned int n)
{
	ll_node_t *prev, *curr;

	if (!list || !list->head)
		return NULL;

	/* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
	if (n > list->size - 1)
		n = list->size - 1;

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (!prev)
		/* Adica n == 0. */
		list->head = curr->next;
	else
		prev->next = curr->next;

	list->size--;

	return curr;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t *list)
{
	if (!list)
		return -1;

	return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t **pp_list)
{
	ll_node_t *curr_node;

	if (!pp_list || !*pp_list)
		return;

	while ((*pp_list)->head) {
		curr_node = (*pp_list)->head;
		(*pp_list)->head = curr_node->next;

		free(curr_node->data); // Free the data pointer
		free(curr_node);		  // Free the node itself
	}

	free(*pp_list);	 // Free the linked list struct
	*pp_list = NULL; // Null out the caller's pointer
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t *list)
{
	ll_node_t *curr;

	if (!list)
		return;

	curr = list->head;
	while (curr) {
		printf("%d ", *((int *)curr->data));
		curr = curr->next;
	}

	printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t *list)
{
	ll_node_t *curr;

	if (!list)
		return;

	curr = list->head;
	while (!curr) {
		printf("%s ", (char *)curr->data);
		curr = curr->next;
	}

	printf("\n");
}

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = calloc(1, sizeof(*q));
	DIE(!q, "calloc queue failed");

	q->data_size = data_size;
	q->max_size = max_size;

	q->buff = malloc(max_size * sizeof(*q->buff));
	DIE(!q->buff, "malloc buffer failed");

	return q;
}

unsigned int
q_get_size(queue_t *q)
{
	return !q ? 0 : q->size;
}

unsigned int
q_is_empty(queue_t *q)
{
	return !q ? 1 : !q->size;
}

void *
q_front(queue_t *q)
{
	if (!q || !q->size)
		return NULL;

	return q->buff[q->read_idx];
}

int q_dequeue(queue_t *q)
{
	if (!q || !q->size)
		return 0;

	free(q->buff[q->read_idx]);

	q->read_idx = (q->read_idx + 1) % q->max_size;
	--q->size;
	return 1;
}

int q_enqueue(queue_t *q, void *new_data)
{
	void *data;
	if (!q || q->size == q->max_size)
		return 0;

	data = malloc(q->data_size);
	DIE(!data, "malloc data failed");
	memcpy(data, new_data, q->data_size);

	q->buff[q->write_idx] = data;
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++q->size;

	return 1;
}

void q_clear(queue_t *q)
{
	unsigned int i;
	if (!q || !q->size)
		return;

	for (i = q->read_idx; i != q->write_idx; i = (i + 1) % q->max_size)
		free(q->buff[i]);

	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
}

void q_free(queue_t *q)
{
	if (!q)
		return;

	q_clear(q);
	free(q->buff);
	free(q);
}

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru lista de adiacenta a grafului.
 */
list_graph_t *
lg_create(int nodes)
{
	list_graph_t *graph = calloc(1, sizeof(list_graph_t));
	DIE(!graph, "calloc failed");
	graph->nodes = nodes;
	graph->neighbors = calloc(nodes, sizeof(linked_list_t *));
	DIE(!graph->neighbors, "calloc failed");
	for (int i = 0; i < nodes; i++)
		graph->neighbors[i] = ll_create(sizeof(int));

	return graph;
}

static int is_node_in_graph(int n, int nodes)
{
	return n >= 0 && n < nodes;
}

/* Adauga o muchie intre nodurile primite ca parametri */
void lg_add_edge(list_graph_t *graph, int src, int dest)
{
	if (!graph || !graph->neighbors || !is_node_in_graph(src, graph->nodes) ||
		!is_node_in_graph(dest, graph->nodes))
		return;

	linked_list_t *neighbor_list = graph->neighbors[src];
	ll_node_t *current = neighbor_list->head;
	int position = 0;

	while (current && *((int *)current->data) < dest) {
		current = current->next;
		position++;
	}

	if (!current)
		position = graph->neighbors[src]->size;

	ll_add_nth_node(graph->neighbors[src], position, &dest);
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int lg_has_edge(list_graph_t *graph, int src, int dest)
{
	ll_node_t *it = graph->neighbors[src]->head;
	for (unsigned int i = 0; i < graph->neighbors[src]->size; i++) {
		if (*(int *)it->data == dest)
			return 1;
		it = it->next;
	}
	return 0;
}

/* Elimina muchia dintre nodurile primite ca parametri */
void lg_remove_edge(list_graph_t *graph, int src, int dest)
{
	ll_node_t *it = graph->neighbors[src]->head; // scot leg primul nod
	unsigned int i;
	for (i = 0; i < graph->neighbors[src]->size; i++)
		if (*(int *)it->data == dest)
			break;
	ll_node_t *nod = ll_remove_nth_node(graph->neighbors[src], i);
	free(nod->data);
	free(nod);
}

/* Elibereaza memoria folosita de lista de adiacenta a grafului */
void lg_free(list_graph_t *graph)
{
	if (!graph)
		return;

	for (int i = 0; i < graph->nodes; i++)
		if (graph->neighbors[i])
			ll_free(&graph->neighbors[i]);

	free(graph->neighbors);
	free(graph);
}

/* Printeaza lista de adiacenta a grafului
 */
void lg_print_graph(list_graph_t *graph)
{
	ll_node_t *it;
	for (int i = 0; i < graph->nodes; i++) {
		printf("%d: ", i);
		it = graph->neighbors[i]->head;
		for (unsigned int j = 0; j < graph->neighbors[i]->size; j++) {
			printf("%d ", *(int *)it->data);
			it = it->next;
		}
		printf("\n");
	}
}

/*
	TODO

	Prints the nodes which make up the minimum path between src and dest.

	For example, if the minimum path between A and B is A->C->D->E->F->B, then
	this function will print "A C D E F B".

	Output format:

	node_1 node_2 node_3 ... node_k

	For the purpose of finding minimum paths, the given graph
	can be either oriented or unoriented. For this exercise, the graph
	is oriented.
*/
void print_min_path(list_graph_t *graph, int src, int dest, int *distance)
{
	int stare[MAX_PEOPLE] = {0};
	int d[MAX_PEOPLE];
	queue_t *q = q_create(sizeof(int), MAX_PEOPLE);

	for (int i = 0; i < graph->nodes; i++)
		d[i] = -1;

	stare[src] = 1;
	d[src] = 0;
	q_enqueue(q, &src);

	while (!q_is_empty(q)) {
		int v = *(int *)q_front(q);
		q_dequeue(q);
		ll_node_t *temp = graph->neighbors[v]->head;

		while (temp) {
			int u = *(int *)(temp->data);
			if (stare[u] == 0) {
				stare[u] = 1;
				d[u] = d[v] + 1;
				q_enqueue(q, &u);
			}
			temp = temp->next;
		}
	}

	if (d[dest] == -1)
		*distance = -1;
	else
		*distance = d[dest];

	q_free(q);
}
