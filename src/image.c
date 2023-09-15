#include "../include/image.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_path(unsigned char pixel);

Graph *load_image(const char *image_file, size_t *start, size_t *end) {
  int width, height, channels;
  unsigned char *imageData =
      stbi_load(image_file, &width, &height, &channels, 1);
  if (!imageData) {
    fprintf(stderr, "Error loading BMP file: %s\n", stbi_failure_reason());
    return NULL;
  }

  Graph *g = create_graph();

  int num_pixels = height * width;
  int i = 0;
  unsigned char pix;

  size_t *verts = malloc(num_pixels * sizeof(size_t));
  memset(verts, -1, num_pixels * sizeof(size_t));

  for (int x = 0; x < width; ++x) {
    pix = imageData[i];
    if (is_path(pix)) {
      *start = create_vertex(g);
      g->vertices[*start].x = x;
      g->vertices[*start].y = 0;
      g->vertices[*start].type = PATH;

      verts[i] = *start;
    }
    ++i;
  }

  for (int y = 1; y < height - 1; ++y) {
    for (int x = 0; x < width; ++x) {
      pix = imageData[i];
      if (is_path(pix)) {
        int left_i = i - 1;
        int top_i = i - width;
        int right_i = i + 1;
        int bottom_i = i + width;

        bool left = left_i >= 0 ? is_path(imageData[left_i]) : false;
        bool top = top_i >= 0 ? is_path(imageData[top_i]) : false;
        bool right = right_i < num_pixels ? is_path(imageData[right_i]) : false;
        bool bottom =
            bottom_i < num_pixels ? is_path(imageData[bottom_i]) : false;

        // If turn
        if ((left || right) && (top || bottom)) {
          verts[i] = create_vertex(g);
          g->vertices[verts[i]].x = x;
          g->vertices[verts[i]].y = y;
          g->vertices[verts[i]].type = PATH;

          if (left) {
            int j = left_i;
            // Check left for a vertex until you find one or hit a wall
            do {
              if (verts[j] != ~(size_t)0) {
                add_undirected_edge(g, verts[i], verts[j]);
              }
              --j;
            } while (j > 0 && j > top_i && is_path(imageData[j]));
          }

          if (top) {
            int j = top_i;
            // Check above for a vertex until you find one or hit a wall
            do {
              if (verts[j] != ~(size_t)0) {
                add_undirected_edge(g, verts[i], verts[j]);
              }
              j -= width;
            } while (j > 0 && is_path(imageData[j]));
          }
        }
      }
      ++i;
    }
  }

  for (int x = 0; x < width; ++x) {
    pix = imageData[i];
    if (is_path(pix)) {
      *end = create_vertex(g);
      g->vertices[*end].x = x;
      g->vertices[*end].y = height - 1;
      g->vertices[*end].type = PATH;

      int top_i = i - width;
      bool top = top_i >= 0 ? is_path(imageData[top_i]) : false;
      assert(top);

      add_undirected_edge(g, *end, verts[top_i]);
    }
    ++i;
  }

  free(verts);

  stbi_image_free(imageData);

  return g;
}

int write_image(const char *in_image_file, const char *out_image_file,
                const Path *p, const Graph *g) {
  int width, height, channels;
  unsigned char *imageData =
      stbi_load(in_image_file, &width, &height, &channels, 3);
  if (!imageData) {
    fprintf(stderr, "Error loading BMP file: %s\n", stbi_failure_reason());
    return EXIT_FAILURE;
  }

  for (int i = p->size - 1; i > 0; --i) {
    Vertex *curr = g->vertices + p->path[i];
    Vertex *next = g->vertices + p->path[i - 1];

    assert((curr->x == next->x) || (curr->y == next->y));

    size_t curr_i = (curr->y * width + curr->x) * channels;
    size_t next_i = (next->y * width + next->x) * channels;

    size_t min_i = curr_i < next_i ? curr_i : next_i;
    size_t max_i = curr_i < next_i ? next_i : curr_i;

    if (curr->x == next->x) {
      while (min_i <= max_i) {
        assert(imageData[min_i] != 0x0);
        imageData[min_i] = 0xFF;
        imageData[min_i + 1] = 0x0;
        imageData[min_i + 2] = 0x0;
        min_i += width * channels;
      }
    } else if (curr->y == next->y) {
      while (min_i <= max_i) {
        assert(imageData[min_i] != 0x0);
        imageData[min_i] = 0xFF;
        imageData[min_i + 1] = 0x0;
        imageData[min_i + 2] = 0x0;
        min_i += channels;
      }
    }
  }

  int result =
      stbi_write_bmp(out_image_file, width, height, channels, imageData);

  stbi_image_free(imageData);

  if (!result) {
    fprintf(stderr, "Error writing image to %s\n", out_image_file);
  }

  return EXIT_SUCCESS;
}

bool is_path(unsigned char pixel) {
  assert(pixel == 0x0 || pixel == 0xFF);
  return pixel == 0xFF;
}
