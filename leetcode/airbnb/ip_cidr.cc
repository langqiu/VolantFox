#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<pair<unsigned int, int>> cidr(unsigned int ip, int val) {
    vector<pair<unsigned int, int>> ans;
    unsigned int max_val = ip+val;
    unsigned n = ip;
    while (n<=max_val) {
      unsigned temp = n;
      unsigned int pow_val = 2;
      int count = 0;
      //
      while (temp+pow_val-1<=max_val && temp%pow_val==0) {
        count++;
        pow_val <<= 1;
      }
      //
      pow_val >>= 1;
      ans.push_back(make_pair(temp, 32-count));
      //
      n = temp+pow_val;
    }
    return ans;
  }
};

int main() {
  Solution s;
  unsigned int ip = 0x100;
  int val = 17;
  vector<pair<unsigned int, int>> ans;
  ans = s.cidr(ip, val);
  for (auto v : ans) {
    cout << v.first << "," << v.second << endl;
  }
  return 0;
}
