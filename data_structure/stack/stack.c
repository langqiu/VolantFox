#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"

void stackNew(stack *s) {
  s->elements = malloc(4*sizeof(int));
  s->malloc_size = 4;
  s->log_size = 0;
  assert(s->elements != NULL);
}

void stackDispose(stack *s) {
  free(s->elements);
  s->elements = NULL;
}

void stackPush(stack *s, int val) {
  if (s->malloc_size == s->log_size) {
    /*
     *int *temp=malloc(s->malloc_size*2*sizeof(int));
     *memcpy(temp, s->elements, s->malloc_size*sizeof(int));
     *s->malloc_size *= 2;
     *free(s->elements);
     *s->elements = temp;
     */
    s->malloc_size *= 2;
    s->elements = realloc(s->elements, s->malloc_size*sizeof(int));
    assert(s->elements != NULL);
  }
  s->elements[s->log_size++] = val;
}

void stackPop(stack *s) {
  assert(s->log_size > 0);
  s->log_size--;
}

int stackTop(stack *s) {
  assert(s->log_size > 0);
  return s->elements[s->log_size-1];
}

bool stackEmpty(stack *s) {
  return s->log_size == 0;
}
