#include <assert.h>
#include "queue_with_two_stack.h"
using namespace std;

QueueWithStack::QueueWithStack() : size_(0){}

QueueWithStack::~QueueWithStack() {}

void QueueWithStack::Push(int val) {
  _FromPopToPush();
  push_stack_.push(val);
  size_++;
}

void QueueWithStack::Pop() {
  assert(size_ > 0);
  _FromPushToPop();
  pop_stack_.pop();
  size_--;
}

int QueueWithStack::Front() {
  assert(size_ > 0);
  _FromPushToPop();
  return pop_stack_.top();
}

int QueueWithStack::Back() {
  assert(size_ > 0);
  _FromPopToPush();
  return push_stack_.top();
}
