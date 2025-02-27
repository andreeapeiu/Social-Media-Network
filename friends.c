#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"
#include "utils.h"

/**
 * @brief Creeaza o legatura intre doi utilizatori
 * @param friends - graful pentru useri
 * @param name_1 - numele primului user
 * @param name_2 - numele celui de-al doilea user
 */
void add_friend(list_graph_t **friends, char *name_1, char *name_2)
{
	uint16_t id_1 = get_user_id(name_1);
	uint16_t id_2 = get_user_id(name_2);

	// adaug o muchie intre cele doua noduri
	lg_add_edge(*friends, id_1, id_2);
	lg_add_edge(*friends, id_2, id_1);

	printf("Added connection %s - %s\n", name_1, name_2);
}

/**
 * @brief Elimina legatura intre doi utilizatori
 * @param friends - graful pentru useri
 * @param name_1 - numele primului user
 * @param name_2 - numele celui de-al doilea user
 */
void remove_friend(list_graph_t **friends, char *name_1, char *name_2)
{
	uint16_t id_1 = get_user_id(name_1);
	uint16_t id_2 = get_user_id(name_2);

	// scot muchia dintre noduri
	lg_remove_edge(*friends, id_1, id_2);
	lg_remove_edge(*friends, id_2, id_1);

	printf("Removed connection %s - %s\n", name_1, name_2);
}

/**
 * @brief Calculeaza distanta intre 2 persoane de pe platforma
 * @param friends - graful pentru useri
 * @param name_1 - numele primului user
 * @param name_2 - numele celui de-al doilea user
 */
void find_distance(list_graph_t *friends, char *name_1, char *name_2)
{
	uint16_t id_1 = get_user_id(name_1);
	uint16_t id_2 = get_user_id(name_2);

	int distance = 0;
	print_min_path(friends, id_1, id_2, &distance);
	if (distance <= 0)
		printf("There is no way to get from %s to %s\n", name_1, name_2);
	else
		printf("The distance between %s - %s is %d\n",
			   name_1, name_2, distance);
}

/**
 * @brief Afiseaza numarul de prieteni al unui user
 * @param friends - graful pentru useri
 * @param name - numele user-ului
 */
void get_nr_friends(list_graph_t *friends, char *name)
{
	uint16_t id = get_user_id(name);
	int nr_friends = friends->neighbors[id]->size;

	printf("%s has %d friends\n", name, nr_friends);
}

/**
 * @brief Gaseste prietenii comuni pentru doi utilizatori
 * @param friends - graful pentru useri
 * @param name_1 - numele primului user
 * @param name_2 - numele celui de-al doilea user
 */
void find_common_friends(list_graph_t *friends, char *name_1, char *name_2)
{
	uint16_t id_1 = get_user_id(name_1);
	uint16_t id_2 = get_user_id(name_2);

	// vector in care pun numele prietenilor gasiti
	char common_friends[550][550];
	int nr_common_friends = 0;

	ll_node_t *it_1 = friends->neighbors[id_1]->head;
	unsigned int i;
	int ok = 0;
	// parcurg prietenii primului user
	for (i = 0; i < friends->neighbors[id_1]->size; i++) {
		ll_node_t *it_2 = friends->neighbors[id_2]->head;
		ok = 0;
		// verific daca cei doi useri au prieteni comuni
		// parcurg prietenii celui de-al doilea user
		for (unsigned int j = 0; j < friends->neighbors[id_2]->size; j++) {
			if (*(int *)it_1->data == *(int *)it_2->data) {
				ok = 1;
				break;
			}
			it_2 = it_2->next;
		}
		// daca am gasit un prieten comun
		if (ok == 1) {
			char *name = get_user_name(*(int *)it_1->data);
			strcpy(common_friends[nr_common_friends], name);
			nr_common_friends++;
		}
		it_1 = it_1->next;
	}

	if (nr_common_friends == 0) {
		printf("No common friends for %s and %s\n", name_1, name_2);
	} else {
		printf("The common friends between %s and %s are:\n", name_1, name_2);
		for (int i = 0; i < nr_common_friends; i++)
			printf("%s\n", common_friends[i]);
	}
}

