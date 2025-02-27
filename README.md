### Copyright Oprisan Alexia-Ioana and Peiu Andreea - 314CA

# SD Homework 3

## TASK 1

### add friend

This function's role is to create bidirectional connections between the graph's
nodes to highlight the friendship. The adjacency list `friends` stores the
connections established between friends. This list is traversed, and a
connection is created between two users by adding a node to the list
corresponding to the start node.

### remove friend

The function receives two users as parameters and removes the friendship
connection between them. Thus, the adjacency lists associated with both IDs are
traversed, and the connection between the two is removed.

### distance

The role of this function is to check if there is a path between the two users
and to determine the shortest way to traverse the graph from one node to the
other, using an algorithm for finding the minimum path between two nodes.

### suggestions

Friend suggestions are generated for a given user based on the friendship
network in the graph. The function finds and displays all the friends of the
friends who are not already friends with the given user.

### common

The function displays, in ascending order by id, all the mutual friends of two
users. The adjacency list is traversed, and the mutual friends are identified
to be displayed. The lists associated with the two users are already sorted by
id when the friendship is created.

### friends

This function's role is to count how many friends a certain user has. The size
of the list associated with that user is displayed on the screen.

### popular

The adjacency list of a user is traversed, and it checks for each of their
friends how many connections they have. After determining the number of
connections for each friend, the person with the most connections is displayed.

## TASK 2

### create post

This function creates a post by reallocating memory for the array of posts in
which they are stored. A post is of type `struct post_t`, retaining several
pieces of information about it, such as `titlu`, `user_id` (the user who
created the post), `likes` (the number of likes the post accumulates). In
addition, memory is allocated for a generic tree needed to store reposts.

### repost

The function receives as parameters a name, a `post_id`, and optionally a
`repost_id`. The post with the id `post_id` is searched for in the array of
structures using the function `find_post_by_id`. If only the first two
parameters are received, then the original post is reposted, and a node is
added to the root's children array. Otherwise, a search is performed through
the `events` tree to find the node with the id `repost_id`, and a node is added
to the children array of that node. The search is done using the implemented
function `find_node_by_id`.

### common-repost

The closest ancestor of the two nodes given as parameters is searched for
recursively. The algorithm is similar to the `lowest common-ancestor` one but
is adapted for the generic tree `events`.

### like

The function receives a name, a `post_id`, and optionally a `repost_id`. In the
structure of each node in the tree, there is a frequency array of size
`MAX_PEOPLE` to store whether the user has liked the post or not. This is
necessary to decrement the number of likes if the user has already liked the
post.

### ratio

This function determines the most appreciated post and checks if a repost has
more likes than the original post, displaying an appropriate message.

### delete

The function deletes a repost or a post and all its descendant nodes. If a post
is deleted, the array is shifted one position to the left so that the remaining
posts occupy consecutive positions.

### get-likes

Displays the number of likes a post or a repost has.

### get-reposts

Displays all the reposts of the node given as a parameter. The tree is
traversed recursively, from left to right, for each node. If the function is
called for the original post, its title is also displayed. Additionally, for
each repost, the person who made the repost is displayed.

## TASK 3

### feed

The traversal is done from the end of the array toward the beginning to ensure
that the displayed posts are the most recent. As long as `feed_size` is not
zero, it checks if the user is a friend or if the post belongs to the searched
user. If this condition is met, the name of the user is displayed along with
the title of the post made by them.

### view-profile

This function traverses the array of posts and checks if they belong to the
desired user and displays the title of those that meet this condition. Then,
all the trees built in the search for reposts made by that user are traversed.

### friends-repost

The function traverses the tree associated with the post whose id was received
from the keyboard and recursively checks for each repost if the person who made
the repost is a friend of the user provided as a parameter.

### common-group

The function searches in the adjacency list of the user provided as a parameter
for the largest complete subgraph that is formed. The algorithm used for this
search is called `Bron Kerbosch`.
