#ifndef __H_QUEUE_WITH_TWO_STACK_H__
#define __H_QUEUE_WITH_TWO_STACK_H__

#include <stack>

class QueueWithStack {
  public:
    QueueWithStack();
    ~QueueWithStack();
    void Push(int val);
    void Pop();
    int Front();
    int Back();
    inline bool Empty() { return size_==0; };
    inline int Size() { return size_; };
  private:
    inline void _FromPushToPop() {
      while (!push_stack_.empty()) {
        pop_stack_.push(push_stack_.top());
        push_stack_.pop();
      }
    };
    inline void _FromPopToPush() {
      while (!pop_stack_.empty()) {
        push_stack_.push(pop_stack_.top());
        pop_stack_.pop();
      }
    };
    std::stack<int> push_stack_, pop_stack_;
    int size_;
};

#endif /* ifndef __H_QUEUE_WITH_TWO_STACK_H__ */
