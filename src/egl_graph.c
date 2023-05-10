#include "egl_graph.h"
#include <stdlib.h>

static bool egl_graph_adjacent(struct egl_graph *graph, void *v1, void *v2) {
  return NULL;
}

static bool egl_graph_has_cycles(struct egl_graph *graph) { return NULL; }

static void *egl_graph_add_vertex(struct egl_graph *graph, void *v) {
  return NULL;
}

static void *egl_graph_remove_vertex(struct egl_graph *graph, void *v) {
  return NULL;
}

static void *egl_graph_add_edge(struct egl_graph *graph, void *v1, void *v2,
                                void *w) {
  return NULL;
}

static void *egl_graph_remove_edge(struct egl_graph *graph, void *v1,
                                   void *v2) {
  return NULL;
}

static void *egl_graph_get_vertex(struct egl_graph *graph, void *value) {
  return NULL;
}

static void *egl_graph_set_vertex(struct egl_graph *graph, void *v,
                                  void *value) {
  return NULL;
}

static void **egl_graph_components(struct egl_graph *graph) { return NULL; }

static void **egl_graph_neighbours(struct egl_graph *graph, void *v) {
  return NULL;
}

static void **egl_graph_top_sort(struct egl_graph *graph) { return NULL; }

static void **egl_graph_scc(struct egl_graph *graph) { return NULL; }

static struct egl_graph *egl_graph_transpose(struct egl_graph *graph) {
  return NULL;
}

static bool egl_graph_path(struct egl_graph *graph, void *v1, void *v2) {
  return NULL;
}

static void egl_graph_init(egl_graph *graph, enum EGL_GRAPH_TYPE type,
                           bool is_weighted) {
  if (graph == NULL) {
    return;
  }

  graph->type = type;
  graph->is_weighted = is_weighted;
  switch (type) {
  case ADJ_LIST:
    graph->representation.adjacency_list = egl_vector_new();
    if (graph->representation.adjacency_list == NULL) {
      graph = NULL;
      return;
    }
    break;
  case ADJ_MATRIX:
    // TODO: Update egl_matrix to be dynamic (variable dimensions)
    graph->representation.adjacency_matrix = egl_matrix_new(1, 1);
    if (graph->representation.adjacency_matrix == NULL) {
      graph = NULL;
      return;
    }
    break;
  }

  graph->adjacent = egl_graph_adjacent;
  graph->has_cycles = egl_graph_has_cycles;
  graph->add_vertex = egl_graph_add_vertex;
  graph->remove_vertex = egl_graph_remove_vertex;
  graph->add_edge = egl_graph_add_edge;
  graph->remove_edge = egl_graph_remove_edge;
  graph->get_vertex = egl_graph_get_vertex;
  graph->set_vertex = egl_graph_set_vertex;
  graph->components = egl_graph_components;
  graph->neighbours = egl_graph_neighbours;
  graph->top_sort = egl_graph_top_sort;
  graph->scc = egl_graph_scc;
  graph->transpose = egl_graph_transpose;
  graph->path = egl_graph_path;
}

egl_graph *egl_graph_new(enum EGL_GRAPH_TYPE type, bool is_weighted) {
  egl_graph *graph = malloc(sizeof(*graph));

  if (graph == NULL) {
    return NULL;
  }

  egl_graph_init(graph, type, is_weighted);

  return graph;
}
