#include "egl_llist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Create a new egl_llist_node with a given value
 * Returns NULL if a problem occurred
 */

static egl_llist_node *egl_llist_node_new(void *i) {
  egl_llist_node *node = malloc(sizeof(*node));
  if (node == NULL) {
    return NULL;
  }
  node->value = i;
  node->next = NULL;
  return node;
}

/*
 * Find a node with a given value in a given egl_llist
 */

static egl_llist_node *egl_llist_find(struct egl_llist *llist, void *i,
                                      int (*compare)(const void *,
                                                     const void *)) {
  egl_llist_node *curr = llist->head;

  while (curr) {
    if (compare(i, curr->value) == 0) {
      return curr;
    }
    curr = curr->next;
  }

  return NULL;
}

/*
 * Reverse a given egl_llist in place
 */

static void egl_llist_reverse(struct egl_llist *llist) {
  egl_llist_node *prev = NULL;
  egl_llist_node *curr = llist->head;
  egl_llist_node *next = NULL;

  while (curr != NULL) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }

  llist->head = prev;
}

/*
 * Delete an egl_llist_node from an egl_llist
 * Deletion compares values as pointers
 */

static void egl_llist_delete_p(struct egl_llist *llist, void *i) {
  egl_llist_node *curr = llist->head;
  egl_llist_node *prev = NULL;

  while (curr != NULL) {
    if (curr->value == i) {
      if (prev != NULL) {
        prev->next = curr->next;
      } else {
        llist->head = NULL;
      }
      free(curr->value);
      free(curr);
      return;
    }

    prev = curr;
    curr = curr->next;
  }
}

/*
 * Delete an egl_llist_node from an egl_llist
 * Deletion compares values given a comparison function
 */

static void egl_llist_delete_v(struct egl_llist *llist, void *i,
                               int (*compare)(const void *a, const void *b)) {
  egl_llist_node *curr = llist->head;
  egl_llist_node *prev = NULL;

  while (curr != NULL) {
    if (compare(i, curr->value) == 0) {
      if (prev != NULL) {
        prev->next = curr->next;
      } else {
        llist->head = NULL;
      }
      free(curr->value);
      free(curr);
      return;
    }

    prev = curr;
    curr = curr->next;
  }
}

/*
 * Add an egl_llist_node with a given value to an egl_llist
 * Returns NULL if a problem occurred
 */

static egl_llist_node *egl_llist_add(struct egl_llist *llist, void *i) {
  if (llist->head == NULL) {
    llist->head = egl_llist_node_new(i);
    return llist->head;
  }

  egl_llist_node *curr = llist->head;
  while (curr != NULL && curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = egl_llist_node_new(i);
  return curr->next;
}

/*
 * Free every egl_llist_node, it's value and the egl_llist itself
 */

static void egl_llist_free(struct egl_llist *llist) {
  if (llist == NULL) {
    return;
  }

  egl_llist_node *tmp;
  egl_llist_node *head = llist->head;

  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp->value);
    free(tmp);
  }

  free(llist);
}

/*
 * Initialise a new egl_llist
 * Set the head to NULL
 */

void egl_llist_init(egl_llist *llist) {
  if (llist == NULL)
    return;
  llist->head = NULL;
  llist->find = &egl_llist_find;
  llist->reverse = &egl_llist_reverse;
  llist->add = &egl_llist_add;
  llist->free = &egl_llist_free;
  llist->delete_p = &egl_llist_delete_p;
  llist->delete_v = &egl_llist_delete_v;
}

/*
 * Create a new egl_llist
 * Returns NULL if a problem occurred
 */

egl_llist *egl_llist_new(void) {
  egl_llist *llist = malloc(sizeof(*llist));
  egl_llist_init(llist);
  if (llist == NULL) {
    return NULL;
  } else {
    return llist;
  }
}
