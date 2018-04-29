#include <vector>
#include <iostream>
using namespace std;

class Solution {
  public:
    int threeSumSmaller(vector<int>& nums, int target) {
      int ans = 0;
      int n = nums.size();
      if (n < 3) return ans;
      sort(nums.begin(), nums.end(), less<int>());
      // process
      for (int i=0; i<n-2; i++) {
        int front = i+1, tail = n-1;
        while (front < tail) {
          int sum = nums[i]+nums[front]+nums[tail];
          if (sum >= target) {
            tail--;
          } else {
            ans += tail-front;
            front++;
          }
        }
      }
      return ans;
    }
};

int main() {
  vector<int> nums;
  int input;
  while (cin >> input) {
    nums.push_back(input);
  }
  int target;
  cin >> target;
  cout << target << endl;
  Solution solution;
  cout << solution.threeSumSmaller(nums, target) << endl;
  return 0;
}