/**
 * @brief Afiseaza utilizatorul cu cele mai multe conexiuni
 * dintre el si prietenii lui.
 * @param friends - graful pentru useri
 * @param name - numele user-ului
 */
void get_popular(list_graph_t *friends, char *name)
{
	uint16_t id = get_user_id(name);
	if (!friends->neighbors[id] || friends->neighbors[id]->size == 0) {
		printf("No friends data available for %s.\n", name);
		return;
	}

	int max_friends = -1;
	uint16_t max_id = id;

	ll_node_t *it = friends->neighbors[id]->head;
	for (unsigned int i = 0; i < friends->neighbors[id]->size; i++) {
		uint16_t friend_id = *(uint16_t *)(it->data);
		int nr = friends->neighbors[friend_id]->size;
		if (nr > max_friends) {
			max_friends = nr;
			max_id = friend_id;
		} else if (nr == max_friends) {
			if (friend_id < max_id && max_id != id)
				max_id = friend_id;
		}
		it = it->next;
	}

	if ((unsigned int)max_friends <= friends->neighbors[id]->size)
		max_id = id;

	if (max_id == id)
		printf("%s is the most popular\n", name);
	else
		printf("%s is the most popular friend of %s\n",
			   get_user_name(max_id), name);
}

/**
 * @brief Gaseste si afiseaza toti prietenii prietenilor
 * @param friends - graful pentru useri
 * @param name - numele user-ului
 */
void get_suggestions(list_graph_t *friends, char *name)
{
	int frecv[MAX_PEOPLE] = {0};
	int id = get_user_id(name);
	ll_node_t *it = friends->neighbors[id]->head;

	for (unsigned int i = 0; i < friends->neighbors[id]->size; i++) {
		uint16_t friend_id = *(uint16_t *)(it->data);
		ll_node_t *it2 = friends->neighbors[friend_id]->head;

		// Parcurge prietenii prietenilor
		for (unsigned int j = 0; j < friends->neighbors[friend_id]->size; j++) {
			uint16_t friend_of_friend_id = *(uint16_t *)(it2->data);
			if (friend_of_friend_id != id &&
				!lg_has_edge(friends, id, friend_of_friend_id))
				frecv[friend_of_friend_id]++;
			it2 = it2->next;
		}
		it = it->next;
	}

	it = friends->neighbors[id]->head;
	for (unsigned int i = 0; i < friends->neighbors[id]->size; i++) {
		uint16_t friend_id = *(uint16_t *)(it->data);
		frecv[friend_id] = 0;
		it = it->next;
	}

	int has_suggestions = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
		if (frecv[i] > 0)
			has_suggestions = 1;

	if (has_suggestions == 1) {
		printf("Suggestions for %s:\n", name);
		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (frecv[i] > 0)
				printf("%s\n", get_user_name(i));
		}
	} else {
		printf("There are no suggestions for %s\n", name);
	}
}

void handle_input_friends(char *input, list_graph_t **friends)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name_1 = strtok(NULL, "\n ");
		char *name_2 = strtok(NULL, "\n ");
		add_friend(friends, name_1, name_2);
	} else if (!strcmp(cmd, "remove")) {
		char *name_1 = strtok(NULL, "\n ");
		char *name_2 = strtok(NULL, "\n ");
		remove_friend(friends, name_1, name_2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n ");
		get_suggestions(*friends, name);
	} else if (!strcmp(cmd, "distance")) {
		char *name_1 = strtok(NULL, "\n ");
		char *name_2 = strtok(NULL, "\n ");
		find_distance(*friends, name_1, name_2);
	} else if (!strcmp(cmd, "common")) {
		char *name_1 = strtok(NULL, "\n ");
		char *name_2 = strtok(NULL, "\n ");
		find_common_friends(*friends, name_1, name_2);
	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n ");
		get_nr_friends(*friends, name);
	} else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n ");
		get_popular(*friends, name);
	}

	free(commands);
}
