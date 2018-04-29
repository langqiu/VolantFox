#include <iostream>
#include <cmath>
using namespace std;

int main() {
  int i=pow(2, 30)+pow(2, 23)+pow(2, 22); // 2^(128+1-127) * (1+2^(-1))
  cout << i << endl;
  float f=*(float*)&i;
  cout << f << endl;
  return 0;
}
