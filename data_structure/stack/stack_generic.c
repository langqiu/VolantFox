#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack_generic.h"

void stackNew(stack *s, int elem_size, void (*free_fn)(void*)) {
  s->free_fn = free_fn;
  s->elem_size = elem_size;
  s->malloc_length = 4;
  s->log_length = 0;
  s->elements = malloc(4*s->elem_size);
  assert(s->elements != NULL);
}

void stackDispose(stack *s) {
  if (s->free_fn != NULL) {
    for (int i=0; i<s->log_length; i++) {
      s->free_fn((char*)s->elements+i*s->elem_size);
    }
  }
  free(s->elements);
  s->elements = NULL;
}

static void stackGrow(stack *s) {
  s->malloc_length *= 2;
  s->elements = realloc(s->elements, s->malloc_length*s->elem_size);
  assert(s->elements != NULL);
}
void stackPush(stack *s, void *val_addr) {
  if (s->malloc_length == s->log_length) {
    stackGrow(s);
  }
  void *temp=(char*)s->elements+s->log_length*s->elem_size;
  memcpy(temp, val_addr, s->elem_size);
  s->log_length++;
}

void stackPop(stack *s) {
  assert(s->log_length > 0);
  s->log_length--;
}

void stackTop(stack *s, void *elem_addr) {
  assert(s->log_length > 0);
  void *temp=(char*)s->elements+(s->log_length-1)*s->elem_size;
  memcpy(elem_addr, temp, s->elem_size);
}

bool stackEmpty(stack *s) {
  return s->log_length == 0;
}
