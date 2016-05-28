/* list. c - Simple list

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

#include <debug.h>
#include <list.h>

/* Allocate and initialize a list. Parameter del is a pointer
   to a function to be used to release the memory used by
   a node's value.*/

list_t *new_list (void (*del)(void*))
{
  list_t *list;
  list = malloc (sizeof (list_t));
  sysfail (!list, NULL);
  list->size = 0;
  list->first = NULL;
  list->last = NULL;
  list->del = del; 
  return list;
}

/* Free memory used by a list. */

void release_list (list_t *list)
{
  list_node_t *p;

  fatal (!list->del, "Don't know how to free nodes.");

  /* Free all nodes. */
  p = list->first;
  while (p)
    {
      del_node (list, p);
      p = p->next;
    }

  free (list);
  
}

/* Append a node at the end of the list. */

list_node_t * append_node (list_t *list)
{
  list_node_t * node;
  
  node = malloc (sizeof (list_node_t));
  sysfail (!node, NULL);
  node->value = NULL;

  if (list->first == NULL)
    {
      list->first = node;
      node->previous = NULL;
    }
  else
    {
      list->last->next = node;
      node->previous = list->last;
      }
  
  list->last = node;
  node->next = NULL;

  list->size++;

  return node;
}

/* Remove a node from the list. */

int del_node (list_t *list, list_node_t *node)
{
  list_node_t *p;
  /* Seek the node. */
  p = list->first;
  while ( (p) && (p!=node))
    p = p->next;
  
  /* No such node. */
  if (!p)
    return EXIT_FAILURE;

  /* Ok node found. Let's remove it. */
  
  if (node == list->first)
    list->first = node->next;
  else
    node->previous->next = node->next;

  if (node == list->last)
    list->last = node->previous;
  else
    node->next->previous = node->previous;

  list->del (node->value);
  free (node);

  list->size--;

  return EXIT_SUCCESS;
}


