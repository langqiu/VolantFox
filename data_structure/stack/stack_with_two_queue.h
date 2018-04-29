#ifndef __H_STACK_WITH_TWO_QUEUE_H__
#define __H_STACK_WITH_TWO_QUEUE_H__

#include <queue>

class StackWithQueue {
  public:
    StackWithQueue();
    ~StackWithQueue();
    void Push(int val);
    void Pop();
    int Top();
    inline bool Empty() { return size_==0; };
    inline int Size() { return size_; };
  private:
    std::queue<int> queue_pair_[2];
    int pop_queue_;
    int size_;
};

#endif /* ifndef __H_STACK_WITH_TWO_QUEUE_H__ */
