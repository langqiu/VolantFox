#include <assert.h>
#include "stack_with_two_queue.h"
using namespace std;

StackWithQueue::StackWithQueue() :
  size_(0), pop_queue_(1) {}

StackWithQueue::~StackWithQueue() {}

void StackWithQueue::Push(int val) {
  queue_pair_[1-pop_queue_].push(val);
  while (!queue_pair_[pop_queue_].empty()) {
    queue_pair_[1-pop_queue_].push(queue_pair_[pop_queue_].front());
    queue_pair_[pop_queue_].pop();
  }
  pop_queue_ = 1-pop_queue_;
  size_++;
}

void StackWithQueue::Pop() {
  assert(size_ > 0);
  queue_pair_[pop_queue_].pop();
  size_--;
}

int StackWithQueue::Top() {
  assert(size_ > 0);
  return queue_pair_[pop_queue_].front();
}
