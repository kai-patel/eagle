#ifndef EGL_BSTREE_H
#define EGL_BSTREE_H

#include <stdbool.h>

typedef struct egl_bstree {
  void *value;
  int height;
  struct egl_bstree *left;
  struct egl_bstree *right;
  struct egl_bstree *(*add)(struct egl_bstree *, void *,
                            int (*)(const void *, const void *));
  struct egl_bstree *(*find)(struct egl_bstree *const, const void *,
                             int (*)(const void *, const void *));
  struct egl_bstree *(*remove)(struct egl_bstree *, void *,
                               int (*)(const void *, const void *));
  struct egl_bstree *(*minimum)(struct egl_bstree *tree);
  struct egl_bstree *(*maximum)(struct egl_bstree *tree);
  void (*free)(struct egl_bstree *);
} egl_bstree;

egl_bstree *egl_bstree_new(void *);

#endif
