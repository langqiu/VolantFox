#include <iostream>
#include "stack_with_two_queue.h"
using namespace std;

int main(int argc, char *argv[])
{
  StackWithQueue my_stack;
  cout << "push" << endl;
  my_stack.Push(1);
  my_stack.Push(2);
  my_stack.Push(3);
  my_stack.Push(4);
  my_stack.Push(5);
  my_stack.Push(6);
  cout << "size" << my_stack.Size() << endl;
  while (!my_stack.Empty()) {
    cout << "top" << my_stack.Top() << endl;
    my_stack.Pop();
  }
  cout << "empty" << my_stack.Empty() << endl;
  return 0;
}
