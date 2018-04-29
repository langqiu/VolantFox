#include <iostream>
using namespace std;

void rotate(void *front, void *middle, void *end) {
  int front_size=(char*)middle-(char*)front;
  int end_size=(char*)end-(char*)middle;
  char *buffer=new char[front_size];
  memcpy(buffer, front, front_size);
  memmove(front, middle, end_size);
  memcpy((char*)end-front_size, buffer, front_size);
}

int main(int argc, char *argv[])
{
  char name[]={'h','u','w','e','i','\0'};
  rotate(name, name+2, name+5);
  cout << name << endl;
  return 0;
}
