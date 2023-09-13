#include <stdlib.h>

typedef struct Node {
  size_t val;
  struct Node *next;
} Node;

typedef struct Queue {
  Node *head;
  Node *tail;
  size_t size;
} Queue;

Queue *createQueue();
size_t pop(Queue *q);
void push(Queue *q, size_t val);
