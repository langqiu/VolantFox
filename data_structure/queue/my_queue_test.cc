#include <iostream>
#include "my_queue.h"
using namespace std;

int main(int argc, char *argv[])
{
  MyQueue<int> my_queue;
  for (int i=1; i<8; i++) {
    my_queue.Push(i);
    cout << my_queue.Front() << "," << my_queue.Full() << endl;
  }
  cout << endl;
  for (int i=1; i<8; i++) {
    cout << my_queue.Front() << ",";
    my_queue.Pop();
    cout << my_queue.Empty() << endl;
  }
  return 0;
}
