#include <iostream>
#include "heap_int.h"
#include <assert.h>
using namespace std;

MyHeapInt::MyHeapInt() : heap_size_(0), malloc_size_(0) {
  malloc_size_ = 4;
  elements_ = new int[malloc_size_];
  assert(elements_ != NULL);
}

MyHeapInt::~MyHeapInt() {
  delete[] elements_;
}

void MyHeapInt::Initialize(int *nums, int n) {
  if (n > malloc_size_) {
    while (malloc_size_ < n) {
      malloc_size_ = malloc_size_<<1;
    }
    int *temp=new int[malloc_size_];
    assert(temp != NULL);
    delete[] elements_;
    elements_ = temp;
  }
  memcpy(elements_, nums, n*sizeof(int));
  heap_size_ = n;
  _BuildHeap();
}

void MyHeapInt::PushHeap(int num) {
  if (heap_size_ == malloc_size_) {
    _GrowArray();
  }
  elements_[heap_size_] = INT_MIN;
  heap_size_++;
  _AdjustUp(heap_size_-1, num);
}

void MyHeapInt::PopHeap() {
  assert(heap_size_ > 0);
  swap(elements_[heap_size_-1], elements_[0]);
  heap_size_--;
  if (heap_size_ > 0) _AdjustHeap(0);
}

int MyHeapInt::TopHeap() {
  assert(heap_size_ > 0);
  return elements_[0];
}

void MyHeapInt::PrintHeap() {
  for (int i=0; i<heap_size_; i++) {
    if (i != 0) cout << ",";
    cout << elements_[i];
  }
  cout << endl;
}

void MyHeapInt::SortHeap() {
  while (heap_size_ > 0) {
    PopHeap();
  }
}

void MyHeapInt::_BuildHeap() {
  for (int i=heap_size_/2-1; i>=0; i--) {
    _AdjustHeap(i);
  }
}

void MyHeapInt::_AdjustHeap(int i) {
  int l=_LeftNode(i), r=_RightNode(i);
  int max_val=elements_[i], largest=i;
  if (l<heap_size_ && elements_[l]>max_val) {
    max_val = elements_[l];
    largest = l;
  }
  if (r<heap_size_ && elements_[r]>max_val) {
    largest = r;
  }
  if (largest != i) {
    swap(elements_[i], elements_[largest]);
    _AdjustHeap(largest);
  }
}

void MyHeapInt::_AdjustUp(int i, int num) {
  assert(elements_[i] < num);
  elements_[i] = num;
  int j=_ParentNode(i);
  while (j>=0 &&
      elements_[i] > elements_[j]) {
    swap(elements_[i], elements_[j]);
    i = j;
    j = _ParentNode(i);
  }
}
