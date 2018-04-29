#include <iostream>
using namespace std;


/**
 * @brief compare two ints
 *
 * @param vp1: address of the first int
 * @param vp2: address of the second int
 *
 * @return 0 if equals, positive value if *vp1>*vp2
 */
int IntCmp(void *vp1, void *vp2) {
  int *ip1=(int*)vp1;
  int *ip2=(int*)vp2;
  return *ip1-*ip2;
}

/**
 * @brief compare two string
 *
 * @param vp1: address of first char*
 * @param vp2: address of second char*
 *
 * @return 0, 1, -1
 */
int StrCmp(void *vp1, void *vp2) {
  char *chp1=*(char* *)vp1;
  char *chp2=*(char* *)vp2;
  return strcmp(chp1, chp2);
}

/**
 * @brief find the target in an array
 *
 * @param key: address of the target
 * @param base: base address of the array
 * @param n: length of the array
 * @param elem_size: byte size of the element
 * @param cmpFunction: compare function
 *
 * @return address of the element if found, NULL if not
 */
void *linearSearch(void *key, void *base, int n, int elem_size, int (*cmpFunction)(void *vp1, void *vp2)) {
  for (int i=0; i<n; i++) {
    // calculate the address
    void *elem_addr=(char*)base+i*elem_size;
    //if (memcmp(key, elem_addr, elem_size) == 0) {
    if (cmpFunction(key, elem_addr) == 0) {
      return elem_addr;
    }
  }
  return NULL;
}

/**
 * @brief swap two value
 *
 * @param ap: address of first object
 * @param bp: address of second object
 * @param size: size of the object
 */
void mySwap(void *ap, void *bp, int size) {
  char *temp=new char[size];
  // memcpy(to_char_star, from_char_star, size);
  for (int i=0; i<size; i++) {
    temp[i] = ((char*)ap)[i];
    ((char*)ap)[i] = ((char*)bp)[i];
    ((char*)bp)[i] = temp[i];
  }
  delete[] temp;
  return;
}

int main(int argc, char *argv[])
{
  // swap two value
  float x=53.2, y=67.0;
  mySwap(&x, &y, sizeof(float));
  cout << x << "-" << y << endl;

  // linear search
  int array[5]={23,4,16,92,7};
  int key=16;
  int *found_key=(int*)linearSearch(&key, array, sizeof(array)/sizeof(int), sizeof(int), IntCmp);
  if (found_key != NULL) cout << *found_key << endl;

  // char **
  char *favourite_note="D";
  char *notes[]={"Ab", "F#", "B", "Gb", "D"};
  char* *found_note=(char* *)linearSearch(&favourite_note, notes, sizeof(notes)/sizeof(char*), sizeof(char*), StrCmp);
  if (found_note != NULL) cout << *found_note << endl;

  return 0;
}
