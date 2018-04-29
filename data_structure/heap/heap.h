#ifndef __H_HEAP_H__
#define __H_HEAP_H__

#include <iostream>
#include <assert.h>

template <typename ElemType, typename CmpType = std::less<ElemType> >
class Heap {
  public:
    Heap();
    ~Heap();
    void Init(ElemType *data, int n);
    void Push(ElemType val);
    void Pop();
    ElemType Top();
    bool Empty();
    void Sort();
    void Print();
    void IncreaseKey(int i, ElemType val);
    inline ElemType* get_elements() { return elements_; };
    inline int get_heap_size() { return heap_size_; };
  private:
    void _Build();
    void _Adjust(int i);
    inline int _LeftNode(int i) { return 2*(i+1)-1; };
    inline int _RightNode(int i) { return 2*(i+1); };
    inline int _ParentNode(int i) { return i==0? -1:(i-1)/2; };
    ElemType *elements_;
    int heap_size_;
    int malloc_size_;
    CmpType cmp_struct_;
};

template <typename ElemType, typename CmpType>
Heap<ElemType, CmpType>::Heap() :
  elements_(NULL),
  heap_size_(0),
  malloc_size_(4) {
  elements_ = new ElemType[malloc_size_];
  assert(elements_ != NULL);
}

template <typename ElemType, typename CmpType>
Heap<ElemType, CmpType>::~Heap() {
  if (elements_ != NULL) {
    delete elements_;
    elements_ = NULL;
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::Init(ElemType *data, int n) {
  // input detection
  assert(data != NULL && n != 0);
  if (n > malloc_size_) {
    while (malloc_size_ < n) {
      malloc_size_ = malloc_size_ << 1;
    }
    ElemType *temp = new ElemType[malloc_size_];
    assert(temp != NULL);
    delete elements_;
    elements_ = temp;
  }
  memcpy(elements_, data, n*sizeof(ElemType));
  heap_size_ = n;
  _Build();
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::Push(ElemType val) {
  if (heap_size_ == malloc_size_) {
    malloc_size_ = malloc_size_ << 1;
    ElemType *temp = new ElemType[malloc_size_];
    assert(temp != NULL);
    memcpy(temp, elements_, heap_size_*sizeof(ElemType));
    delete elements_;
    elements_ = temp;
  }
  elements_[heap_size_] = val;
  heap_size_++;
  IncreaseKey(heap_size_-1, elements_[heap_size_-1]);
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::Pop() {
  assert(heap_size_ > 0);
  std::swap(elements_[0], elements_[heap_size_-1]);
  heap_size_--;
  if (heap_size_ > 0) _Adjust(0);
}

template <typename ElemType, typename CmpType>
ElemType Heap<ElemType, CmpType>::Top() {
  assert(heap_size_ > 0);
  return elements_[0];
}

template <typename ElemType, typename CmpType>
bool Heap<ElemType, CmpType>::Empty() {
  return heap_size_ == 0;
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::Sort() {
  while (heap_size_ > 1) {
    Pop();
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::Print() {
  std::cout << "heap: ";
  for (int i=0; i<heap_size_; i++) {
    if (i != 0) std::cout << ",";
    std::cout << elements_[i];
  }
  std::cout << std::endl;
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::IncreaseKey(int i, ElemType val) {
  assert(i >= 0 && i < heap_size_);
  assert(!cmp_struct_(val, elements_[i]));
  elements_[i] = val;
  int parent_node = _ParentNode(i);
  while (parent_node >=0 && cmp_struct_(elements_[parent_node], elements_[i])) {
    std::swap(elements_[parent_node], elements_[i]);
    i = parent_node;
    parent_node = _ParentNode(i);
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::_Build() {
  for (int i=_ParentNode(heap_size_-1); i>=0; i--) {
    _Adjust(i);
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::_Adjust(int i) {
  assert(i >= 0 && i < heap_size_);
  int l = _LeftNode(i), r = _RightNode(i);
  int largest=i;
  if (l < heap_size_ && cmp_struct_(elements_[largest], elements_[l])) {
    largest = l;
  }
  if (r < heap_size_ && cmp_struct_(elements_[largest], elements_[r])) {
    largest = r;
  }
  if (largest != i) {
    std::swap(elements_[largest], elements_[i]);
    _Adjust(largest);
  }
}

#endif /* ifndef __H_HEAP_H__ */
