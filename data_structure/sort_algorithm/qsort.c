#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *vp1, void *vp2, int size) {
  char *buffer=malloc(size);
  memcpy(buffer, vp1, size);
  memcpy(vp1, vp2, size);
  memcpy(vp2, buffer, size);
  free(buffer);
}

/*
 *void qsort_helper(void *base, int l, int r, int elem_size, int (*cmpfn)(void*, void*)) {
 *  if (l<r) {
 *    void *target=(char*)base+r*elem_size;
 *    int b=l-1;
 *    for (int i=l; i<r; i++) {
 *      void *curr=(char*)base+i*elem_size;
 *      if (cmpfn(curr, target) <= 0) {
 *        b++;
 *        void *temp=(char*)base+b*elem_size;
 *        if (b != i) swap(curr, temp, elem_size);
 *      }
 *    }
 *    swap((char*)base+(b+1)*elem_size, target, elem_size);
 *    qsort_helper(base, l, b, elem_size, cmpfn);
 *    qsort_helper(base, b+2, r, elem_size, cmpfn);
 *  }
 *}
 */

void qsort_helper(void *base, int l, int r, int elem_size, int (*cmpfn)(void*, void*)) {
  if (l<r) {
    // partition
    void *target=(char*)base+l*elem_size;
    int i=l-1, j=r+1;
    while (1) {
      do {
        i++;
      } while (cmpfn((char*)base+i*elem_size, target)<=0 && i<r);
      do {
        j--;
      } while (cmpfn((char*)base+j*elem_size, target)>=0 && j>l);
      if (i < j) swap((char*)base+i*elem_size, (char*)base+j*elem_size, elem_size);
      else break;
    }
    swap((char*)base+j*elem_size, target, elem_size);
    qsort_helper(base, l, j-1, elem_size, cmpfn);
    qsort_helper(base, j+1, r, elem_size, cmpfn);
  }
}

void myqsort(void *base, int n, int elem_size, int (*cmpfn)(void*, void*)) {
  qsort_helper(base, 0, n-1, elem_size, cmpfn);
}

int FloatCmp(void *vp1, void *vp2) {
  float *fp1=(float*)vp1;
  float *fp2=(float*)vp2;
  if (*fp1-*fp2>0) return 1;
  if (*fp1-*fp2<0) return -1;
  return 0;
}

int main() {
  float f[]={3.4, 1.0, 12, 33.8, 24, 5.9, 3.14, 81};
  myqsort(f, sizeof(f)/sizeof(float), sizeof(float), &FloatCmp);
  for (int i=0; i<sizeof(f)/sizeof(float); i++) {
    printf("%.2f\n", f[i]);
  }
  return 0;
}
