#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "utils.h"
#include "utils2.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void) {
	#ifdef TASK_1
	// Initialize task 1
	#endif

	#ifdef TASK_2
	// Initialize task 2
	#endif

	#ifdef TASK_3
	// Initialize task 3
	#endif
}

post_t **create_array(post_t ***posts_array, int *post_id,
					  int *nr_posts, int capacity) {
	*posts_array = calloc(capacity, sizeof(post_t *));
	DIE(!*posts_array, "calloc failed");
	*post_id = 1;
	*nr_posts = 0;
	return *posts_array;
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void) {
	init_users();

	init_tasks();
	list_graph_t *friends = lg_create(MAX_PEOPLE);
	int post_id, nr_posts;
	post_t **posts_array;
	int capacity = 1;

	posts_array = create_array(&posts_array, &post_id, &nr_posts, capacity);
	char *input = (char *)malloc(MAX_COMMAND_LEN);

	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);
		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, &friends);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &posts_array, &post_id, &nr_posts, &capacity);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, &friends, &posts_array, &nr_posts);
		#endif
	}

	free_users();
	free(input);
	lg_free(friends);

	free_array_of_posts(posts_array, nr_posts);

	return 0;
}
