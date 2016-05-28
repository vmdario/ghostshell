/* tparse.h - A trivial command line parser example.

   Copyright (c) 2015, Monaco F. J. <moanco@icmc.usp.br>

   This file is part of POSIXeg.

   POSIXeg is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIST_H
#define LIST_H

/* Struct representing a node in double-linked list. 
   Nodes have a (void*) value which you can point to whatever kind
   of data you want. */

typedef struct list_node_t list_node_t;
struct list_node_t
{
  list_node_t *next;		/* Link to the next node. */
  list_node_t *previous;	/* Link to the previous node. */
  void *value;			/* Value associated to the node. */
};

/* Struct representing a double-linked list. */

typedef struct list_t
{
  int size;			/* Number of nodes in the list. */
  list_node_t *first;		/* First node; NULL if empty. */
  list_node_t *last;		/* Last node; NULL if empty. */
  void (*del)(void *);		/* Function used to free a node->value. */
} list_t;

/* Return a pointer to a newly allocated and properly initialized double-linked
   list. You shuld pass a pointer to the function to be used to free the node
   value. For example, if your nodes point to a (char*) block, you can pass
   a pointer to a free(char *) .*/

list_t * new_list (void (*)(void*));

/* Release the memory used by a list (including all the nodes). */

void release_list (list_t *);

/* Append a node to the end of the list. */

list_node_t * append_node (list_t *);

/* Remove a node from the list (and free the node). */

int del_node (list_t *, list_node_t *);

/* Like strdup(), which is not ISO C. */


#endif	/* LIST_H */
