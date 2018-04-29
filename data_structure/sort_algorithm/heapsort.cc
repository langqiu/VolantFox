#include <iostream>
#include <vector>
using namespace std;

int leftNode(int i) {
  return 2*(i+1)-1;
}

int rightNode(int i) {
  return 2*(i+1);
}

int parentNode(int i) {
  return i==0? -1:(i-1)/2;
}

void maxHeapify(vector<int>& vec, int i, int heap_size) {
  int l=leftNode(i), r=rightNode(i), largest=i;
  if (l<heap_size && vec[l]>vec[largest]) {
    largest = l;
  }
  if (r<heap_size && vec[r]>vec[largest]) {
    largest = r;
  }
  if (largest != i) {
    swap(vec[i], vec[largest]);
    maxHeapify(vec, largest, heap_size);
  }
  return;
}

void buildHeap(vector<int>& vec) {
  int last=parentNode(vec.size()-1);
  for (int i=last; i>=0; i--) {
    maxHeapify(vec, i, vec.size());
  }
  return;
}

void myHeapSort(vector<int>& vec) {
  buildHeap(vec);
  int heap_size=vec.size();
  for (int i=vec.size()-1; i>0; i--) {
    swap(vec[i], vec[0]);
    heap_size--;
    maxHeapify(vec, 0, heap_size);
  }
  return;
}

int main(int argc, char *argv[])
{
  vector<int> nums={4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
  myHeapSort(nums);
  for (int i=0; i<nums.size(); i++) {
    if (i != 0) cout << ",";
    cout << nums[i];
  }
  cout << endl;
  return 0;
}
