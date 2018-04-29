#ifndef __H_HEAP_H__
#define __H_HEAP_H__

#include <assert.h>

class MyHeapInt {
  public:
    MyHeapInt();
    ~MyHeapInt();
    void Initialize(int *nums, int n);
    void PushHeap(int num);
    void PopHeap();
    int TopHeap();
    void PrintHeap();
    void SortHeap();
    inline int* get_elements_() { return elements_; };
  private:
    inline int _LeftNode(int i) { return 2*(i+1)-1; };
    inline int _RightNode(int i) { return 2*(i+1); };
    inline int _ParentNode(int i) { return i==0? -1:(i-1)/2; };
    inline void _GrowArray() {
      int *temp=new int[malloc_size_<<1];
      assert(temp != NULL);
      memcpy(temp, elements_, malloc_size_*sizeof(int));
      free(elements_);
      elements_ = temp;
      malloc_size_ = malloc_size_<<1;
    };
    void _BuildHeap();
    void _AdjustHeap(int i);
    void _AdjustUp(int i, int num);
    int *elements_;
    int heap_size_;
    int malloc_size_;
};

#endif /* ifndef __H_HEAP_H__ */
