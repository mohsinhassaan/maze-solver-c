#include <stdlib.h>

typedef enum { PATH, WALL } VertexType;
static const char *VERTEX_TYPES[2] = {"PATH", "WALL"};

typedef struct Vertex {
  int x;
  int y;
  VertexType type;
} Vertex;

typedef struct EdgeList {
  size_t source;
  size_t *targets;
  size_t edge_count;
  size_t max_edges;
} EdgeList;

typedef struct Graph {
  Vertex *vertices;
  EdgeList *adjacencyList;
  size_t vertex_count;
  size_t max_vertices;
} Graph;

typedef struct Path {
  size_t size;
  size_t *path;
} Path;

Graph *create_graph();
int delete_graph(Graph *graph);
size_t create_vertex(Graph *graph);
int add_undirected_edge(Graph *graph, size_t source, size_t dest);
size_t *get_edges_from(const Graph *graph, size_t source);

Path *shortest_path(const Graph *graph, size_t source, size_t dest);

char *graph_string(const Graph *g);
char *vertex_string(const Graph *g, size_t v_index);
char *edge_string(size_t source, size_t dest);
