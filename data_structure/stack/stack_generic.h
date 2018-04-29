#ifndef __H_STACK_GENERIC_H__
#define __H_STACK_GENERIC_H__

#include <stdbool.h>

typedef struct {
  void *elements;
  int elem_size;
  int malloc_length;
  int log_length;
  void (*free_fn)(void*);
} stack;

void stackNew(stack *s, int elem_size, void (*free_fn)(void*));
void stackDispose(stack *s);
void stackPush(stack *s, void *val_addr);
void stackPop(stack *s);
void stackTop(stack *s, void *elem_addr); // whether or not, return the address of memory that is dynamically allocated in the function and pass in the address of client-supplied memory.i
                                          // you must get in the habit of making any function that allcates the memory be the thing that deallocates it as well.
bool stackEmpty(stack *s);

#endif /* ifndef __H_STACK_GENERIC_H__ */
