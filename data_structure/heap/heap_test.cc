#include <iostream>
#include "heap.h"
using namespace std;

int main(int argc, char *argv[])
{
  Heap<int, less<int> > heap;
  int nums[]={4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
  //
  cout << "init" << endl;
  heap.Init(nums, sizeof(nums)/sizeof(int));
  heap.Print();
  cout << "sort" << endl;
  heap.Sort();
  for (int i=0; i<sizeof(nums)/sizeof(int); i++) {
    if (i != 0) cout << ",";
    cout << heap.get_elements()[i];
  }
  cout << endl;
  //
  cout << "init again" << endl;
  heap.Init(nums, sizeof(nums)/sizeof(int));
  heap.Print();
  cout << "push 7 number" << endl;
  heap.Push(11);
  heap.Push(23);
  cout << "push 23" << endl;
  cout << heap.Top() << endl;
  heap.Push(20);
  heap.Push(17);
  heap.Push(5);
  heap.Push(19);
  heap.Push(6);
  heap.Print();
  cout << "pop" << endl;
  heap.Pop();
  heap.Print();
  cout << "sort" << endl;
  heap.Sort();
  for (int i=0; i<sizeof(nums)/sizeof(int)+7; i++) {
    if (i != 0) cout << ",";
    cout << heap.get_elements()[i];
  }
  cout << endl;
  return 0;
}
