#ifndef __H_MY_QUEUE_H__
#define __H_MY_QUEUE_H__

#include <vector>
#include <assert.h>

template <typename T>
class MyQueue {
  public:
    MyQueue();
    ~MyQueue();
    void Push(T val);
    void Pop();
    T Front();
    bool Empty();
    bool Full();
  private:
    std::vector<T> elements_;
    int head_;
    int tail_;
    int max_len_;
};

template <typename T>
MyQueue<T>::MyQueue() :
  head_(0),
  tail_(0),
  max_len_(8) {
  elements_.reserve(max_len_);
}

template <typename T>
MyQueue<T>::~MyQueue() {
}

template <typename T>
void MyQueue<T>::Push(T val) {
  assert(!Full());
  elements_[tail_] = val;
  tail_ = (tail_+1)%max_len_;
}

template <typename T>
void MyQueue<T>::Pop() {
  assert(!Empty());
  head_ = (head_+1)%max_len_;
}

template <typename T>
T MyQueue<T>::Front() {
  assert(!Empty());
  return elements_[head_];
}

template <typename T>
bool MyQueue<T>::Empty() {
  return head_ == tail_;
}

template <typename T>
bool MyQueue<T>::Full() {
  int delta=head_-tail_;
  if (delta < 0) delta += max_len_;
  return delta == 1;
}

#endif /* ifndef __H_MY_QUEUE_H__ */
