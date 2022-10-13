#ifndef EGL_LLIST_H
#define EGL_LLIST_H

typedef struct egl_llist_node {
  void *value;
  struct egl_llist_node *next;
} egl_llist_node;

typedef struct egl_llist {
  egl_llist_node *head;
  egl_llist_node *(*find)(struct egl_llist *, void *,
                          int (*)(const void *, const void *));
  void (*reverse)(struct egl_llist *);
  egl_llist_node *(*add)(struct egl_llist *, void *);
  void (*free)(struct egl_llist *);
  void (*delete_p)(struct egl_llist *, void *); // Delete-by-pointer
  void (*delete_v)(struct egl_llist *, void *,
                   int (*)(const void *, const void *)); // Delete-by-value
} egl_llist;

egl_llist *egl_llist_new(void);

#endif
