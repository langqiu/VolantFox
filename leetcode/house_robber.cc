#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
  vector<int> houseRobber(vector<int> house_list) {
    vector<int> ans;
    int n = house_list.size();
    int* d = new int[n];
    memset(d, 0, sizeof(int)*n);
    queue<vector<int>> q;
    vector<int> v1, v2;
    // init
    for (int i=0; i<n; i++) {
      if (i==0) {
        d[i] = house_list[0];
        v1.push_back(i);
        q.push(v1);
      } else if (i==1) {
        d[i] = house_list[0]>house_list[1]? house_list[0]:house_list[1];
        v2.push_back(house_list[0]>house_list[1]? 0:1);
        q.push(v2);
      } else {
        if (d[i-1]>d[i-2]+house_list[i]) {
          d[i] = d[i-1];
          q.pop();
          q.push(q.front());
        } else {
          d[i] = d[i-2]+house_list[i];
          vector<int> v = q.front();
          v.push_back(i);
          q.pop();
          q.push(v);
        }
      }
    }
    vector<int> v;
    ans.push_back(d[n-1]>d[n-2]? d[n-1]:d[n-2]);
    if (d[n-2]>d[n-1]) {
      v = q.front();
    } else {
      v = q.back();
    }
    for (auto val : v) {
      ans.push_back(val);
    }
    return ans;
  }
};

int main() {
  Solution s;
  vector<int> house_list = {1};
  for (auto v : house_list) {
    cout << v << ",";
  }
  cout << endl;
  vector<int> ans;
  ans = s.houseRobber(house_list);
  for (auto v : ans) {
    cout << v << ",";
  }
  cout << endl;
  return 0;
}
