/*
 *#include <stdio.h>
 *#include "stack.h"
 *
 *int main(int argc, char *argv[])
 *{
 *  stack st;
 *  stackNew(&st);
 *  for (int i=0; i<9; i++) {
 *    stackPush(&st, i);
 *    printf("top_elem:%d\n", stackTop(&st));
 *  }
 *  stackPop(&st);
 *  printf("empty:%d\n", stackEmpty(&st));
 *  stackPop(&st);
 *  printf("empty:%d\n", stackEmpty(&st));
 *  printf("top:%d\n", stackTop(&st));
 *  stackDispose(&st);
 *  return 0;
 *}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack_generic.h"

void StringFreeFunc(void *val_addr) {
  char *val=*(char**)val_addr;
  printf("before_free:%s\n", val);
  free(val);
}

int main(int argc, char *argv[]) {
  char *friends[]={"Allen", "Bob", "Carl"};
  stack st;
  stackNew(&st, sizeof(char*), &StringFreeFunc);
  for (int i=0; i<3; i++) {
    char *copy=strdup(friends[i]);
    stackPush(&st, &copy);
  }
  /*
   *while (!stackEmpty(&st)) {
   *  char *name;
   *  stackTop(&st, &name);
   *  stackPop(&st);
   *  printf("string:%s\n", name);
   *  free(name);
   *}
   */
  stackDispose(&st);
  return 0;
}
