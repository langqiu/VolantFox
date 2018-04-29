#ifndef __H_PRIORITY_QUEUE_H__
#define __H_PRIORITY_QUEUE_H__

#include "heap.h"

class PriorityQueueInt {
  public:
    PriorityQueueInt();
    ~PriorityQueueInt();
    void Init(int *data, int n);
    void Insert(int val);
    int Top();
    void Pop();
    void IncreaseKey(int node, int val);
    void Print();
  private:
    Heap<int> *p_heap_;
};

#endif /* ifndef __H_PRIORITY_QUEUE_H__ */
