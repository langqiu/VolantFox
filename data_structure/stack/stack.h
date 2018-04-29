#ifndef __H_STACK_H__
#define __H_STACK_H__

#include <stdbool.h>

typedef struct {
  int *elements;
  int malloc_size;
  int log_size;
} stack;

void stackNew(stack *s);
void stackDispose(stack *s);
void stackPush(stack *s, int val);
void stackPop(stack *s);
int stackTop(stack *s);
bool stackEmpty(stack *s);

#endif /* ifndef __H_STACK_H__ */
