#include "../include/image.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

size_t add_path(Graph *g, int x, int y);
void test_graph();

int main(int argc, char *argv[]) {
  Graph *g;
  size_t start, end;
  char *in_img = argc > 1 ? argv[1] : "assets/tinier_maze.bmp";
  char *out_img = argc > 2 ? argv[2] : "solved.bmp";

  clock_t start_t, diff_t;
  int msec;

  start_t = clock();

  g = load_image(in_img, &start, &end);

  diff_t = clock() - start_t;
  msec = diff_t * 1000 / CLOCKS_PER_SEC;
  printf("Graph loaded in %dms\n", msec);

  /* char *g_str = graph_string(g); */
  /* printf("%s\n", g_str); */
  /* free(g_str); */

  start_t = clock();

  Path *path = shortest_path(g, start, end);

  diff_t = clock() - start_t;
  msec = diff_t * 1000 / CLOCKS_PER_SEC;
  printf("Shortest path calculated in %dms\n", msec);

  if (path != NULL && path->size > 0) {
    /* size_t i; */
    /* for (i = path->size - 1; i > 0; --i) { */
    /*   printf("%zu -> ", path->path[i]); */
    /* } */
    /* printf("%zu\n", path->path[0]); */

    start_t = clock();

    write_image(in_img, out_img, path, g);

    diff_t = clock() - start_t;
    msec = diff_t * 1000 / CLOCKS_PER_SEC;
    printf("Solution written in %dms\n", msec);

    printf("Length of path: %zu\n", path->size);

    free(path->path);
    free(path);
  }

  delete_graph(g);
}

void test_graph() {
  Graph *g = create_graph();

  add_path(g, 9, 0); //  0

  add_path(g, 3, 1); //  1
  add_path(g, 5, 1); //  2
  add_path(g, 9, 1); //  3

  add_path(g, 1, 3); //  4
  add_path(g, 3, 3); //  5
  add_path(g, 7, 3); //  6
  add_path(g, 9, 3); //  7

  add_path(g, 7, 5); //  8
  add_path(g, 9, 5); //  9

  add_path(g, 3, 7); //  10

  add_path(g, 1, 9); //  11
  add_path(g, 9, 9); //  12

  add_path(g, 1, 10); // 13

  add_undirected_edge(g, 0, 3);

  add_undirected_edge(g, 1, 5);

  add_undirected_edge(g, 2, 3);

  add_undirected_edge(g, 3, 7);

  add_undirected_edge(g, 4, 5);
  add_undirected_edge(g, 4, 11);

  add_undirected_edge(g, 5, 10);

  add_undirected_edge(g, 6, 7);
  add_undirected_edge(g, 6, 8);

  add_undirected_edge(g, 8, 9);

  add_undirected_edge(g, 9, 12);

  add_undirected_edge(g, 11, 12);

  add_undirected_edge(g, 11, 13);

  char *g_str = graph_string(g);
  printf("%s\n", g_str);
  free(g_str);

  Path *path = shortest_path(g, 0, 13);

  if (path->size > 0) {
    size_t i;
    for (i = path->size - 1; i > 0; --i) {
      printf("%zu -> ", path->path[i]);
    }
    printf("%zu\n", path->path[0]);
  }

  free(path->path);
  free(path);

  delete_graph(g);
}

size_t add_path(Graph *g, int x, int y) {
  size_t index = create_vertex(g);

  Vertex *v = g->vertices + index;
  v->x = x;
  v->y = y;
  v->type = PATH;

  return index;
}
