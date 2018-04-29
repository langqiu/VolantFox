#include <iostream>
#include "priority_queue_int.h"
using namespace std;

int main(int argc, char *argv[])
{
  PriorityQueueInt my_priority;
  int nums[]={4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
  //
  cout << "init" << endl;
  my_priority.Init(nums, sizeof(nums)/sizeof(int));
  my_priority.Print();
  cout << "push 19" << endl;
  my_priority.Insert(19);
  my_priority.Print();
  cout << "increase 5 to 100" << endl;
  my_priority.IncreaseKey(5, 100);
  my_priority.Print();
  cout << "pop" << endl;
  my_priority.Pop();
  my_priority.Print();

  return 0;
}
