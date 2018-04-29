#include <iostream>
#include "heap_int.h"
using namespace std;

int main(int argc, char *argv[])
{
  int nums[]={4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
  MyHeapInt my_heap;
  //
  cout << "init" << endl;
  my_heap.Initialize(nums, sizeof(nums)/sizeof(int));
  my_heap.PrintHeap();
  cout << "sort" << endl;
  my_heap.SortHeap();
  for (int i=0; i<sizeof(nums)/sizeof(int); i++) {
    if (i != 0) cout << ",";
    cout << my_heap.get_elements_()[i];
  }
  cout << endl;
  //
  cout << "init anain" << endl;
  my_heap.Initialize(nums, sizeof(nums)/sizeof(int));
  my_heap.PrintHeap();
  cout << "push 7 number" << endl;
  my_heap.PushHeap(11);
  my_heap.PushHeap(23);
  cout << "push 23" << endl;
  cout << my_heap.TopHeap() << endl;
  my_heap.PushHeap(20);
  my_heap.PushHeap(17);
  my_heap.PushHeap(5);
  my_heap.PushHeap(19);
  my_heap.PushHeap(6);
  my_heap.PrintHeap();
  cout << "pop" << endl;
  my_heap.PopHeap();
  my_heap.PrintHeap();
  cout << "sort" << endl;
  my_heap.SortHeap();
  for (int i=0; i<sizeof(nums)/sizeof(int)+7; i++) {
    if (i != 0) cout << ",";
    cout << my_heap.get_elements_()[i];
  }
  cout << endl;
  return 0;
}
