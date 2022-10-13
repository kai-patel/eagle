#include <stdlib.h>

#include "egl_vector.h"

/*
 * Resize an egl_vector to a specified capacity
 * If new capacity < old capacity, return NULL
 * Returns NULL if a problem occurs
 */

static egl_vector *egl_vector_resize(struct egl_vector *vec,
                                     uint32_t capacity) {
  if (capacity < vec->capacity)
    return NULL;

  void **items = realloc(vec->items, sizeof(*vec->items) * capacity);

  if (items != NULL) {
    vec->items = items;
    vec->capacity = capacity;
    return vec; // No need to free after successful realloc
  }

  return NULL;
}

/*
 * Add an element to an egl_vector
 * If the vector does not have capacity, resize
 * On success, return the element
 * Returns NULL if a problem occurs
 */

static void *egl_vector_add(struct egl_vector *vec, void *i) {
  if (vec->length == vec->capacity) {
    if (egl_vector_resize(vec, vec->capacity * 2) == NULL)
      return NULL;
  }
  vec->items[vec->length++] = i;
  return i;
}

/*
 * Free both the egl_vector itself and the underlying array
 */

static void egl_vector_free(struct egl_vector *vec) {
  if (vec == NULL)
    return;
  if (vec->items != NULL) {
    for (size_t i = 0; i < vec->length; i++) {
      free(vec->items[i]);
    }
    free(vec->items);
  }
  free(vec);
}

/*
 * Initialise a newly created egl_vector
 * Set initial capacity to EGL_INTERNAL_VECTOR_INITIAL_CAPACITY
 * Set length to 0
 * Set initial elements to 0
 */

static void egl_vector_init(egl_vector *vec) {
  if (vec == NULL)
    return;
  vec->capacity = EGL_INTERNAL_VECTOR_INITIAL_CAPACITY;
  vec->length = 0;
  vec->items = calloc(vec->capacity, sizeof(*vec->items));
  vec->add = &egl_vector_add;
  vec->free = &egl_vector_free;
  vec->resize = &egl_vector_resize;
}

/*
 * Create a new egl_vector.
 * Returns NULL and frees any allocated memory if a problem occurred
 */

egl_vector *egl_vector_new(void) {
  egl_vector *vec = malloc(sizeof(*vec));
  egl_vector_init(vec);
  if (vec->items == NULL) {
    free(vec);
    return NULL;
  } else {
    return vec ? vec : NULL;
  }
}
