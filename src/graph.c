#include "../include/graph.h"
#include "../include/common.h"
#include "../include/queue.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph *create_graph() {
  Graph *graph = malloc(sizeof(Graph));
  graph->vertex_count = 0;
  graph->max_vertices = ALLOC_BLOCK;
  graph->vertices = malloc(ALLOC_BLOCK * sizeof(Vertex));
  graph->adjacencyList = malloc(ALLOC_BLOCK * sizeof(EdgeList));

  return graph;
}

int delete_graph(Graph *graph) {
  for (size_t i = 0; i < graph->vertex_count; ++i) {
    EdgeList *adjList = graph->adjacencyList + i;
    free(adjList->targets);
  }

  free(graph->adjacencyList);
  free(graph->vertices);
  free(graph);

  return EXIT_SUCCESS;
};

size_t create_vertex(Graph *g) {
  while (g->vertex_count >= g->max_vertices) {
    g->max_vertices += ALLOC_BLOCK;
    g->vertices = realloc(g->vertices, g->max_vertices * sizeof(Vertex));
    g->adjacencyList =
        realloc(g->adjacencyList, g->max_vertices * sizeof(EdgeList));
  }

  size_t index = g->vertex_count++;

  EdgeList *edgeList = g->adjacencyList + index;
  edgeList->source = index;
  edgeList->edge_count = 0;
  edgeList->targets = malloc(ALLOC_BLOCK * sizeof(Vertex *));
  edgeList->max_edges = ALLOC_BLOCK;

  return index;
};

int add_undirected_edge(Graph *g, size_t source, size_t dest) {
  assert(source < g->vertex_count && dest < g->vertex_count);

  EdgeList *sourceEdgeList = g->adjacencyList + source;

  while (sourceEdgeList->edge_count >= sourceEdgeList->max_edges) {
    sourceEdgeList->targets =
        realloc(sourceEdgeList->targets, ALLOC_BLOCK * sizeof(Vertex));
    sourceEdgeList->max_edges += ALLOC_BLOCK;
  }

  EdgeList *destEdgeList = g->adjacencyList + dest;

  while (destEdgeList->edge_count >= destEdgeList->max_edges) {
    destEdgeList->targets =
        realloc(destEdgeList->targets, ALLOC_BLOCK * sizeof(Vertex));
    destEdgeList->max_edges += ALLOC_BLOCK;
  }

  sourceEdgeList->targets[sourceEdgeList->edge_count++] = dest;
  destEdgeList->targets[destEdgeList->edge_count++] = source;

  return EXIT_SUCCESS;
}

size_t *get_edges_from(const Graph *g, size_t source) {
  assert(source < g->vertex_count);
  return g->adjacencyList[source].targets;
};

Path *shortest_path(const Graph *g, size_t source, size_t dest) {
  assert(source < g->vertex_count && dest < g->vertex_count);

  bool *explored = calloc(g->vertex_count, sizeof(bool));
  long *prev = malloc(g->vertex_count * sizeof(long));
  memset(prev, -1, g->vertex_count * sizeof(long));
  Queue *q = createQueue();
  size_t u, v;
  bool found = false;

  explored[source] = true;
  push(q, source);

  while (!found && q->size > 0) {
    u = pop(q);
    EdgeList *edgeList = g->adjacencyList + u;
    for (size_t i = 0; i < edgeList->edge_count; ++i) {
      v = edgeList->targets[i];
      if (!explored[v]) {
        explored[v] = true;
        prev[v] = u;
        push(q, v);

        if (v == dest) {
          found = true;
          break;
        }
      }
    }
  }

  if (found) {
    size_t *path_vertices = malloc(ALLOC_BLOCK * sizeof(size_t));
    size_t path_size = 0;
    size_t allocated_size = 32;
    size_t crawl = dest;

    path_vertices[path_size++] = crawl;

    while (prev[crawl] != -1) {
      while (path_size >= allocated_size) {
        path_vertices = realloc(path_vertices, ALLOC_BLOCK * sizeof(size_t));
        allocated_size += ALLOC_BLOCK;
      }
      crawl = prev[crawl];
      path_vertices[path_size++] = crawl;
    }

    Path *path = malloc(sizeof(Path));
    path->size = path_size;
    path->path = path_vertices;

    return path;
  } else {
    return NULL;
  }
}

char *graph_string(const Graph *g) {
  unsigned long buf_size = g->vertex_count * STRING_BASE_SIZE * sizeof(char);

  char *str = malloc(buf_size);
  char temp_str[STRING_BASE_SIZE];

  snprintf(str, buf_size,
           "Graph {\n"
           "  vertex_count: %zu,\n"
           "  vertices: [\n",
           g->vertex_count);

  char *v_str, *curr, *next;

  for (size_t i = 0; i < g->vertex_count; ++i) {
    v_str = vertex_string(g, i);
    curr = v_str;

    while ((next = strchr(curr, '\n')) != NULL) {
      *next = '\0';
      snprintf(temp_str, sizeof(temp_str), "    %s\n", curr);
      strlcat(str, temp_str, buf_size);
      curr = next + 1;
    }

    snprintf(temp_str, sizeof(temp_str), "    %s,\n", curr);
    strlcat(str, temp_str, buf_size);

    free(v_str);
  }

  strlcat(str,
          "  ],\n"
          "}",
          buf_size);

  unsigned long len = (strlen(str) + 1);
  char *ret_str = malloc(len * sizeof(char));
  memcpy(ret_str, str, len);

  free(str);

  return ret_str;
}

char *vertex_string(const Graph *g, size_t v_index) {
  assert(v_index < g->vertex_count);

  Vertex *v = g->vertices + v_index;
  EdgeList *el = g->adjacencyList + v_index;

  unsigned long buf_size = STRING_BASE_SIZE * sizeof(char);
  char *str = malloc(buf_size);
  char temp_str[STRING_BASE_SIZE];

  snprintf(str, buf_size,
           "Vertex {\n"
           "  index: %zu,\n"
           "  x: %d,\n"
           "  y: %d,\n"
           "  type: %s\n"
           "  edge_count: %zu,\n"
           "  edges: [\n",
           v_index, v->x, v->y, VERTEX_TYPES[v->type], el->edge_count);

  for (size_t i = 0; i < el->edge_count; ++i) {
    char *estr = edge_string(v_index, el->targets[i]);
    snprintf(temp_str, sizeof(temp_str), "    %s,\n", estr);

    free(estr);

    strlcat(str, temp_str, buf_size);
  }

  strlcat(str,
          "  ],\n"
          "}",
          buf_size);

  unsigned long len = (strlen(str) + 1);
  char *ret_str = malloc(len * sizeof(char));
  memcpy(ret_str, str, len);

  free(str);

  return ret_str;
}

char *edge_string(size_t source, size_t dest) {
  char *ret_str;
  asprintf(&ret_str, "%zu -> %zu", source, dest);

  return ret_str;
}
