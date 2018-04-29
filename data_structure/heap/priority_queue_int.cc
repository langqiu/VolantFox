#include <iostream>
#include "priority_queue_int.h"
using namespace std;

PriorityQueueInt::PriorityQueueInt() :
  p_heap_(NULL) {
  p_heap_ = new Heap<int>();
}

PriorityQueueInt::~PriorityQueueInt() {
  delete p_heap_;
  p_heap_ = NULL;
}

void PriorityQueueInt::Init(int *data, int n) {
  p_heap_->Init(data, n);
}

void PriorityQueueInt::Insert(int val) {
  p_heap_->Push(val);
}

int PriorityQueueInt::Top() {
  return p_heap_->Top();
}

void PriorityQueueInt::Pop() {
  p_heap_->Pop();
}

void PriorityQueueInt::IncreaseKey(int node, int val) {
  p_heap_->IncreaseKey(node, val);
}

void PriorityQueueInt::Print() {
  p_heap_->Print();
}
