#ifndef EGL_GRAPH_H
#define EGL_GRAPH_H

#include "egl_llist.h"
#include "egl_matrix.h"
#include "egl_vector.h"

enum EGL_GRAPH_TYPE { ADJ_LIST, ADJ_MATRIX };

typedef struct egl_graph {
  enum EGL_GRAPH_TYPE type;
  bool is_weighted;
  union {
    egl_vector *adjacency_list;
    egl_matrix *adjacency_matrix;
  } representation;
  bool (*adjacent)(struct egl_graph *, void *, void *);
  bool (*has_cycles)(struct egl_graph *);
  void *(*add_vertex)(struct egl_graph *, void *);
  void *(*remove_vertex)(struct egl_graph *, void *);
  void *(*add_edge)(struct egl_graph *, void *, void *, void *);
  void *(*remove_edge)(struct egl_graph *, void *, void *);
  void *(*get_vertex)(struct egl_graph *, void *);
  void *(*set_vertex)(struct egl_graph *, void *, void *);
  void **(*components)(struct egl_graph *);
  void **(*neighbours)(struct egl_graph *, void *);
  void **(*top_sort)(struct egl_graph *);
  void **(*scc)(struct egl_graph *);
  struct egl_graph *(*transpose)(struct egl_graph *);
  bool (*path)(struct egl_graph *, void *, void *);
} egl_graph;

#endif
