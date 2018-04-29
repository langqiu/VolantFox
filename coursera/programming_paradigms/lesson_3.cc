#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  // strcpy and strdup
  //char name[6];
  char *name=new char[6];
  strcpy(name, "huwei");
  cout << name << endl;
  string str=strdup("hello");
  cout << str << endl;
  // how does float looks like in memory
  short s=65;
  cout << sizeof(s) << endl;
  char ch=*(char*)&s;
  cout << ch << endl;
  return 0;
}
