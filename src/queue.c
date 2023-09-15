#include "../include/queue.h"
#include <stddef.h>
#include <stdlib.h>

Queue *createQueue() {
  Queue *q = malloc(1 * sizeof(Queue));
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
  return q;
}

size_t pop(Queue *q) {
  Node *node = q->head;
  q->head = node->next;

  if (q->head == NULL) {
    q->tail = NULL;
  }

  size_t val = node->val;
  free(node);

  --q->size;

  return val;
}

void push(Queue *q, size_t val) {
  Node *node = malloc(sizeof(Node));
  node->val = val;
  node->next = NULL;

  if (q->tail != NULL) {
    q->tail->next = node;
  }
  q->tail = node;

  if (q->head == NULL) {
    q->head = q->tail;
  }

  ++q->size;
}

void deleteQueue(Queue *q) {
  while (q->size != 0) {
    pop(q);
  }
  free(q);
}
