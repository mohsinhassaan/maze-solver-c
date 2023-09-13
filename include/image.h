#include "graph.h"

Graph *load_image(const char *image_file, size_t *start, size_t *end);
int write_image(const char *in_image_file, const char *out_image_file,
                const Path *p, const Graph *g);
