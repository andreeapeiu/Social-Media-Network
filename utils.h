#ifndef UTILS_H
#define UTILS_H

static inline int MIN(int x, int y) {
	return x < y ? x : y;
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIE(assertion, call_description)                       \
	do                                                         \
	{                                                          \
		if (assertion)                                         \
		{                                                      \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);                          \
			exit(errno);                                       \
		}                                                      \
	} while (0)

typedef struct ll_node_t
{
	void *data;
	struct ll_node_t *next;
} ll_node_t;

typedef struct linked_list_t
{
	ll_node_t *head;
	unsigned int data_size;
	unsigned int size;
} linked_list_t;

typedef struct
{
	linked_list_t **neighbors; /* Listele de adiacenta ale grafului */
	int nodes;				   /* Numarul de noduri din graf. */
} list_graph_t;

typedef struct queue_t
{
	unsigned int max_size;
	unsigned int size;
	unsigned int data_size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
} queue_t;

linked_list_t *ll_create(unsigned int data_size);

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se returneaza
 * ultimul nod.
 */
ll_node_t *get_nth_node(linked_list_t *list, unsigned int n);

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t *list, unsigned int n,
					 const void *new_data);

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t *
ll_remove_nth_node(linked_list_t *list, unsigned int n);

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t *list);

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t **pp_list);

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t *list);

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t *list);

queue_t *q_create(unsigned int data_size, unsigned int max_size);

unsigned int q_get_size(queue_t *q);

unsigned int
q_is_empty(queue_t *q);

void *q_front(queue_t *q);

int q_dequeue(queue_t *q);

int q_enqueue(queue_t *q, void *new_data);

void q_clear(queue_t *q);

void q_free(queue_t *q);

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru lista de adiacenta a grafului.
 */
list_graph_t *lg_create(int nodes);

/* Adauga o muchie intre nodurile primite ca parametri */
void lg_add_edge(list_graph_t *graph, int src, int dest);

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int lg_has_edge(list_graph_t *graph, int src, int dest);

/* Elimina muchia dintre nodurile primite ca parametri */
void lg_remove_edge(list_graph_t *graph, int src, int dest);

/* Elibereaza memoria folosita de lista de adiacenta a grafului */
void lg_free(list_graph_t *graph);

/* Printeaza lista de adiacenta a grafului */
void lg_print_graph(list_graph_t *graph);

void print_min_path(list_graph_t *graph, int src, int dest, int *distance);

#endif /* UTILS_H */
