#include <iostream>
#include "queue_with_two_stack.h"
using namespace std;

int main(int argc, char *argv[])
{
  QueueWithStack my_queue;
  cout << "push 4" << endl;
  my_queue.Push(4);
  cout << "front" << my_queue.Front() << endl;
  cout << "push 18" << endl;
  my_queue.Push(18);
  cout << "front" << my_queue.Front() << endl;
  cout << "push 30" << endl;
  my_queue.Push(30);
  cout << "back" << my_queue.Back() << endl;
  cout << "push 7" << endl;
  my_queue.Push(7);
  cout << "back" << my_queue.Back() << endl;
  cout << "pop 4" << endl;
  my_queue.Pop();
  cout << "front" << my_queue.Front() << endl;
  cout << "pop 18" << endl;
  my_queue.Pop();
  cout << "front" << my_queue.Front() << endl;
  cout << "push 40" << endl;
  my_queue.Push(40);
  cout << "back" << my_queue.Back() << endl;
  return 0;
}
