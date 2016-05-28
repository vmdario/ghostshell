/* example.c - Example of list functions. 

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


#include <stdlib.h>
#include <stdio.h>

#include <list.h>		/* List API is here. */
#include <tparse.h>

/* Function used to free a node's value. */

void delstring (void *str)
{
  free (str);
}


int main (int argc, char **argv)
{
  list_t *list;
  list_node_t *p, *p1, *p2, *p3, *p4;
  
  /* Get a new empty list. */

  list = new_list (delstring);

  /* Append some nodes. */

  p1 = append_node (list);
  p1->value = stringdup ("Node 1");

  p2 = append_node (list);
  p2->value = stringdup ("Node 2");

  p3 = append_node (list);
  p3->value = stringdup ("Node 3");

  /* Show list. */

  p = list->first;
  while (p)
    {
      printf ("> %s\n", (char *) p->value);
      p = p->next;
    }
  printf ("List size is %d\n", list->size);

  /* Delete a given node (p2). */

  printf ("Del\n");
  del_node (list, p2);

  /* Show the list again. */

  p = list->first;
  while (p)
    {
      printf ("> %s\n", (char *) p->value);
      p = p->next;
    }
  printf ("List size is %d\n", list->size);

  printf ("Add\n");
  del_node (list, p2);
  
  /* Append another node. */

  p4 = append_node (list);
  p4->value = stringdup ("Node 4");

  /* Show the list once more. */

  p = list->first;
  while (p)
    {
      printf ("> %s\n", (char *) p->value);
      p = p->next;
    }
  printf ("List size is %d\n", list->size);
  release_list (list);

  return EXIT_SUCCESS;

}
