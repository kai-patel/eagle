#ifndef EGL_HMAP_H
#define EGL_HMAP_H

#include <stdbool.h>

typedef struct egl_hmap {
  size_t size;
  void *(*add)(struct egl_hmap *, void *);
  void *(*remove)(struct egl_hmap *, void *);
  bool (*contains_key)(struct egl_hmap *, void *);
  bool (*contains_value)(struct egl_hmap *, void *);
  void *(*get)(struct egl_hmap *, void *);
  void **(*to_array)(struct egl_hmap *);
  void (*free)(struct egl_hmap *);
} egl_hmap;

egl_hmap *egl_hmap_new(void);

#endif
