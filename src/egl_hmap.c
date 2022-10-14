#include "egl_hmap.h"
#include <stdlib.h>

/*
 * Free all keys, values, and underlying data structures for a given egl_hmap
 */

void egl_hmap_free(struct egl_hmap *map) { return; }

/*
 * Add a key and its value to a given egl_hmap
 * Returns the added key
 * Returns NULL if an error occurred
 */

void *egl_hmap_add(struct egl_hmap *map, void *key, void *value) {
  return NULL;
}

/*
 * Remove a key and its value from a given egl_hmap
 * Returns the removed key
 * Returns NULL if an error occurred
 */

void *egl_hmap_remove(struct egl_hmap *map, void *key) { return NULL; }

/*
 * Test whether a given value exists in an egl_hmap
 */

bool egl_hmap_contains_value(struct egl_hmap *map, void *value) {
  return false;
}

/*
 * Test whether a given key exists in an egl_hmap
 */

bool egl_hmap_contains_key(struct egl_hmap *map, void *key) { return false; }

/*
 * Get a value from an egl_hmap, given a key
 */

void *egl_hmap_get(struct egl_hmap *map, void *key) { return NULL; }

/*
 * Get a pointer to the first element in an array of values of the egl_hmap
 * Returns NULL if an error occurred
 */

void **egl_hmap_to_array(struct egl_hmap *map) { return NULL; }

/*
 * Initialise a newly created egl_hmap
 */

egl_hmap *egl_hmap_init(struct egl_hmap *map) { return NULL; }

/*
 * Creates a new, empty egl_hmap
 * Returns NULL if an error occurred
 */

egl_hmap *egl_hmap_new(void) { return NULL; }
