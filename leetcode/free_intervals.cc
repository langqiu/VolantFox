#include <vector>
#include <iostream>
using namespace std;

bool cmp(pair<int, int>& l, pair<int, int>& r) {
  return l.first < r.first;
}

class Solution {
public:
  vector<pair<int, int>> freeIntervals(vector<pair<int, int>>& intervals) {
    vector<pair<int, int>> timeTable;
    for (auto vec : intervals) {
      timeTable.push_back(make_pair(vec.first, 1));
      timeTable.push_back(make_pair(vec.second, -1));
    }
    sort(timeTable.begin(), timeTable.end(), cmp);

    vector<pair<int, int>> ans;
    int l = 0, sum = 0;
    while (l<timeTable.size()) {
      int i = l;
      while (i<timeTable.size() && timeTable[i].first==timeTable[l].first) {
        sum += timeTable[i].second;
        i++;
      }
      if (sum==0 && i<timeTable.size()) {
        ans.push_back(make_pair(timeTable[i-1].first, timeTable[i].first));
      }
      l = i;
    }
    return ans;
  }
};

int main() {
  Solution s;
  vector<pair<int, int>> intervals = {make_pair(1,3), make_pair(2,4), make_pair(6,7), make_pair(2,5),
    make_pair(7,8), make_pair(12,20), make_pair(18,22), make_pair(30,31)};
  vector<pair<int, int>> ans;
  ans = s.freeIntervals(intervals);
  for (auto vec : ans) {
    cout << "[" << vec.first << "," << vec.second << "]" << endl;
  }
  return 0;
}
