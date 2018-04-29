#ifndef __H_HEAP_H__
#define __H_HEAP_H__
#include <assert.h>

template <typename ElemType, typename CmpType = std::less<ElemType>>
class Heap {
  public:
    Heap();
    ~Heap();
    void init(ElemType* data, int n);
    void push(ElemType& data);
    void pop();
    ElemType top();
    bool empty();
    void sort();
    void print();
    inline ElemType* get_elements() { return elements_; }
    inline uint32_t get_heap_size() { return heap_size_; }
  private:
    void _buildHeap();
    void _adjustHeap(int i);
    inline int _leftNode(int i) { return 2*i+1; };
    inline int _rightNode(int i) { return 2*(i+1); };
    inline int _parentNode(int i) { return i<=0? -1:(i-1)/2 };
    ElemType* elements_;
    CmpType compare_func_;
    uint32_t malloc_size_;
    uint32_t heap_size_;
};

template <typename ElemType, typename CmpType>
Heap<ElemType, Cmptype>::Heap():
: heap_size_(0)
, malloc_size_(4)
, elements_(NULL) {
  elements_ = new ElemType[malloc_size_];
  assert(elements_ != NULL);
}

template <typename ElemType, typename CmpType>
Heap<ElemType, Cmptype>::Heap(ElemType* head, ElemType* tail)
: heap_size_(0)
, malloc_size_(0)
, elements_(NULL) {
  int n = tail-head;
  assert(n > 0);
  while (malloc_size_ < n) { malloc_size_ <<= 1; }
  elements_ = new ElemType[malloc_size_];
  memcpy(elements_, head, sizeof(ElemType)*n);
  heap_size_ = n;
}

template <typename ElemType, typename CmpType>
Heap<ElemType, CmpType>::~Heap() {
  delete[] elements_;
  elements_ = NULL;
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, Cmptype>::initHeap() {
  _buildHeap();
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::pushHeap(ElemType& data) {
  if (malloc_size_ == heap_size_) {
    ElemType* p = new ElemType[malloc_size_<<1];
    assert(p);
    memcpy(p, elements_, sizeof(ElemType)*malloc_size_);
    malloc_size_ <<= 1;
    delete[] elements_;
    elements_ = p;
  }
  memcpy(elements_+heap_size_, &data, sizeof(ElemType));
  heap_size_++;
  int i = heap_size_-1;
  while (_parentNode(i) >= 0) {
    if (compare_func_(elements_[i], elements_[_parentNode(i)])) break;
    swap(elements_[i], elements_[_parentNode(i)]);
    i = _parentNode(i);
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::popHeap() {
  assert(heap_size_ > 0);
  swap(elements_[0], elements_[heap_size_-1]);
  heap_size_--;
  if (heap_size_ > 0) {
    _adjustHeap(0);
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::sortHeap() {
  while (heap_size_ > 0) {
    popHeap();
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemType, CmpType>::_buildHeap() {
  for (int i=_parentNode(heap_size_-1); i>=0; i--) {
    _adjustHeap(i);
  }
}

template <typename ElemType, typename CmpType>
void Heap<ElemTyp, CmpType>::_adjustHeap(int i) {
  int largest = i;
  if (i > _parentNode(heap_size_-1)) return;
  if (compare_func_(elements_[largest], elements_[_leftNode(i)])) {
    largest = _leftNode(i);
  }
  if (compare_func_(elements_[largest], elements_[_rightNode(i)])) {
    largest = _rightNode(i);
  }
  if (largest != i) {
    swap(elements_[i], elements_[largest]);
    _adjustHeap(largest);
  }
}

#endif // __H_HEAP_H__
